/*
 * PCF8583.c
 *
 *  Created on: 21/03/2018
 *      Author: Daniel Barragán Alvarez
 */

#include "PCF8563.h"
#include "I2C.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "semphr.h"

//array to store readed values of time
uint8_t time[THREE_BYTE];
//array to store readed values of date
uint8_t date[THREE_BYTE];
//string for time
uint8_t g_time_s[11];
//string for date
uint8_t g_date_s[8];
//format en 0 es 24, en 1 es am/pm
volatile static uint8_t format_h;
SemaphoreHandle_t mutex_time;
SemaphoreHandle_t mutex_date;

//this function changes the variable val from decimal format to hexadecimal
uint16_t decToHex(uint8_t val)
{
    //separate the number by the higher byte
    uint16_t hb = (val/10) << 0x04;
    //separate the number by the lower byte
    uint16_t lb = (0x0F & (val%10));
    //combine bytes
    uint16_t valHex = hb | lb;

    return valHex;
}

void init_clk ()
{
    static uint8_t buffer = 0x00;//write cero in register 0 to start PCF8563
    i2c_writes (PCF8563_SLAVE_ADDRESS, CLK_REGISTER_ADRESS, ONE_BYTE, &buffer,
            PCF_SUBADRESS_SIZE);
    mutex_time = xSemaphoreCreateMutex();
    mutex_date = xSemaphoreCreateMutex();
    return;
}

uint8_t setTimeFormat(uint8_t newFormat)
{
    if(1 == newFormat)
    {//toggle format value
        xSemaphoreTake(mutex_time, portMAX_DELAY);
        format_h ^= 0x01;
        xSemaphoreGive(mutex_time);
        return 0;
    }//if other value than one was sent
    return -1;
}

uint8_t* getTime ()
{//update time array values
    i2c_read (PCF8563_SLAVE_ADDRESS, PCF8563_TIME_ADDRESS, THREE_BYTE, &time[0],
            PCF_SUBADRESS_SIZE);
    return &time[0];
}

uint8_t* generateTimeString()
{
    xSemaphoreTake(mutex_time, portMAX_DELAY);
    //update time variables
    getTime();
    //if am/pm format is needed
    if(format_h){
        //change hours to decimal format
        uint8_t hd =((0xF0 & time[2])>>4);
        uint8_t hu = (0x0F & time[2]);
        uint8_t hours = (hd*10) + hu;
        //must change hours variables iun the array
        // also the am/pm sections
        if(12>=hours)
        {//if less than 12 hours the its am
            g_time_s[0] = (hours / 10) +48;
            g_time_s[1] = (hours % 10) +48;
            g_time_s[9] = 'A';
        }
        else
        {//if morethan 12 hours the its pm
            g_time_s[0] = ((hours-12) / 10) +48;
            g_time_s[1] = ((hours-12) % 10) +48;
            g_time_s[9] = 'P';
        }
        g_time_s[10] = 'M';
    }
    else
    {
        g_time_s[0] = ((0xF0 & time[2])>>4) +48;
        g_time_s[1] = (0x0F & time[2]) +48;
        //am/pm section with spaces
        g_time_s[9] = ' ';
        g_time_s[10] = ' ';
    }
    //other time variables casted to ASCII
        g_time_s[2] = ':';
        g_time_s[3] = ((0xF0 & time[1])>>4) +48;
        g_time_s[4] = (0x0F & time[1]) +48;
        g_time_s[5] = ':';
        g_time_s[6] = ((0xF0 & time[0])>>4) +48;
        g_time_s[7] = (0x0F & time[0]) +48;
        g_time_s[8] = ' ';
        //indicate end of the string
        g_time_s[11] = '\0';
        xSemaphoreGive(mutex_time);
     //return direction of the first element of the string
     return &g_time_s[0];
}

uint8_t* getDate ()
{
    //update date variables
    i2c_read (PCF8563_SLAVE_ADDRESS, PCF8563_DATE_ADDRESS, TWO_BYTE, &date[0],
            PCF_SUBADRESS_SIZE);

    //Cast date variables to deciaml format
    uint8_t days = ((0x30 & date[0])>>4)*10;
    days += (0x0F & date[0]);

    uint8_t month = ((0x10 & date[1])>>4)*10;
    month += (0x0F & date[1]);

    uint8_t years = (0xC0 & date[0])>>6;

    //store decimal formats in the date array
    date[0]=days;
    date[1]=month;
    date[2]=years;

    return &date[0];
}

uint8_t* generateDateString()
{
    xSemaphoreTake(mutex_date, portMAX_DELAY);
    //update date variables
    getDate();
    //cast array of date to a string
    g_date_s[0] = (date[0]/10) +48;
    g_date_s[1] = (date[0]%10) +48;
    g_date_s[2] = '/';
    g_date_s[3] = (date[1]/10) +48;
    g_date_s[4] = (date[1]%10) +48;
    g_date_s[5] = '/';
    g_date_s[6] = (date[2]/10) +48;
    g_date_s[7] = (date[2]%10) +48;
    xSemaphoreGive(mutex_date);
    //indicate end of the string
    g_date_s[8] = '\0';
    //return direction of the first element of the string
    return &g_date_s[0];
}

uint8_t setTime (uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    uint8_t buffer[3];
    xSemaphoreTake(mutex_time, portMAX_DELAY);
    //change time variables sent to hex format
    buffer[0] = decToHex(seconds);
    buffer[1] = decToHex(minutes);
    buffer[2] = decToHex(hours);
    //update the time using I2C
    uint8_t val = i2c_writes (PCF8563_SLAVE_ADDRESS, PCF8563_TIME_ADDRESS, THREE_BYTE,
            &buffer[0], PCF_SUBADRESS_SIZE);
    xSemaphoreGive(mutex_time);
    return val;
}

uint8_t setDate (uint8_t day, uint8_t month, uint8_t year)
{
    xSemaphoreTake(mutex_date, portMAX_DELAY);
    uint8_t buffer[2];
    //change date variables according to data sheet directions
    uint8_t y = ((0x03 & year)<<6);
    uint8_t dt = ((0x03 & (day/10))<<4);
    uint8_t du = (0x0F & (day%10));
    //combine all variables
    buffer[0] = y | dt | du;
    //change date variables according to data sheet directions
    uint8_t mt = ((0x01 & month/10)<<0x4);
    uint8_t mu = (0x0F & (month%10));
    //combine all variables
    buffer[1] = mt | mu;
    //update date using I2C
    uint8_t val = i2c_writes (PCF8563_SLAVE_ADDRESS, PCF8563_DATE_ADDRESS, TWO_BYTE,
            &buffer[0], PCF_SUBADRESS_SIZE);
    xSemaphoreGive(mutex_date);
    return val;
}

Epoc getEpoc()
{
    getTime();
    getDate();

    Epoc epoc;
    epoc.day = date[0];
    epoc.month = date[1];
    epoc.year = date[2];
    uint8_t hd =((0xF0 & time[2])>>4);
    uint8_t hu = (0x0F & time[2]);
    epoc.hour = (hd*10) + hu;
    hd =((0xF0 & time[1])>>4);
    hu = (0x0F & time[1]);
    epoc.minute = (hd*10) + hu;
    hd =((0xF0 & time[0])>>4);
    hu = (0x0F & time[0]);
    epoc.second = (hd*10) + hu;;

    return epoc;
}
