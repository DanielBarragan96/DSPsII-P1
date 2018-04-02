/*
 * SPI.h
 *
 *  Created on: Mar 24, 2018
 *      Author: Sergio
 */

#ifndef SPI_H_
#define SPI_H_

#include "fsl_dspi.h"

/* Init SPI */
void SPI_init();

/* SPI send data */
void SPI_send(uint8_t* string);

/* SPI send one byte */
void SPI_sendOneByte (uint8_t Data);

#endif /* SPI_H_ */
