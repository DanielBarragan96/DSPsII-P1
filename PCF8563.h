/*
 * PCF8583.h
 *
 *  Created on: 03/11/2017
 *      Author: ALEX
 */

#ifndef PCF8563_H_
#define PCF8563_H_

#include "stdint.h"

#define PCF8563_WRITE_DATA_ADDRESS 0x51
#define CLK_REGISTER_ADRESS 0X00

#define ONE_BYTE 1

#define PCF8563_WRITE_ADDRESS 0x51
#define PCF8563_READ_ADDRESS 0x51

#define PCF8563_TIME_ADDRESS 0x04
#define PCF8563_DATE_ADDRESS 0x05

#define PCF8563_SECONDS_ADDRESS 0x02
#define PCF8563_MINUTES_ADDRESS 0x03
#define PCF8563_HOURS_ADDRESS 0x04

#define PCF8563_DAYS_ADDRESS 0x05
#define PCF8563_MONTHS_ADDRESS 0x07
#define PCF8563_YEARS_ADDRESS 0x08

uint8_t PCF8563_setSeconds (uint8_t data);
uint8_t PCF8563_setMinutes (uint8_t data);
uint8_t PCF8563_setHours (uint8_t data);
uint8_t PCF8563_setYears (uint8_t data);
uint8_t PCF8563_setMonths (uint8_t data);
uint8_t PCF8563_setDays (uint8_t data);

uint8_t PCF8563_getSeconds ();
uint8_t PCF8563_getMinutes ();
uint8_t PCF8563_getHours ();
uint8_t PCF8563_getYears ();
uint8_t PCF8563_getMonths ();
uint8_t PCF8563_getDays ();

uint8_t PCF8583_setData (uint8_t address, uint8_t data);

void init_clk();

uint8_t getTime();

#endif /* PCF8563_H_ */
