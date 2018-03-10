/*
 * PCF8583.h
 *
 *  Created on: 03/11/2017
 *      Author: ALEX
 */

#ifndef PCF8563_H_
#define PCF8563_H_

#include "DataTypeDefinitions.h"

#define PCF8563_WRITE_ADDRESS 0xA2
#define PCF8563_READ_ADDRESS 0xA3
#define PCF8563_SECONDS_ADDRESS 0x02
#define PCF8563_MINUTES_ADDRESS 0x03
#define PCF8563_HOURS_ADDRESS 0x04
#define PCF8563_DAYS_ADDRESS 0x05
#define PCF8563_MONTHS_ADDRESS 0x07
#define PCF8563_YEARS_ADDRESS 0x08

void PCF8563_setSeconds(uint8 data);
void PCF8563_setMinutes(uint8 data);
void PCF8563_setHours(uint8 data);
void PCF8563_setYears(uint8 data);
void PCF8563_setMonths(uint8 data);
void PCF8563_setDays(uint8 data);

uint8 PCF8563_getSeconds();
uint8 PCF8563_getMinutes();
uint8 PCF8563_getHours();
uint8 PCF8563_getYears();
uint8 PCF8563_getMonths();
uint8 PCF8563_getDays();

void PCF8583_setData(uint8 address, uint8 data);

#endif /* PCF8563_H_ */

