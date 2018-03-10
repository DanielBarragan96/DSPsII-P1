/**
	\file States.h
	\brief
		This is the header file for controlling the system status
	\author Daniel Barragán
	\date	05/11/2017
 */

#ifndef STATES_H_
#define STATES_H_

#include "DataTypeDefinitions.h"

#define STATE_MACHINE_SIZE 4 // size of the State Machine
#define READ_NUMBER_ERRO 100 // represents error reading number for date

/*! This data type represents all the limits and differentials to cast a ASCII to an HEX*/
typedef enum{
	NUMBER_LOW=48,
	NUMBER_HIGH=57,
	MINLETTER_LOW=97,
	MINLETTER_HIGH=102,
	MINLETTER_DIF=87,
	UPLETTER_LOW=65,
	UPLETTER_HIGH=70,
	UPLETTER_DIF=55,
	ERROR_ADSRESS=0000
}ASCIIDecode;
/*! This data type represents the Time values*/
typedef struct{
	uint8 Seconds;
	uint8 Minutes;
	uint8 Hours;
}Time;
/*! This data type represents the Date values*/
typedef struct{
	uint8 Years;
	uint8 Months;
	uint8 Days;
}Date;
/*! This data type represents the screen status*/
typedef enum{//each value is set in ASCII value
	PRINCIPAL= '0',
	R_MEM,
	W_MEM,
	ST_H,
	ST_D,
	FORM_H,
	R_H,
	R_D,
	COM_T2,
	ECO
}ProgramStatus;
/*! This data type is used to control the sub functions of the State Machine*/
typedef enum{
	CERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	TEN=10
}Index;
/*! This data type is used to handle the AM/PM time format*/
typedef enum{
	FORM24,
	FORM12,
	FORM12_LIMIT=0X12,
	FORM12_LIMIT_DEC = 12
}HourFormat;
/*! This data type is used to control the serial port screen*/
typedef struct
{
	uint8(*fptrFirst)();
	uint8(*fptrSecond)();
	uint8(*fptrThird)();
	uint8(*fptrFour)();
}StateType;
/*! This data type is used to control the system status*/
typedef struct{
	ProgramStatus currentStatus;
	uint8	stateIndex;
	Time currentTime;
	Date currentDate;
	uint16 address;
	HourFormat hourFormat;
}SystemControl;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It is used when the current state has finished, and we need to reset the serial port.
 	 \return TRUE if there were no troubles
 */
BooleanType noFunction();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function updates the serial port according to the current sub function index.
 	 \return TRUE if there were no troubles
 */
BooleanType controlSystem();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function handles the ENTER pressed, or when the sub function index
 	 its out of limits.
 	 \return TRUE if there were no troubles
 */
BooleanType controlMenu();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function returns the direction of the currentSystem variable.
 	 \return currentSystem direction
 */
SystemControl* getSystem();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function returns the value of the FIFOIndex.
 	 \return TRUE if there were no troubles
 */
BooleanType updateSystemTimeDate();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function updates the system address.
 	 \param[in]  newAddressH high part of the memory.
 	 \param[in]  newAddressL high part of the memory.
 	 \return TRUE if there were no troubles
 */
BooleanType setSystemAddress();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function pauses the excecution of TeraTerm.
 	 \return TRUE if there were no troubles
 */
BooleanType pauseExcecution();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function toggle hourFormat.
 	 \return TRUE if there were no troubles
 */
BooleanType toggleHourFormat();

#endif /*STATES_H_*/
