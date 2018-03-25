/*
 * SPI.h
 *
 *  Created on: Mar 24, 2018
 *      Author: Sergio
 */

#ifndef SPI_H_
#define SPI_H_

#include "fsl_dspi.h"

void SPI_sendOneByte (SPI_Type *base, uint8_t Data);

#endif /* SPI_H_ */
