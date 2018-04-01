/*
 * I2C.h
 *
 *  Created on: 14/3/2018
 *      Author: Daniel Barragan
 */

#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"

#define I2C_TRANS_DELAY 10
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/*!
 \brief	 Start the necessary peripherals to use the I2C.
 \return TRUE if there was no problem
 */
int8_t init_i2c ();
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/*!
 \brief	 This function uses the I2C to read from a slave.
 \param[in] slaveAdress the device adress
 \param[in] subaddress adress inside the devuice
 \param[in] dataSize how many bytes to read
 \param[in] bufferOut where the read info is stored
 \param[in] the device subadress size
 \return TRUE if there was no problem, or -1 if there was an error with the i2c
 */
int8_t i2c_read (uint8_t slaveAdress, uint8_t subaddress, uint8_t dataSize,
        uint8_t* bufferOut, uint8_t subaddressSize);
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/*!
 \brief  This function uses the I2C to write to a slave.
 \param[in] slaveAdress the device adress
 \param[in] subaddress adress inside the devuice
 \param[in] dataSize how many bytes to read
 \param[in] bufferOut where the read info is stored
 \param[in] the device subadress size
 \return TRUE if there was no problem, or -1 if there was an error with the i2c
 */
int8_t i2c_writes (uint8_t slaveAdress, uint8_t subaddress, uint8_t dataSize,
        uint8_t* buffer, uint8_t subaddressSize);

 #endif /* I2C_H_ */
