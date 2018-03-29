/*
 * init.h
 *
 *  Created on: 17/09/2017
 *      Author: Daniel Barragán
 */

#include "MK64F12.h"
#include "init.h"
#include "MEM24LC256.h"
#include "PCF8563.h"
#include "fsl_clock.h"
#include "fsl_i2c.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

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
    CLOCK_EnableClock (kCLOCK_PortB);
    init_i2c ();

//	TODO init UART
//	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
//	UART_init (UART_0,  60000000, BD_115200);
//	/**Enables the UART 0 interrupt*/
//	UART0_interruptEnable(UART_0);
//   /**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
//   CLOCK_EnableClock (kCLOCK_PortB);
//
//  /**Configures the pin control register of pin16 in PortB as UART RX*/
//  PORTB->PCR[16] = PORT_PCR_MUX(3);
//  /**Configures the pin control register of pin16 in PortB as UART TX*/
//  PORTB->PCR[17] = PORT_PCR_MUX(3);

//  TODO init NVIC
//	/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
//	NVIC_setBASEPRI_threshold(PRIORITY_5);
//	/**Enables the UART 0 interrupt in the NVIC*/
//	NVIC_enableInterruptAndPriotity(UART0_IRQ, PRIORITY_3);
//	/**Enables interrupts*/
//	EnableInterrupts;

//	TODO init TeraTerm
//	/**Print menu by the Serial output*/
//	printTTMainMenu();

}
