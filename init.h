/*
 * init.h
 *
 *  Created on: 16/09/2017
 *      Author: Daniel Barragán
 */

#ifndef INIT_H_
#define INIT_H_

#include "DataTypeDefinitions.h"

/** Constant that enable the clock of the DAC*/
#define DAC_SIM 0x1000U

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function initializes the necesary componentes of the Kinetis to start the program

 	 \return void
 */
void initMain();


#endif /* INIT_H_ */
