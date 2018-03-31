
/*
 * PantallaPC.h
 *
 *  Created on: 2 nov. 2017
 *      Author: Avelar Díaz José Francisco
 *      		Chung Correa Sergio Raúl
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

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Init TX, RX, baudrate UART, Habilitar interrupciones de UART y NVIC
 	\return void
 */
void initP();

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
 	 \brief	 Funcion que convierte valores del reloj o memoria enteros a un array
 	 \param[in]  x Dato a convertir.
 	 \return sint8 array
 */
sint8* valorMem(uint8 x);

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Apagar bandera del formato de hora
 	 \return void
 */
void clearFlagF();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Prender bandera del formato de hora
 	 \return void
 */
void setFlagF();

//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Regresa el valor de la bandera. Si queremos cambiar el formato regresa TRUE
 	 \return Booleantype
 */
BooleanType getFlagF();

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
 	 \brief	 Funcion utilizada para corregir el valor de la hora si cambiamos el formato
 	 valores en hexadecimal.
 	 \param[in]  x valor recivido del 8563
 	 \return integer value
 */
uint8 formatoHora(uint8 x);

void Eco();

void Comunicacion();

void Fecha_Hora();
void initmutex();
void chat( void* args );

uint8_t escogerMenu(UART_Type *uart);
