
/*
 * PantallaPC.h
 *
 *  Created on: 2 nov. 2017
 *      Author: Avelar Díaz José Francisco
 *      		Chung Correa Sergio Raúl
 *      		Barragán Alvarez Daniel
 */
#include "DataTypeDefinitions.h"

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Funcion que manda las instrucciones al Teraterm de posicion y los datos a imprimir
 	  \param[in]  uartChannel Canal utilizado.
 	  \param[in]  Posicion Posicion donde se va a escribir en el bufer.
 	  \param[in]  string datos a imprimir en el bufer
 	 \return void
 */
void escribirP(UART_Type *base, sint8* Posicion,  sint8* data );

void escribirchat(sint8* Posicion, sint8* string );
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pantalla donde se muestan las 7 opciones de la practica
 	 \return void
 */
void MenuInicial();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pantalla para leer de la memoria
 	 \return void
 */
void LeerM();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pantalla para escribir datos a la memoria
 	 \return void
 */
void EscribirM();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pantalla para modificar la hora
 	 \return void
 */
void Ehora();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pantalla para modificar la fecha
 	 \return void
 */
void Efecha();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pantalla para cambiar el formato de 24hrs a 12
 	 \return void
 */
void Fhora();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pantalla para ver la hora
 	 \return void
 */
void Lhora();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pantalla para ver la fecha
 	 \return void
 */
void Lfecha();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Funcion de espera hasta que terminemos de ingresar los valores
 	 \return void
 */
void ingresoDatos(UART_Type *base);

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Funcion utilizada para corregir el 'desfase' en los valores en ASCII con los
 	 valores en hexadecimal.
 	 \return integer value
 */
uint8 valMemoria();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
     \brief  Mandar mensajes por SPI
     \return void
 */
void Eco();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
     \brief  Iniciar chat entre dos terminales
     \return void
 */
void Comunicacion();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
     \brief  Pantalla para ver la hora
     \return void
 */
void Fecha_Hora();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
     \brief  Inicializa mutex del módulo
     \return void
 */
void initmutex();

void chat(UART_Type *uart);

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
     \brief  Permite cambiar entre diferentes menús
     \return void
 */
uint8_t escogerMenu(UART_Type *uart);

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
     \brief  Retorna el valor de shor_time
     \return void
 */
bool getShowTime();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
     \brief  Retorna el valor de shor_date
     \return void
 */
bool getShowDate();
