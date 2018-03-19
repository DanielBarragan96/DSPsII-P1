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
uart_handle_t g_uartHandle;
uint8_t g_RxRingBuffer[RX_RING_BUFFER_SIZE] = {0}; /* RX ring buffer. */
volatile bool rxBuffer_Empty = true;
volatile bool txBuffer_Full = false;
volatile bool tx_OnGoing = false;
volatile bool rx_OnGoing = false;
UART_MailBoxType UART0_BT_Mailbox;

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

void PORTE_IRQHandler(){

   	PORT_ClearPinsInterruptFlags(PORTE, BIT25);

   }

void uart_init(){

	CLOCK_EnableClock(kCLOCK_PortE);
	port_pin_config_t config_UART ={ kPORT_PullDisable, kPORT_SlowSlewRate,
			kPORT_PassiveFilterDisable, kPORT_OpenDrainDisable,
			kPORT_LowDriveStrength, kPORT_MuxAlt3, kPORT_UnlockRegister, };

	PORT_SetPinInterruptConfig(PORTE, BIT25, kPORT_InterruptLogicOne);

	PORT_SetPinConfig(PORTE, 24, &config_UART);   //TX
	PORT_SetPinConfig(PORTE, 25, &config_UART);  //RX

	UART_Init(UART4, &config_UART, CLOCK_GetFreq(UART0_CLK_SRC));
	UART_TransferCreateHandle(UART4, &g_uartHandle, BT_UART_UserCallback, NULL);
	UART_TransferStartRingBuffer(UART4, &g_uartHandle, g_RxRingBuffer, RX_RING_BUFFER_SIZE);
	NVIC_EnableIRQ(PORTE_IRQn);

	}



void setflagEnter(){
	UART0_BT_Mailbox.flagEnter = TRUE;
}

void clearflagEnter(){
	UART0_BT_Mailbox.flagEnter = FALSE;
}

bool getflagEnter(){
	return UART0_BT_Mailbox.flagEnter;
}


