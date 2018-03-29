/*
 * PCF8583.c
 *
 *  Created on: 03/11/2017
 *      Author: ALEX
 */

#include "PCF8563.h"
#include "I2C.h"
#include "fsl_i2c.h"

#define SUBADRESS_SIZE 1
uint8_t dataFromPCF8563[3];

uint8_t PCF8563_setSeconds (uint8_t data)
{
    return i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_SECONDS_ADDRESS,
            sizeof(data), &data, SUBADRESS_SIZE);
}

uint8_t PCF8563_getSeconds ()
{
    uint8_t* pdataFromPCF8563 = &dataFromPCF8563;
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_SECONDS_ADDRESS,
            sizeof(dataFromPCF8563), pdataFromPCF8563, SUBADRESS_SIZE);
    return dataFromPCF8563[0];
}

uint8_t PCF8563_setMinutes (uint8_t data)
{
    return i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_MINUTES_ADDRESS,
            sizeof(data), &data, SUBADRESS_SIZE);
}

uint8_t PCF8563_getMinutes ()
{
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_MINUTES_ADDRESS,
            sizeof(dataFromPCF8563), &dataFromPCF8563[0], SUBADRESS_SIZE);
    return dataFromPCF8563[0];
}

uint8_t PCF8563_setHours (uint8_t data)
{
    return i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_HOURS_ADDRESS,
            sizeof(data), &data, SUBADRESS_SIZE);
}

uint8_t PCF8563_getHours ()
{
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_HOURS_ADDRESS,
            sizeof(dataFromPCF8563), &dataFromPCF8563[0], SUBADRESS_SIZE);
    return dataFromPCF8563[0];
}

uint8_t PCF8563_setYears (uint8_t data)
{
    return i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_YEARS_ADDRESS,
            sizeof(data), &data, SUBADRESS_SIZE);
}

uint8_t PCF8563_getYears ()
{
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_YEARS_ADDRESS,
            sizeof(dataFromPCF8563), &dataFromPCF8563[0], SUBADRESS_SIZE);
    return dataFromPCF8563[0];
}

uint8_t PCF8563_setMonths (uint8_t data)
{
    return i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_MONTHS_ADDRESS,
            sizeof(data), &data, SUBADRESS_SIZE);
}

uint8_t PCF8563_getMonths ()
{
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_MONTHS_ADDRESS,
            sizeof(dataFromPCF8563), &dataFromPCF8563[0], SUBADRESS_SIZE);
    return dataFromPCF8563[0];
}

uint8_t PCF8563_setDays (uint8_t data)
{
    return i2c_writes (PCF8563_WRITE_ADDRESS, PCF8563_DAYS_ADDRESS,
            sizeof(data), &data, SUBADRESS_SIZE);
}

uint8_t PCF8563_getDays ()
{
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_DAYS_ADDRESS,
            sizeof(dataFromPCF8563), &dataFromPCF8563[0], SUBADRESS_SIZE);
    return dataFromPCF8563[0];
}

uint8_t PCF8583_setData (uint8_t address, uint8_t data)
{
    return i2c_writes (PCF8563_WRITE_DATA_ADDRESS, address, sizeof(data), &data, SUBADRESS_SIZE);
}

void init_clk()
{
    static i2c_master_transfer_t masterXfer;
    static uint8_t buffer = 0x00;
    i2c_writes(PCF8563_READ_ADDRESS, CLK_REGISTER_ADRESS, ONE_BYTE, buffer, ONE_BYTE);
}

uint8_t getTime()
{
    i2c_read (PCF8563_READ_ADDRESS, PCF8563_TIME_ADDRESS,
                1, &dataFromPCF8563[0], SUBADRESS_SIZE);
        return dataFromPCF8563[0];
}
