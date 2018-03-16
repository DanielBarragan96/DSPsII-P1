/*
 * UART.c
 *
 *  Created on: Mar 12, 2018
 *      Author: Sergio
 */
#include <TeraTerm_Task_UART.h>
#include "board.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "Fifo.h"

#define RX_RING_BUFFER_SIZE 20U
#define ECHO_BUFFER_SIZE 8U
#define ENTER 13

/*******************************************************************************
 * Variables
 ******************************************************************************/
uart_handle_t g_uartHandle;
uint8_t g_rxRingBuffer[RX_RING_BUFFER_SIZE] = {0}; /* RX ring buffer. */
uint8_t g_rxBuffer[ECHO_BUFFER_SIZE] = {0}; /* Buffer for receive data to echo. */
uint8_t g_txBuffer[ECHO_BUFFER_SIZE] = {0}; /* Buffer for send data to echo. */
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

UART_MailBoxType UART0_MailBox;
/******************************************************************************/
/* UART user callback */

void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
   userData = userData;

   if (kStatus_UART_TxIdle == status)
   {
       txBufferFull = false;
       txOnGoing = false;
   }

   if (kStatus_UART_RxIdle == status)
   {
	   if(ENTER == UART_ReadBlocking(UART0, (uint8_t*) UART0_MailBox.mailBox, sizeof(UART0_MailBox.mailBox)))
		  setflagE();
       rxBufferEmpty = false;
       rxOnGoing = false;
   }
}

void uart_init(){

	uart_config_t config;
	UART_GetDefaultConfig(&config);

	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx = true;
	config.enableRx = true;

	UART_Init(UART0, &config, CLOCK_GetFreq(UART0_CLK_SRC));
	UART_TransferCreateHandle(UART0, &g_uartHandle, UART_UserCallback, NULL);
	UART_TransferStartRingBuffer(UART0, &g_uartHandle, g_rxRingBuffer, RX_RING_BUFFER_SIZE);
}


void uart_send(UART_Type *base, uint8_t* string){
	while (*string)
{//se transmiten los datos hasta llegar al caracter nulo
	uart_transfer_t xfer;
	xfer.data = string;
	xfer.dataSize = 1;//sizeof( string) ;
	txOnGoing = true;
    UART_TransferSendNonBlocking(UART0, &g_uartHandle, &xfer);
     /* Wait send finished */
      while (txOnGoing)
      {
      }
      string++;
	}
}

void uart_receive(UART_Type *base, uint8_t* string){
	uint8_t receiveData[32];
	uart_transfer_t xfer;
	xfer.data = receiveData;
	xfer.dataSize = sizeof(receiveData)/sizeof(receiveData[0]);
	rxOnGoing = true;
	UART_TransferReceiveNonBlocking(UART0, &g_uartHandle, &xfer, &xfer.dataSize);

	if(ENTER == *xfer.data)
	    	rxOnGoing = 0;

	while (rxOnGoing)
	      {
	      }
}

void setflagE(){
	UART0_MailBox.flagEnter = TRUE;
}

void clearflagE(){
	UART0_MailBox.flagEnter = FALSE;
}

bool getflagEnter(){
	return UART0_MailBox.flagEnter;
}
