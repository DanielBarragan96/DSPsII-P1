/**
	\file
	\brief
		This is the header file for the UART device driver.
		It contains the macros and function definition.
	\author Daniel Barragán
	\date	2/11/2017
	\todo
		To implement all needed functions
 */
#ifndef TERATERM_H_
#define TERATERM_H_

#include "DataTypeDefinitions.h"

/*! This data type is used to indicate when TeraTerm worked properly*/
typedef enum{
	ERROR,
	GOOD
}TERA_TERM_STATUS;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MainMenu through the serial port.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMainMenu();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuRMemI2C through the serial port.
 	 We have three stages to control this state, this is the first.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuRMemI2C_1();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuRMemI2C through the serial port.
 	 We have three stages to control this state, this is the second.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuRMemI2C_2();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuRMemI2C through the serial port.
 	 We have three stages to control this state, this is the third.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuRMemI2C_3();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuWMemI2C through the serial port.
 	 We have three stages to control this state, this is the first.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuWMemI2C_1();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuWMemI2C through the serial port.
 	 We have three stages to control this state, this is the second.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuWMemI2C_2();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuWMemI2C through the serial port.
 	 We have three stages to control this state, this is the third.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuWMemI2C_3();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MENUSetH through the serial port.
 	 We have two stages to control this state, this is the first.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuSetH_1();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MENUSetH through the serial port.
 	 We have two stages to control this state, this is the second.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuSetH_2();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MENUSetD through the serial port.
 	 We have two stages to control this state, this is the first.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuSetD_1();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MENUSetD through the serial port.
 	 We have two stages to control this state, this is the second.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuSetD_2();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuFH through the serial port.
 	 We have two stages to control this state, this is the first.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuFH_1();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuFH through the serial port.
 	 We have two stages to control this state, this is the second.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuFH_2();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuRH through the serial port.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuRH_1();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MenuRD through the serial port.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuRD_1();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MainMenu through the serial port.
 	 We have two stages to control this state, this is the first.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuEchoLCD_1();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends the MainMenu through the serial port.
 	 We have two stages to control this state, this is the second.
 	 \return GOOD if there were no troubles
 */
uint8 printTTMenuEchoLCD_2();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function stores the value of the number send in asciiVal (tens and units)
 	 \return GOOD if there were no troubles
 */
BooleanType setTimeASCII(uint8 value, BooleanType hour);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function first update the time and date structures and then
 	 write asciiVal of Time in the serial port.
 	 \return TRUE if there were no troubles
 */
BooleanType writeTime();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function first update the time and date structures and then
 	 write asciiVal of Date in the serial port.
 	 \return TRUE if there were no troubles
 */
BooleanType writeDate();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function pops the first two elements of the FIFO and transform this into an
 	  hex value with the two numbers.
 	 \return the hex value to set Date/Time;
 */
sint8 dataSet();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function pops the first two elements of the FIFO and transform this into an
 	  decimal value with the two numbers.
 	 \return the hex value to set Date/Time;
 */
sint8 dataSetInt();

#endif /* TERATERM_H_ */
