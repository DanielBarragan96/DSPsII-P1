/*
 * PCF8583.c
 *
 *  Created on: 03/11/2017
 *      Author: ALEX
 */

#include "PCF8563.h"
#include "I2C.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

uint8_t dataFromPCF8563[THREE_BYTE];
uint8_t time[THREE_BYTE];
uint8_t date[THREE_BYTE];

void init_clk ()
{
    static uint8_t buffer = 0x00;
    i2c_writes (PCF8563_SLAVE_ADDRESS, CLK_REGISTER_ADRESS, ONE_BYTE, &buffer,
            PCF_SUBADRESS_SIZE);
    return;
}

uint8_t* getTime ()
{
    i2c_read (PCF8563_SLAVE_ADDRESS, PCF8563_TIME_ADDRESS, THREE_BYTE, &time[0],
            PCF_SUBADRESS_SIZE);
    return &time[0];
}

void printTimeTeraTerm()
{
    getTime();
    PRINTF ("\r");
        if(HEX_TEN <= time[2])
            PRINTF ("%x : ", time[2]);
        else
            PRINTF ("0%x : ", time[2]);

        if(HEX_TEN <= time[1])
            PRINTF ("%x : ", time[1]);
        else
            PRINTF ("0%x : ", time[1]);

        if(HEX_TEN <= time[0])
            PRINTF ("%x  ", time[0]);
        else
            PRINTF ("0%x ", time[0]);
}

uint8_t* getDate ()
{
    i2c_read (PCF8563_SLAVE_ADDRESS, PCF8563_DATE_ADDRESS, TWO_BYTE, &date[0],
            PCF_SUBADRESS_SIZE);
    uint8_t days = ((0x30 & date[0])>>4)*10;
    days += (0x0F & date[0]);

    uint8_t month = ((0x10 & date[1])>>4)*10;
    month += (0x0F & date[1]);

    uint8_t years = (0xC0 & date[0])>>6;

    date[0]=days;
    date[1]=month;
    date[2]=years;

    return &date[0];
}

void printDateTeraTerm()
{
    getDate();
    PRINTF("\r%d / %d / %d", date[0],date[1], date[2]);
}

uint8_t setTime (uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    uint8_t buffer[3];
    buffer[0]=seconds;
    buffer[1]=minutes;
    buffer[2]=hours;
    return i2c_writes (PCF8563_SLAVE_ADDRESS, PCF8563_TIME_ADDRESS, THREE_BYTE,
            &buffer[0], PCF_SUBADRESS_SIZE);
}

uint8_t setDate (uint8_t day, uint8_t month, uint8_t year)
{
    uint8_t buffer[2];
    uint8_t y = ((0x03 & year)<<6);
    uint8_t dt = ((0x03 & (day/10))<<4);
    uint8_t du = (0x0F & (day%10));

    buffer[0] = y | dt | du;

    uint8_t mt = ((0x01 & month/10)<<0x4);
    uint8_t mu = (0x0F & (month%10));
    buffer[1] = mt | mu;

    return i2c_writes (PCF8563_SLAVE_ADDRESS, PCF8563_DATE_ADDRESS, TWO_BYTE,
            &buffer[0], PCF_SUBADRESS_SIZE);
}
