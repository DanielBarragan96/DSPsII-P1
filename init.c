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


void initMain(){
	//Change the Kinetis clock speed
	 int mcg_clk_hz;
		   unsigned char modeMCG = 0;


		#ifndef PLL_DIRECT_INIT
		   mcg_clk_hz = fei_fbi(SLOW_IRC_FREQ,SLOW_IRC);// 64 Hz ---> 32768
		   mcg_clk_hz = fbi_fbe(CLK_FREQ_HZ,LOW_POWER,EXTERNAL_CLOCK); // 97.656KHz ---> 50000000
		   mcg_clk_hz = fbe_pbe(CLK_FREQ_HZ,PLL0_PRDIV,PLL0_VDIV);	// 97.656KHz ---> 50000000 and PLL is configured to generate 60000000
		   mcg_clk_hz =  pbe_pee(CLK_FREQ_HZ);// 117.18 KHz ---> 60000000
		#else
		      mcg_clk_hz = pll_init(CLK_FREQ_HZ, LOW_POWER, EXTERNAL_CLOCK, PLL0_PRDIV, PLL0_VDIV, PLL_ENABLE);
		#endif

		   modeMCG = what_mcg_mode();

//		   I2C_init(I2C_0, 60000000, 100000);

		   i2c_master_config_t masterConfig;
		   i2c_slave_config_t slaveConfig;

		   CLOCK_EnableClock(kCLOCK_PortB);
//            /**configures both ptc1 and ptb18 in alt modes 3,4 respectively*/
//            GPIO_pinControlRegisterType pinControlRegisterMux2 = GPIO_MUX2;
//            GPIO_pinControlRegister(GPIO_B,BIT2,&pinControlRegisterMux2); //SCL
//            GPIO_pinControlRegister(GPIO_B,BIT3,&pinControlRegisterMux2); //SDA

           // Get default configuration for master.
            I2C_MasterGetDefaultConfig(&masterConfig);
            I2C_MasterInit(I2C0, &masterConfig, I2C_MASTER_CLK_FREQ);
            I2C_SlaveGetDefaultConfig(&slaveConfig);
            slaveConfig.addressingMode = kI2C_RangeMatch;

            slaveConfig.slaveAddress = MEM24LC256_WRITE_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);
            slaveConfig.slaveAddress = MEM24LC256_READ_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);


            slaveConfig.slaveAddress = PCF8563_WRITE_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);
            slaveConfig.slaveAddress = PCF8563_READ_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);
            slaveConfig.slaveAddress = PCF8563_SECONDS_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);
            slaveConfig.slaveAddress = PCF8563_MINUTES_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);
            slaveConfig.slaveAddress = PCF8563_HOURS_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);
            slaveConfig.slaveAddress = PCF8563_DAYS_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);
            slaveConfig.slaveAddress = PCF8563_MONTHS_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);
            slaveConfig.slaveAddress = PCF8563_YEARS_ADDRESS;
            I2C_SlaveInit(I2C0, &slaveConfig,I2C_CLK);

           I2C_Enable(I2C0, true);
           I2C_EnableInterrupts(I2C0, kI2C_GlobalInterruptEnable);

//       uint32_t status = kI2C_StopDetectFlag;
//       I2C0->D = 6;
//	   // Send start and slave address.
//	   I2C_MasterStart(I2C0, MEM24LC256_WRITE_ADDRESS, kI2C_Write);
//	   // Wait address sent out.
//	   while(!((status = I2C_MasterGetStatusFlags(I2C0)) & kI2C_IntPendingFlag))
//	   { }
//	   I2C_MasterClearStatusFlags(I2C0, kI2C_StopDetectFlag);
//	   I2C_MasterStart(I2C0, MEM24LC256_READ_ADDRESS, kI2C_Read);
//	   while(!((status = I2C_SlaveGetStatusFlags(I2C0)) & kI2C_IntPendingFlag))
//	   { }
//	   //TODO donde leo el valor recibido
//	   uint8_t read = I2C0->D;
//
//	   //set initial values for the clock
//	   PCF8563_setSeconds(0x50);
//	   PCF8563_setMinutes(0x59);
//	   PCF8563_setHours(0x21);
//	   PCF8563_setYears(0x98);
//	   PCF8563_setMonths(0x11);
//	   PCF8563_setDays(0x30);



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
