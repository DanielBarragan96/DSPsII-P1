/*
 * MEM24LC256.c
 *  Source file of the 24LC256
 *  Created on: 07/11/2017
 *      Author: J. Alejandro Ávila & Daniel Barragán
 */

#include "MEM24LC256.h"
#include "I2C.h"

void MEM24LC256_setData (uint16 address, uint8_t dataSize, uint8* data)
{
    i2c_writes (MEM24LC256_WRITE_ADDRESS, address, dataSize, data);
}

uint8* MEM24LC256_getData (uint16 address, uint8 bytes, uint8* data)
{
    i2c_read (MEM24LC256_READ_ADDRESS, address, bytes, data);
    return data;
}
