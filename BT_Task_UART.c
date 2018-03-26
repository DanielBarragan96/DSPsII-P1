/*
 * BT_Task_UART.c
 *
 *  Created on: Mar 16, 2018
 *      Author: Sergio
 */
#include <BT_Task_UART.h>
#include "board.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "Fifo.h"
#include "DataTypeDefinitions.h"

#define RX_RING_BUFFER_SIZE 20U
#define ECHO_BUFFER_SIZE 8U
#define ENTER 13


/*******************************************************************************
 * Variables UART
 ******************************************************************************/
uart_handle_t g_UartHandle;
uint8_t g_RxRingBuffer[RX_RING_BUFFER_SIZE] = {0}; /* RX ring buffer. */
volatile bool rxBuffer_Empty = true;
volatile bool txBuffer_Full = false;
volatile bool tx_OnGoing = false;
volatile bool rx_OnGoing = false;
UART_MailBoxType UART4_BT_Mailbox;

/******************************************************************************/
/* UART user callback */

void BT_UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
   userData = userData;

   if (kStatus_UART_TxIdle == status)
   {
       txBuffer_Full = false;
       tx_OnGoing = false;
   }

   if (kStatus_UART_RxIdle == status)
   {
	   rxBuffer_Empty = false;
       rx_OnGoing = false;
   }
}

void PORTA_IRQHandler() {

	PORT_ClearPinsInterruptFlags(PORTA, 1 << 4);
}

void uart_BT_init(){

	uart_config_t config;
	BOARD_InitPins_BT();
	config.baudRate_Bps = 9600;
	config.enableTx = true;
	config.enableRx = true;

	UART_Init(UART0, &config, CLOCK_GetFreq(UART0_CLK_SRC));
	UART_TransferCreateHandle(UART0, &g_UartHandle, BT_UART_UserCallback, NULL);
	UART_TransferStartRingBuffer(UART0, &g_UartHandle, g_RxRingBuffer, RX_RING_BUFFER_SIZE);

	NVIC_EnableIRQ(PORTA_IRQn);
	NVIC_SetPriority(PORTA_IRQn, 5);
	EnableIRQ(UART0_RX_TX_IRQn);

	}

void uart_BT_send(UART_Type *base, uint8_t* string){
	while (*string)//se transmiten los datos hasta llegar al caracter nulo
{
	uart_transfer_t xfer;
	xfer.data = string;
	xfer.dataSize = 1;//sizeof( string) ;
	tx_OnGoing = true;
    UART_TransferSendNonBlocking(UART0, &g_UartHandle, &xfer);
     /* Wait send finished */
      while (tx_OnGoing)
      {
      }
      string++;

	}

}

void uart_BT_receive(UART_Type *base){
	uint8_t receiveData[32];
		uint8_t i=0;
		uart_transfer_t xfer;
		xfer.data = receiveData;
		xfer.dataSize = sizeof(receiveData)/sizeof(receiveData[0]);
		rx_OnGoing = true;
		UART_TransferReceiveNonBlocking(UART0, &g_UartHandle, &xfer, &xfer.dataSize);



		while (rx_OnGoing)
		      {

			if(ENTER == receiveData[i])
				    	rx_OnGoing = 0;
			i==31?i=0:i++;

		      }
	}



void setflagEnter(){
	UART4_BT_Mailbox.flagEnter = TRUE;
}

void clearflagEnter(){
	UART4_BT_Mailbox.flagEnter = FALSE;
}

bool getflagEnter(){
	return UART4_BT_Mailbox.flagEnter;
}


