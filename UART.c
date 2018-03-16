/*
 * UART.c
 *
 *  Created on: Mar 12, 2018
 *      Author: Sergio
 */
#include "board.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "FIFO.h"


#define RX_RING_BUFFER_SIZE 20U
#define ECHO_BUFFER_SIZE 8U

/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

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
       rxBufferEmpty = false;
       rxOnGoing = false;
   }
}

void uart_init(){
	uart_config_t config;
	uart_transfer_t xfer;
	uart_transfer_t sendXfer;
	uart_transfer_t receiveXfer;
	size_t receivedBytes;
	uint32_t i;

	UART_GetDefaultConfig(&config);

	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx = true;
	config.enableRx = true;

	UART_Init(UART0, &config, CLOCK_GetFreq(UART0_CLK_SRC));
	UART_TransferCreateHandle(UART0, &g_uartHandle, UART_UserCallback, NULL);
	UART_TransferStartRingBuffer(UART0, &g_uartHandle, g_rxRingBuffer, RX_RING_BUFFER_SIZE);

	uint8_t msg[] = "k pedo raza";

}

uint8_t uart_msg(uint8_t g_tipString[]){
	uint8_t msg [] = "";
	if()
	msg = (*getFIFO());
	return msg;
}

void uart_send(){
	xfer.data = g_tipString;
	xfer.dataSize = sizeof(g_tipString) - 1;
	txOnGoing = true;
    UART_TransferSendNonBlocking(DEMO_UART, &g_uartHandle, &xfer);
}
