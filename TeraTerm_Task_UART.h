/*
 * UART.h
 *
 *  Created on: Mar 12, 2018
 *      Author: Sergio
 */

#ifndef TERATERM_TASK_UART_H_
#define TERATERM_TASK_UART_H_

#include "board.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"

/**
 * \brief A mail box type definition for serial port
 */
typedef struct{
	bool flag; /** Flag to indicate that there is new data*/
	bool flagEnter;
	uint8_t mailBox; /** it contains the received data*/
}UART_MailBoxType;


/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

void uart_send(UART_Type *base, uint8_t* string);
void uart_receive(UART_Type *base, uint8_t* string);

void setflagE();

void clearflagE();

bool getflagEnter();
#endif /* TERATERM_TASK_UART_H_ */
