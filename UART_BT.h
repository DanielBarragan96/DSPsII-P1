/*
 * UART_BT.h
 *
 *  Created on: Mar 27, 2018
 *      Author: Sergio
 */

#ifndef UART_BT_H_
#define UART_BT_H_

#include "board.h"
#include "DataTypeDefinitions.h"
#include "fsl_uart.h"

void BT_UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

void uart_BT_init();

UART_MailBoxType* uart_BT_receive();
void uart_BT_send(UART_Type *base, uint8_t* string);

#endif /* UART_BT_H_ */
