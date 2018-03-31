/*
 * PCF8583.h
 *
 *  Created on: 03/11/2017
 *      Author: ALEX
 */

#ifndef PCF8563_H_
#define PCF8563_H_

#include "stdint.h"

#define PCF8563_SLAVE_ADDRESS 0x51
#define CLK_REGISTER_ADRESS 0X00
#define PCF_SUBADRESS_SIZE 1
#define ONE_BYTE 1
#define TWO_BYTE 2
#define THREE_BYTE 3
#define HEX_TEN 0x10

#define PCF8563_TIME_ADDRESS 0x02
#define PCF8563_DATE_ADDRESS 0x05
#define PCF8563_MONTH_ADDRESS 0x06


void init_clk();

uint8_t* getTime();

void printTimeTeraTerm();

uint8_t* getDate();

void printDateTeraTerm();

uint8_t setTime (uint8_t hours, uint8_t minutes, uint8_t seconds);

uint8_t setDate (uint8_t day, uint8_t month, uint8_t year);

#endif /* PCF8563_H_ */
