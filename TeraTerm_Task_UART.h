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


void uart_TeraTerm_init();

/* UART user callback */
void TeraTerm_UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);
void uart_TeraTerm_send(UART_Type *base, uint8_t* string);
void uart_TeraTerm_receive(UART_Type *base, uint8_t* string);
void uart_TeraTerm_echo();

void setflagE();

void clearflagE();

bool getflagE();
#endif /* TERATERM_TASK_UART_H_ */
