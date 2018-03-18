/*
 * MEM24LC256.c
 *  Source file of the 24LC256
 *  Created on: 07/11/2017
 *      Author: J. Alejandro Ávila & Daniel Barragán
 */

#include "MEM24LC256.h"
#include "I2C.h"

uint8_t checkStringSize (uint8_t* string)
{
    uint8_t size = 0;
    while (*string)
    {
        size++;
        string++;
    }
    return size;
}

int8_t MEM24LC256_setData (uint16_t address, uint8_t* data)
{
    uint8_t dataSize = checkStringSize (data);
    return i2c_writes (MEM24LC256_WRITE_ADDRESS, address, dataSize, data);
}

int8_t MEM24LC256_getData (uint16_t address, uint8_t dataSize, uint8_t* data)
{
    return i2c_read (MEM24LC256_READ_ADDRESS, address, dataSize, data);
}
