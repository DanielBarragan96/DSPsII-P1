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

#define QUEUE_END 160

/* Init UART */
void uart_TeraTerm_init ();

/* UART user callback */
void TeraTerm_UART_UserCallback (UART_Type *base, uart_handle_t *handle,
        status_t status, void *userData);

/* UART send data*/
void uart_TeraTerm_send (UART_Type *base, uint8_t* string);

/* UART receibe data*/
void uart_TeraTerm_receive ();

/* Echo function*/
void uart_TeraTerm_echo ();

/* Read Queue */
uint8_t leerQueue_TeraTerm ();

uint8_t longitud_Queue_TeraTerm ();

#endif /* UART_TERATERM_H_ */
