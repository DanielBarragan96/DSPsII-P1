/*
 * init.h
 *
 *  Created on: 17/09/2017
 *      Author: Daniel Barragán
 */

#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "init.h"
#include "MCG.h"
#include "MEM24LC256.h"
#include "PCF8563.h"
#include "fsl_clock.h"
#include "fsl_i2c.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "TeraTerm_Task_UART.h"
#include "BT_Task_UART.h"
#include "I2C.h"

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

void initMain ()
{
    //Change the Kinetis clock speed
    int mcg_clk_hz;
    unsigned char modeMCG = 0;

#ifndef PLL_DIRECT_INIT
    mcg_clk_hz = fei_fbi (SLOW_IRC_FREQ, SLOW_IRC); // 64 Hz ---> 32768
    mcg_clk_hz = fbi_fbe (CLK_FREQ_HZ, LOW_POWER, EXTERNAL_CLOCK); // 97.656KHz ---> 50000000
    mcg_clk_hz = fbe_pbe (CLK_FREQ_HZ, PLL0_PRDIV, PLL0_VDIV);// 97.656KHz ---> 50000000 and PLL is configured to generate 60000000
    mcg_clk_hz = pbe_pee (CLK_FREQ_HZ);	// 117.18 KHz ---> 60000000
#else
    mcg_clk_hz = pll_init(CLK_FREQ_HZ, LOW_POWER, EXTERNAL_CLOCK, PLL0_PRDIV, PLL0_VDIV, PLL_ENABLE);
#endif

    modeMCG = what_mcg_mode ();
    init_i2c();
    uart_TeraTerm_init();
    uart_BT_init();

    CLOCK_EnableClock (kCLOCK_PortB);
}
