/*
 * MEM24LC256.h
 *	Header file of the 24LC256
 *  Created on: 07/11/2017
 *      Author: Alex Ávila y Daniel Barragan
 */

#ifndef MEM24LC256_H_
#define MEM24LC256_H_

#include "stdint.h"

#define MEM24LC256_SLAVE_ADDRESS 0x50 // Slave address with chip select bits on ground and w/r bit in 0

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief	This function uses the I2C to send the write instruction to be able to write the address
 high and low of the memory where we are storing data and then writes the data into the address.
 \param[in]  uint16_t (HEX) address where we get the data from (gets separated into high and low).
 \param[in]  uint8_t (HEX) data that is gonna be stored in the data address we get from the param.
 \return void
 */
int8_t MEM24LC256_setData (uint16_t address, uint8_t* data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief	This function uses the I2C to send the write instruction to be able to write the address
 high and low of the memory where we are storing data and then switches into read mode and receive
 to be able to get the data from the address we are sending.
 \param[in]  uint16_t address where we get the data from (gets separated into high and low).
 \return the data from the memory address we give as a param.
 */
int8_t MEM24LC256_getData (uint16_t address, uint8_t dataSize, uint8_t* data);

#endif /* MEM24LC256_H_ */
