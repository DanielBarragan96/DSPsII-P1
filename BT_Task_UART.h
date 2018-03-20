/*
 * BT_Task_UART.h
 *
 *  Created on: Mar 16, 2018
 *      Author: Sergio
 */

#ifndef BT_TASK_UART_H_
#define BT_TASK_UART_H_

#include "board.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"

void BT_UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

void setflagEnter();
void uart_BT_init();
void clearflagEnter();
bool getflagEnter();


#endif /* BT_TASK_UART_H_ */
