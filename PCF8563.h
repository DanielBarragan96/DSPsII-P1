/*
 * PCF8583.h
 *
 *  Created on: 03/11/2017
 *      Author: ALEX
 */

#ifndef PCF8563_H_
#define PCF8563_H_

#include "stdint.h"

//PCF8563 slave direction for the I2C
#define PCF8563_SLAVE_ADDRESS 0x51
//calendar clock register directrion
#define CLK_REGISTER_ADRESS 0X00
//memory device subadress size
#define PCF_SUBADRESS_SIZE 1
//reading sizes
#define ONE_BYTE 1
#define TWO_BYTE 2
#define THREE_BYTE 3
//hexadecimal number ten mask
#define HEX_TEN 0x10
//PCF8563 time variables adress
#define PCF8563_TIME_ADDRESS 0x02
//PCF8563 date variables adress
#define PCF8563_DATE_ADDRESS 0x05

typedef struct
{
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}Epoc;
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function initialize the PCF8563 clock
 \return void
 */
void init_clk();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function changes the displayed time format
 \param[in]  newFormat indicate if we want to change the hour format
 \return the data from the memory address we give as a param.
 */
uint8_t setTimeFormat(uint8_t newFormat);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function updates the time variables
 \return the direction of the first element of the time array.
 */
uint8_t* getTime();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function updates the time variables
 \return a string of the time.
 */
uint8_t* generateTimeString();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function updates the date variables
 \return the direction of the first element of the date array.
 */
uint8_t* getDate();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function updates the date variables
 \return a string of the date.
 */
uint8_t* generateDateString();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function changes the time in the PCF8563
        The variables sent, must be in decimal format
 \param[in]  hours
 \param[in]  minutes
 \param[in]  seconds
 \return the data from the memory address we give as a param.
 */
uint8_t setTime (uint8_t hours, uint8_t minutes, uint8_t seconds);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function changes the date in the PCF8563
        The variables sent, must be in decimal format
 \param[in]  day
 \param[in]  month
 \param[in]  year
 \return the data from the memory address we give as a param.
 */
uint8_t setDate (uint8_t day, uint8_t month, uint8_t year);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief This function updates the date variables
 \return a string of the date.
 */
Epoc getEpoc();

#endif /* PCF8563_H_ */
