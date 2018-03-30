/*
 * SPI.h
 *
 *  Created on: Mar 24, 2018
 *      Author: Sergio
 */

#ifndef SPI_H_
#define SPI_H_

#include "fsl_dspi.h"

void SPI_init();
void SPI_send(uint8_t* string);
void SPI_sendOneByte (uint8_t Data);

#endif /* SPI_H_ */
