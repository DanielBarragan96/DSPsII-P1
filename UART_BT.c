/*
 * UART_BT.c
 *
 *  Created on: Mar 27, 2018
 *      Author: Sergio
 */

#include "UART_BT.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "DataTypeDefinitions.h"
#include "pin_mux.h"
#include "LCDNokia5110.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"

#include "PantallaPC.h"
#include "PCF8563.h"

#define RX_RING_BUFFER_SIZE 20U
#define ENTER 13
#define QUEUE_END 160
#define ESC 127

/*******************************************************************************
 * Variables UART
 ******************************************************************************/
uart_handle_t g_UartHandle;
uint8_t g_RxRingBuffer[RX_RING_BUFFER_SIZE] =
{ 0 }; /* RX ring buffer. */
volatile bool rxBuffer_Empty = true;
volatile bool txBuffer_Full = false;
volatile bool tx_OnGoing = false;
volatile bool rx_OnGoing = false;
QueueHandle_t g_uart4_queue;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* UART user callback */

void BT_UART_UserCallback (UART_Type *base, uart_handle_t *handle,
        status_t status, void *userData)
{

    BaseType_t pxHigherPriorityTaskWoken;
    if (kStatus_UART_TxIdle == status)
    {
        txBuffer_Full = false;
        tx_OnGoing = false;
//		xEventGroupSetBitsFromISR(xEventGroup, TX_FINISHED, pxHigherPriorityTaskWoken);
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxBuffer_Empty = false;
        rx_OnGoing = false;
    }
}

/*Inicializamos UART4
 * Creamos su Handle y la Queue
 */
void uart_BT_init ()
{

    uart_config_t config;
    UART_GetDefaultConfig (&config);
    config.baudRate_Bps = 9600;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init (UART4, &config, CLOCK_GetFreq (UART4_CLK_SRC));
    UART_TransferCreateHandle (UART4, &g_UartHandle, BT_UART_UserCallback,
            NULL);
    g_uart4_queue = xQueueCreate(32, sizeof(UART_MailBoxType));

}

/*Funcion que envia por la terminal un string que recibe como parametro*/
void uart_BT_send (UART_Type *base, uint8_t* string)
{

    while (*string)    //se transmiten los datos hasta llegar al caracter nulo
    {
        uart_transfer_t xfer;
        xfer.data = string;
        xfer.dataSize = 1;    //sizeof( string) ;
        tx_OnGoing = true;
        UART_TransferSendNonBlocking (base, &g_UartHandle, &xfer);
        /* Wait send finished */
        while (tx_OnGoing)
        {
        }
        string++;
    }
}

/*Funcion que recibe los datos de la terminal Bluetooth
 * Al final se guardan uno por uno los datos introduciods a una Queue esto
 * simulando un push() para una FIFO
 */
void uart_BT_receive ()
{
    uart_BT_init ();

    uint8_t receiveData[32] =
    { 0 };

    uint8_t i = 0;
    uart_transfer_t xfer;
    xfer.data = (uint8_t*) receiveData;
    xfer.dataSize = sizeof(receiveData) / sizeof(receiveData[0]);
    rx_OnGoing = true;
    UART_TransferReceiveNonBlocking (UART4, &g_UartHandle, &xfer,
            &xfer.dataSize);

    while (rx_OnGoing)
    {
        if (getShowTimeBT ())
            escribirP (UART4, "\033[11;10H", (sint8 *) generateTimeString ());
        else if (getShowDateBT ())
            escribirP (UART4, "\033[11;10H", (sint8 *) generateDateString ());

        if (ENTER == receiveData[i])
            rx_OnGoing = 0;
        i == 31 ? i = 0 : i++;
        vTaskDelay (pdMS_TO_TICKS(10));
    }

    i = 0;
    while (ENTER != receiveData[i] && i < 32)
    {
        UART_MailBoxType msg;
        msg.mailBox = receiveData[i];
        msg.flagEnter = TRUE;
        xQueueSend(g_uart4_queue, &msg, portMAX_DELAY);
        i++;
    }
}

/*Funcion que imprime en el LCD cualquier valor introducido a la UART */
void uart_BT_echo ()
{
    uart_BT_init ();

    uint8_t receiveData[32];
    uint8_t i = 0;
    uart_transfer_t xfer;
    limpiar_lcd ();
    xfer.data = receiveData;
    xfer.dataSize = sizeof(receiveData) / sizeof(receiveData[0]);
    rx_OnGoing = true;
    UART_TransferReceiveNonBlocking (UART4, &g_UartHandle, &xfer,
            &xfer.dataSize);

    while (rx_OnGoing)
    {

        if (ESC == receiveData[i])
            rx_OnGoing = 0;

        imprimir_lcd (xfer.data, 2, 0);
        i == 31 ? i = 0 : i++;
    }
}

/*Obtenemos un valor de la Queue, simula una funcion pop() para una FIFO */
uint8_t leerQueue_BT ()
{
    UART_MailBoxType msg;
    msg.mailBox = 0;
    uint8_t mensaje;

    xQueueGenericReceive (g_uart4_queue, &msg, pdMS_TO_TICKS(10), pdFALSE);
    mensaje = msg.mailBox;
    msg.flagEnter = false;

    if (0 == mensaje)
    {
        return QUEUE_END;
    }

    else
        return mensaje;
}

/*Obtenemos el valor de la longitud de nuestra Queue */
uint8_t longitud_Queue_BT ()
{
    uint8_t valor = uxQueueMessagesWaiting (g_uart4_queue);
    return valor;
}
