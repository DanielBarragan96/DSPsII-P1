/*
 * UART_TeraTerm.h
 *
 *  Created on: Mar 27, 2018
 *      Author: Sergio
 */

#ifndef UART_TERATERM_H_
#define UART_TERATERM_H_

#include "board.h"
#include "fsl_uart.h"
#include "DataTypeDefinitions.h"


void uart_TeraTerm_init();

/* UART user callback */
void TeraTerm_UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);
void uart_TeraTerm_send(UART_Type *base, uint8_t* string);
UART_MailBoxType* uart_TeraTerm_receive();
void uart_TeraTerm_echo();

bool getflagE();


#endif /* UART_TERATERM_H_ */
