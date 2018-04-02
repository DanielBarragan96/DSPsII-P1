/*
 * UART_TeraTerm.c
 *
 *  Created on: Mar 27, 2018
 *      Author: Sergio
 */
#include "board.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "UART_TeraTerm.h"
#include "LCDNokia5110.h"
#include "DataTypeDefinitions.h"
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
#define ESC 27

/*******************************************************************************
 * Variables UART
 ******************************************************************************/
uart_handle_t g_uartHandle;
uint8_t g_rxRingBuffer[RX_RING_BUFFER_SIZE] =
{ 0 }; /* RX ring buffer. */
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;
QueueHandle_t g_uart0_queue;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* UART user callback */
void TeraTerm_UART_UserCallback (UART_Type *base, uart_handle_t *handle,
        status_t status, void *userData)
{

    if (kStatus_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing = false;
    }
}

/*Inicializamos UART0
 * Creamos su Handle y la Queue
 */
void uart_TeraTerm_init ()
{

    uart_config_t config;
    UART_GetDefaultConfig (&config);
    config.enableTx = true;
    config.enableRx = true;

    UART_Init (UART0, &config, CLOCK_GetFreq (UART0_CLK_SRC));
    UART_TransferCreateHandle (UART0, &g_uartHandle, TeraTerm_UART_UserCallback,
    NULL);
    g_uart0_queue = xQueueCreate(32, sizeof(UART_MailBoxType));

}

/*Funcion que envia por la terminal un string que recibe como parametro*/
void uart_TeraTerm_send (UART_Type *base, uint8_t* string)
{

    while (*string)    //se transmiten los datos hasta llegar al caracter nulo
    {
        uart_transfer_t xfer;
        xfer.data = string;
        xfer.dataSize = 1;    //sizeof( string) ;
        txOnGoing = true;
        UART_TransferSendNonBlocking (base, &g_uartHandle, &xfer);
        /* Wait send finished */
        while (txOnGoing)
        {
        }
        string++;
    }
}

/*Funcion que recibe los datos de la terminal TeraTerm
 * Al final se guardan uno por uno los datos introduciods a una Queue esto
 * simulando un push() para una FIFO
 */
void uart_TeraTerm_receive ()
{
    uart_TeraTerm_init ();

    uint8_t receiveData[32] =
    { 0 };

    uint8_t i = 0;
    uart_transfer_t xfer;
    xfer.data = (uint8_t*) receiveData;
    xfer.dataSize = sizeof(receiveData) / sizeof(receiveData[0]);
    rxOnGoing = true;
    UART_TransferReceiveNonBlocking (UART0, &g_uartHandle, &xfer,
            &xfer.dataSize);

    while (rxOnGoing)
    {
        if (getShowTime ())
            escribirP (UART0, "\033[11;10H", (sint8 *) generateTimeString ());
        else if (getShowDate ())
            escribirP (UART0, "\033[11;10H", (sint8 *) generateDateString ());

        if (ENTER == receiveData[i])
            rxOnGoing = 0;
        i == 31 ? i = 0 : i++;
        vTaskDelay (pdMS_TO_TICKS(10));
    }

    i = 0;
    while (ENTER != receiveData[i] && i < 32)
    {
        UART_MailBoxType msg;
        msg.mailBox = receiveData[i];
        msg.flagEnter = TRUE;
        xQueueSend(g_uart0_queue, &msg, portMAX_DELAY);
        i++;
    }
}

/*Funcion que imprime en el LCD cualquier valor introducido a la UART */
void uart_TeraTerm_echo ()
{
    uart_TeraTerm_init ();

    uint8_t receiveData[32];
    uint8_t i = 0;
    uart_transfer_t xfer;
    limpiar_lcd ();
    xfer.data = receiveData;
    xfer.dataSize = sizeof(receiveData) / sizeof(receiveData[0]);
    rxOnGoing = true;
    UART_TransferReceiveNonBlocking (UART0, &g_uartHandle, &xfer,
            &xfer.dataSize);

    while (rxOnGoing)
    {
        if (ESC == receiveData[i])
            rxOnGoing = 0;

        imprimir_lcd (xfer.data, 2, 0);
        i == 31 ? i = 0 : i++;
    }
}

/*Obtenemos un valor de la Queue, simula una funcion pop() para una FIFO */
uint8_t leerQueue_TeraTerm ()
{
    UART_MailBoxType msg;
    msg.mailBox = 0;
    uint8_t mensaje;

    xQueueGenericReceive (g_uart0_queue, &msg, pdMS_TO_TICKS(100), pdFALSE);
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
uint8_t longitud_Queue_TeraTerm ()
{
    uint8_t valor = uxQueueMessagesWaiting (g_uart0_queue);
    return valor;
}

