/*
 * Fifo.c
 *
 *  Created on: Mar 27, 2018
 *      Author: Sergio
 */
#include "Fifo.h"
#include "DataTypeDefinitions.h"

sint8 FIFO[16];
uint16 size = 16;
uint16 contador = 0;
BooleanType Fflag = FALSE;

/*
 * Funcion Push de la FIFO donde obtenemos el dato, revisamos si nuestro índice es menor que el
 * tamaño de la FIFO e ingresamos el dato. Si indice excede el tamaño, prendemos la bandera y
 * ponemos el contador en 0
 */
void push(uint8 data){
		if(contador<size){
		FIFO[contador++] = data;
		}
		else{
			setFIFO_flag();
			resetContador();
		}
}

/*
 * Obtenemos el valor contenido en la primer posicion de la FIFO, después recorremos toda la FIFO
 * una posicion y devolvemos el valor obtenido restando 48 (para convertirlo de ASCII a hexadecimal)
 */
uint8 pop(){
	uint8  variable = FIFO[0];

	uint8 i ;
	for(i = 0;i < (size-2) ;i++){

		FIFO[i] = FIFO[i+1];
	}
	FIFO[size-1] = 0;
	return (variable-48);
}

/*
 * Encendemos la bandera para cuando la FIFO esta llena
 */
void setFIFO_flag(){
	Fflag = TRUE;
}

/*
 * Limipiamos la bandera de la FIFO
 */
void clearFIFO_flag(){
	Fflag = FALSE;
}

/*
 * Obtenemos el estado de la bandera
 */
BooleanType getFIFO_flag(){
	return Fflag;
}

/*
 * retornamos el primer valor ingresado a la FIFO
 */
sint8 *getFIFO(){
	return &FIFO[0];
}

/*
 * Ponemos el valor de nuestro indice en 0 y limpiamos la FIFO
 */
void resetContador(){
	contador=0;
	for(uint8 i=0;i<size;i++){
		FIFO[i] = 0;
	}
}


