/*
 * PCF8583.c
 *
 *  Created on: 03/11/2017
 *      Author: ALEX
 */

#include "PCF8563.h"
#include "I2C.h"

void PCF8563_setSeconds(uint8_t data)
{
    i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_SECONDS_ADDRESS, sizeof(data), &data);
}

uint8_t PCF8563_getSeconds()
{
    uint8_t dataFromPCF8563;
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_SECONDS_ADDRESS, sizeof(dataFromPCF8563), &dataFromPCF8563);
    return(dataFromPCF8563);
}

void PCF8563_setMinutes(uint8_t data)
{
    i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_MINUTES_ADDRESS, sizeof(data), &data);
}

uint8_t PCF8563_getMinutes()
{
    uint8_t dataFromPCF8563;
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_MINUTES_ADDRESS, sizeof(dataFromPCF8563), &dataFromPCF8563);
    return(dataFromPCF8563);
}

void PCF8563_setHours(uint8_t data)
{
    i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_HOURS_ADDRESS, sizeof(data), &data);
}

uint8_t PCF8563_getHours()
{
    uint8_t dataFromPCF8563;
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_HOURS_ADDRESS, sizeof(dataFromPCF8563), &dataFromPCF8563);
    return(dataFromPCF8563);
}

void PCF8563_setYears(uint8_t data)
{
    i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_YEARS_ADDRESS, sizeof(data), &data);
}

uint8_t PCF8563_getYears()
{
    uint8_t dataFromPCF8563;
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_YEARS_ADDRESS, sizeof(dataFromPCF8563), &dataFromPCF8563);
    return(dataFromPCF8563);
}

void PCF8563_setMonths(uint8_t data)
{
    i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_MONTHS_ADDRESS, sizeof(data), &data);
}

uint8_t PCF8563_getMonths()
{
    uint8_t dataFromPCF8563;
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_MONTHS_ADDRESS, sizeof(dataFromPCF8563), &dataFromPCF8563);
    return(dataFromPCF8563);
}

void PCF8563_setDays(uint8_t data)
{
    i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_DAYS_ADDRESS, sizeof(data), &data);
}

uint8_t PCF8563_getDays()
{
    uint8_t dataFromPCF8563;
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_DAYS_ADDRESS, sizeof(dataFromPCF8563), &dataFromPCF8563);
    return(dataFromPCF8563);
}

void PCF8583_setData(uint8_t address, uint8_t data)
{
    i2c_writes (PCF8563_WRITE_DATA_ADDRESS, address, sizeof(data), &data);
}
