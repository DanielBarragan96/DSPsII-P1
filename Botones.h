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
#define BOTON_B0_MASK (1)
#define BOTON_B1_MASK (2)
#define BOTON_B2_MASK (4)
#define BOTON_B3_MASK (8)
#define BOTON_B4_MASK (16)
#define BOTON_B5_MASK (32)

typedef enum
{
    BUTTON_0, BUTTON_1, BUTTON_2, NO_BUTTON
} Butons;

/**
 \brief
 Esta funcion inicializa los puerto GPIO para los botones y sus interrupciones
 \return void
 */
void inicializacionBotones ();

/**
 \brief
 Esta funcion regresa el valor del botón presionado
 \return uint8
 */
Butons obtenerBoton ();

/**
 \brief
 Esta funcion limpia la bandera de "Boton presionado"
 \return void
 */
void clearflagB ();

/**
 \brief
 Esta funcion regresa la bandera de "boton presionado"
 \return BooleanType
 */
BooleanType getflagB ();

/**
 \brief
 Esta funcion limpia la bandera de "boton presionado"
 \return void
 */
void clearflagB ();

/**
 \brief
 Esta funcion levanta la bandera de "boton presionado"
 \return void
 */
void setflagB ();

#endif /*DATATYPEDIFINITIONS_H_*/

