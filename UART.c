/*
 * UART.c
 *
 *  Created on: 10 oct. 2017
 *      Author: Avelar Díaz José Francisco
 *      		Chung Correa Sergio Raúl
 */

#include "UART.h"
#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "PantallaPC.h"
#include "Fifo.h"

UART_Type (*UART[6]) = {UART0,UART1,UART2,UART3,UART4,UART5};

UART_MailBoxType UART0_MailBox;
#define ENTER 13

void setflagE(){
	UART0_MailBox.flagEnter = TRUE;
}

void clearflagE(){
	UART0_MailBox.flagEnter = FALSE;
}

BooleanType getflagEnter(){
	return UART0_MailBox.flagEnter;
}

void setFlagM(){
	UART0_MailBox.flag = TRUE;
}

void clearFlagM(){
	UART0_MailBox.flag = FALSE;
}

BooleanType getFlagM(){
	return UART0_MailBox.flag;
}


/*!
 	 \brief	 This is the interrupt services routing (ISR).
 	 \param[in]  void.
 	 \return void
 */
void UART0_RX_TX_IRQHandler(void){
	if((UART0->S1 & UART_S1_RDRF_MASK)){//Se asegura si en el status register 1, el RDRF estÃ¡ en 0, esto quiere decir que se terminÃ³ de recibir la informaciÃ³n
		UART0_MailBox.mailBox = UART0->D; //se guarda el valor recibido en el MailBoxk, en este caso en UART0_D
		setFlagM(); //se activa la bandera del MailBox, esto quiere decir que contiene datos
		if(ENTER == UART0_MailBox.mailBox)//si presionamos ENTER, encendemos su bandera y no guardamos en la FIFO
			setflagE();
		else if(FALSE == UART0_MailBox.flagEnter){//si presionamos cualquier otra tecla, guardamos el valor en la FIFO
			push(UART0_MailBox.mailBox);
		}
	}
}


void UART_init(UART_ChannelType uartChannel, uint32 systemClk, UART_BaudRateType baudRate){//funcion de inicializacion del UART
	uint32 UART_clocks[6] = {systemClk,systemClk,systemClk/2,systemClk/2,systemClk/2,systemClk/2};//se configura las fuentes de reloj. En UART0 y 1 es 21MHz. En las restantes es a velocidad del bus (21MHz/2)
	/*variable utilizadas para el valor del baudrate*/
	uint16 sbr = 0;
	uint8 brfa = 0;

	switch(uartChannel){//para cada UART se le asigna su clock gating
			case UART_0:
				SIM->SCGC4 |=SIM_SCGC4_UART0_MASK;
				break;
			case UART_1:
				SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
				break;
			case UART_2:
				SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
				break;
			case UART_3:
				SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
				break;
			case UART_4:
				SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
				break;
			case UART_5:
				SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;
				break;
		}
	/*utilizando la ecuacion UART baud rate = UART module clock / (16 Ã— (SBR[12:0] + BRFD)) calculamos SBR y BRFD(brfa)*/
	sbr = (uint16) (UART_clocks[uartChannel]/(16*baudRate));
	brfa = (uint8 ) ((UART_clocks[uartChannel]/baudRate) - (16*sbr))*2;

	/*limpiamos los valores del baudrate en su parte alta y baja*/
	UART[uartChannel]->BDH &= ~UART_BDH_SBR_MASK;
	UART[uartChannel]->BDL &= ~UART_BDL_SBR_MASK;

	UART[uartChannel]->C2 &= (~UART_C2_TE_MASK & ~UART_C2_RE_MASK);//se deshabilita el transmisor y receptor del control register 2

	UART[uartChannel]->BDH =  ((sbr & UART_BDH_SBR_MASK) >> 8);//se copian los valores de la parte alta de sbr a los bits SBR del registro BDH del UART0
	UART[uartChannel]->BDL |= (uint8) (sbr & UART_BDL_SBR_MASK);//se copian los valores de sbr al registro BDL con un casting de 8bits pues sbr es de 16

	/*limpiamos y despuÃ©s copiamos los valores de brfa al registro C4 en el campo de BRFA*/
	UART[uartChannel]->C4 &= ~UART_C4_BRFA_MASK;
	UART[uartChannel]->C4 |= UART_C4_BRFA(brfa);

	UART[uartChannel]->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;//habilitamos el transmisor y el receptor
}

void UART_interruptEnable(UART_ChannelType uartChannel){//funcion para habilitar la interrupcion
	UART[uartChannel]->C2 |= UART_C2_RIE_MASK; //habilita la interrupciÃ³n de recepciÃ³n en el registro C2
//	UART[uartChannel]->C2 |= UART_C2_RIE_MASK | UART_C2_TIE_MASK;
	while ((UART[uartChannel]->S1 & UART_S1_RDRF_MASK));//si se sigue enviando informaciÃ³n queda ciclado
}

void UART_putChar (UART_ChannelType uartChannel, uint8 character){
	 while(!(UART[uartChannel]->S1 & UART_S1_TDRE_MASK));//con la bandera TDRE revisamos si no estamos transmitiendo datos
	 UART[uartChannel]->D = character;//escribimos el valor directamente al regristro UARTx_x
}

uint8 UART_getChar (UART_ChannelType uartChannel, uint8 character){
	 while(!(UART[uartChannel]->S1 & UART_S1_RDRF_MASK));//con la bandera TDRE revisamos si no estamos transmitiendo datos
	 return UART[uartChannel]->D;//valor de puerto serial
}

void UART_putString(UART_ChannelType uartChannel, sint8* string){//hacemos un uso del paso por referencia de la variable string
	while (*string){//se transmiten los datos hasta llegar al caracter nulo
		UART_putChar(uartChannel, *string++);
	}
}






