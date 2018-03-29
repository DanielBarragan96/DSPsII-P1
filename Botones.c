/*
 * Alarma.h


 *
 *  Created on: 25 oct. 2017
 *      Author: Sergio Chung, Francisco Avelar
 */

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "MK64F12.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "DataTypeDefinitions.h"
#include "Botones.h"

static BooleanType butonFlag=FALSE;


 void inicializacionBotones(PORT_Type *base, GPIO_Type *baseGPIO){
	 /*Se inicializa cada uno de los pines a utilizar por los botones*/
	 CLOCK_EnableClock(kCLOCK_PortC);

	 port_pin_config_t config_button =
	     	{ kPORT_PullDisable,
	     	  kPORT_SlowSlewRate,
	 		  kPORT_PassiveFilterDisable,
	     	  kPORT_OpenDrainDisable,
	 		  kPORT_LowDriveStrength,
	 		  kPORT_MuxAsGpio,
	     	  kPORT_UnlockRegister
	     	};

	 gpio_pin_config_t button_config_gpio =
	     	{
	     			kGPIO_DigitalInput, 1
	     	};

	 PORT_SetPinConfig(base, BIT5, &config_button);
	 PORT_SetPinConfig(base, BIT7, &config_button);
	 PORT_SetPinConfig(base, BIT0, &config_button);
	 GPIO_PinInit(baseGPIO, BIT5, &button_config_gpio);
	 GPIO_PinInit(baseGPIO, BIT7, &button_config_gpio);
	 GPIO_PinInit(baseGPIO, BIT0, &button_config_gpio);

	 /*Se habilitan sus interrupciones*/
	 NVIC_EnableIRQ(PORTC_IRQn);
	 NVIC_SetPriority(PORTC_IRQn, 5);

 }

 uint8 obtenerBoton(GPIO_Type *baseGPIO){

	 uint8 valorBoton;

		 Binario numero = {0};

		 /*Se leen los pines y se guardan en la estructura*/
	    numero.bitField.bit0 = GPIO_PinRead(baseGPIO, BIT0);
	    numero.bitField.bit1 = GPIO_PinRead(baseGPIO, BIT7);
	    numero.bitField.bit2 = GPIO_PinRead(baseGPIO, BIT5);

		uint32 x = numero.allBits;

		/*Dependiendo del botón, la funcion regresará 0,1,2,3,4 or 5*/
		if(BOTON_B0_MASK==x)
			valorBoton=0;
		else if(BOTON_B1_MASK==x){
			valorBoton=1;
		}
		else if(BOTON_B2_MASK==x){
			valorBoton=2;
				}
		return valorBoton;
 }


void clearflagB(){
	butonFlag = FALSE;
}

BooleanType getflagB(){
	return butonFlag;
}

void setflagB(){
	butonFlag = TRUE;
}

