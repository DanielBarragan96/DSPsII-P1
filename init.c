/*
 * init.h
 *
 *  Created on: 17/09/2017
 *      Author: Daniel Barragán
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "DataTypeDefinitions.h"
#include "SPI.h"
#include "DataTypeDefinitions.h"
#include "PantallaPC.h"
#include "UART_BT.h"
#include "UART_TeraTerm.h"
#include "LCDNokia5110.h"
#include "MEM24LC256.h"
#include "PCF8563.h"



/* I2C source clock */
#define I2C_MASTER_CLK_SRC I2C0_CLK_SRC
#define I2C_MASTER_CLK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)
#define EXAMPLE_I2C_MASTER_BASEADDR I2C0

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_BAUDRATE 100000U
#define I2C_DATA_LENGTH 33U

#define CLK_FREQ_HZ 50000000  /* CLKIN0 frequency */
#define SLOW_IRC_FREQ 32768	/*This is the approximate value for the slow irc*/
#define FAST_IRC_FREQ 4000000 /*This is the approximate value for the fast irc*/
#define EXTERNAL_CLOCK 0 /*It defines an external clock*/
#define PLL_ENABLE 1 /**PLL is enabled*/
#define PLL_DISABLE 0 /**PLL is disabled*/
#define CRYSTAL_OSC 1  /*It defines an crystal oscillator*/
#define LOW_POWER 0     /* Set the oscillator for low power mode */
#define SLOW_IRC 0 		/* Set the slow IRC */
#define CLK0_TYPE 0     /* Crystal or canned oscillator clock input */
#define PLL0_PRDIV 25    /* PLL predivider value */
#define PLL0_VDIV 30    /* PLL multiplier value*/

#define BUFFER_SIZE 8
#define I2C_CLK 12000000U

void menus_task(void* args);

void initTasks ()
{
	xTaskCreate(menus_task, "Menus PC", 110, (void*) UART0, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(menus_task, "Menus BT", 110, (void*) UART4, configMAX_PRIORITIES-1, NULL);

	vTaskStartScheduler();

}



