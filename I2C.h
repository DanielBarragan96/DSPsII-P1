/*
 * I2C.h
 *
 *  Created on: Feb 7, 2018
 *      Author: lei-n
 */

#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/*!
 \brief	 Start the necessary peripherals to use the I2C and read the
 accelerometer.
 \return TRUE if there was no problem
 */
int8_t init_i2c ();
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/*!
 \brief	 This function reads the status of the accelerometer, and also
 manages the status of the LED sequence
 \param[in] subaddress devicce adress
 \param[in] dataSize
 \return TRUE if there was no problem
 */
int8_t i2c_read (uint8_t slaveAdress, uint32_t subaddress, uint8_t dataSize,
        uint8_t* bufferOut);
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/*!
 \brief  This function reads the status of the accelerometer, and also
 manages the status of the LED sequence
 \param[in] subaddress devicce adress
 \param[in] dataSize
 \param[in] buffer[dataSize] cointains the information to write
 \return TRUE if there was no problem
 */
int8_t i2c_writes (uint8_t slaveAdress, uint32_t subaddress, uint8_t dataSize,
        uint8_t* buffer);

#endif /* I2C_H_ */
