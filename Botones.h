/*
 * Teclado.h

 *
 *  Created on: 11/08/2017
 *      Author: Sergio Chung, Francisco Avelar
*/
#ifndef BOTONES_H_
#define BOTONES_H_

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "MK64F12.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "DataTypeDefinitions.h"

/*Masks for the values from the 6 buttons*/
#define BOTON_B0_MASK (0x20)
#define BOTON_B1_MASK (0x10)
#define BOTON_B2_MASK (0x8)
#define BOTON_B3_MASK (0x4)
#define BOTON_B4_MASK (0x2)
#define BOTON_B5_MASK (0x1)


/**
 	 \brief
 	 	 Esta funcion inicializa los puerto GPIO para los botones y sus interrupciones
 	 \param[in] portName es el puerto en el que están los botones.
 	 \return void
*/
void inicializacionBotones(PORT_Type *base, GPIO_Type *baseGPIO);




/**
 	 \brief
 	 	 Esta funcion revisa los botones para obtener el que fue presionado, regresa el indice del mismo.
 	 \param[in] portName es el puerto en el que están los botones
 	 \return uint8
*/
uint8 obtenerBoton(GPIO_Type *baseGPIO);




/**
 	 \brief
 	 	 Esta funcion limpia la bandera de "Boton presionado"
 	 \return void
*/
void clearflagB();



/**
 	 \brief
 	 	 Esta funcion regresa la bandera de "boton presionado"
 	 \return BooleanType
*/
BooleanType getflagB();



/**
 	 \brief
 	 	 Esta funcion limpia la bandera de "boton presionado"
 	 \return void
*/
void clearflagB();



/**
 	 \brief
 	 	 Esta funcion levanta la bandera de "boton presionado"
 	 \return void
*/
void setflagB();


#endif /*DATATYPEDIFINITIONS_H_*/



