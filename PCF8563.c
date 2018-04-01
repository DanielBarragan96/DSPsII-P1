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

uint8_t time[THREE_BYTE];
uint8_t date[THREE_BYTE];

uint8_t g_time_s[8];
uint8_t g_date_s[8];

volatile static uint8_t format;

uint16_t decToHex(uint8_t val)
{
    uint16_t hb = (val/10) << 0x04;
    uint16_t lb = (0x0F & (val%10));

    uint16_t valHex = hb | lb;

    return valHex;
}

void init_clk ()
{
    static uint8_t buffer = 0x00;
    i2c_writes (PCF8563_SLAVE_ADDRESS, CLK_REGISTER_ADRESS, ONE_BYTE, &buffer,
            PCF_SUBADRESS_SIZE);
    return;
}

uint8_t setTimeFormat(uint8_t newFormat)
{
    if(0 <= newFormat && 1 >= newFormat)
    {
        format = newFormat;
        return 0;
    }
    return -1;
}

uint8_t* getTime ()
{
    i2c_read (PCF8563_SLAVE_ADDRESS, PCF8563_TIME_ADDRESS, THREE_BYTE, &time[0],
            PCF_SUBADRESS_SIZE);
    return &time[0];
}

uint8_t* generateTimeString()
{
    getTime();

    g_time_s[0] = ((0xF0 & time[2])>>4) +48;
    g_time_s[1] = (0x0F & time[2]) +48;
    g_time_s[2] = ':';
    g_time_s[3] = ((0xF0 & time[1])>>4) +48;
    g_time_s[4] = (0x0F & time[1]) +48;
    g_time_s[5] = ':';
    g_time_s[6] = ((0xF0 & time[0])>>4) +48;
    g_time_s[7] = (0x0F & time[0]) +48;
    g_time_s[8] = '\0';

     return &g_time_s[0];
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

uint8_t* generateDateString()
{
    getDate();
    g_date_s[0] = (date[0]/10) +48;
    g_date_s[1] = (date[0]%10) +48;
    g_date_s[2] = '/';
    g_date_s[3] = (date[1]/10) +48;
    g_date_s[4] = (date[1]%10) +48;
    g_date_s[5] = '/';
    g_date_s[6] = (date[2]/10) +48;
    g_date_s[7] = (date[2]%10) +48;
    g_date_s[8] = '\0';

    return &g_date_s[0];
}

uint8_t setTime (uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    uint8_t buffer[3];
    buffer[0] = decToHex(seconds);
    buffer[1] = decToHex(minutes);
    buffer[2] = decToHex(hours);
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
