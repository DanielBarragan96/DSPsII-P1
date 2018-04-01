/*
 * MEM24LC256.c
 *  Source file of the 24LC256
 *  Created on: 14/03/2018
 *      Author: Daniel Barragán
 */

#include "MEM24LC256.h"
#include "I2C.h"

//return the size of a string
uint8_t checkStringSize (uint8_t* string)
{
    uint8_t size = 0;
    while (*string)
    { //until the end of the string
        size++; //increment word count
        string++; //increment string index
    }
    return size; //return word count
}

int8_t MEM24LC256_setData (uint16_t address, uint8_t* data)
{
    uint8_t dataSize = checkStringSize (data); //obtain the string lenght
    //write in the memory
    return i2c_writes (MEM24LC256_WRITE_ADDRESS, address, dataSize, data,
            SUBADRESS_SIZE);
}

int8_t MEM24LC256_getData (uint16_t address, uint8_t dataSize, uint8_t* data)
{
    //read from the memory
    return i2c_read (MEM24LC256_READ_ADDRESS, address, dataSize, data,
            SUBADRESS_SIZE);
}

