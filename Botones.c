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

//for interruptions
#define PORTC_IRQ_BIT0_MASK (0)
#define PORTC_IRQ_BIT5_MASK (5)
#define PORTC_IRQ_BIT7_MASK (7)
#define PORTC_IRQ (32)

static bool butonFlag = false;

static volatile Butons valorBoton;

void PORTC_IRQHandler (void)
{
    Binario numero =
    { 0 };

    /*Se leen los pines y se guardan en la estructura*/
    numero.bitField.bit0 = GPIO_PinRead (GPIOC, BIT5);//B0
    numero.bitField.bit1 = GPIO_PinRead (GPIOC, BIT7);//B1
    numero.bitField.bit2 = GPIO_PinRead (GPIOC, BIT0);//B2

    uint32 x = numero.allBits;

    uint32_t bits_irq = PORT_GetPinsInterruptFlags(PORTC);

    /*Dependiendo del botón, la funcion regresará 0,1,2,3,4 or 5*/
    if (BOTON_B0_MASK & x)
    {
        valorBoton = BUTTON_0;
        PORT_ClearPinsInterruptFlags (PORTC, 1 << (BIT5)); //clear irq
    }
    else if (BOTON_B1_MASK & x)
    {
        valorBoton = BUTTON_1;
        PORT_ClearPinsInterruptFlags (PORTC, 1 << (BIT7)); //clear irq
    }
    else if (BOTON_B2_MASK & x)
    {
        valorBoton = BUTTON_2;
        PORT_ClearPinsInterruptFlags (PORTC, 1 << (BIT0)); //clear irq
    }
    else
    {
        valorBoton = NO_BUTTON;
    }
    bits_irq = PORT_GetPinsInterruptFlags(PORTC);
    setflagB ();
}

void inicializacionBotones ()
{
    /*Se inicializa cada uno de los pines a utilizar por los botones*/
    CLOCK_EnableClock (kCLOCK_PortC);

    port_pin_config_t config_button =
    { kPORT_PullDown, kPORT_FastSlewRate, kPORT_PassiveFilterDisable,
            kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
            kPORT_UnlockRegister };

    gpio_pin_config_t button_config_gpio =
    { kGPIO_DigitalInput, 1 };

    PORT_SetPinConfig (PORTC, BIT5, &config_button);
    PORT_SetPinConfig (PORTC, BIT7, &config_button);
    PORT_SetPinConfig (PORTC, BIT0, &config_button);
    GPIO_PinInit (GPIOC, BIT5, &button_config_gpio);
    GPIO_PinInit (GPIOC, BIT7, &button_config_gpio);
    GPIO_PinInit (GPIOC, BIT0, &button_config_gpio);

    /* Init input switch GPIO. */
    PORT_SetPinInterruptConfig(PORTC, BIT5, kPORT_InterruptRisingEdge);
    PORT_SetPinInterruptConfig(PORTC, BIT7, kPORT_InterruptRisingEdge);
    PORT_SetPinInterruptConfig(PORTC, BIT0, kPORT_InterruptRisingEdge);
    /*Se habilitan sus interrupciones*/
    NVIC_EnableIRQ (PORTC_IRQn);
    NVIC_SetPriority (PORTC_IRQn, 2);
}

Butons obtenerBoton ()
{
    Butons x = valorBoton;
    valorBoton = NO_BUTTON;
    clearflagB ();
    return x;
}

void clearflagB ()
{
    butonFlag = false;
}

BooleanType getflagB ()
{
    return butonFlag;
}

void setflagB ()
{
    butonFlag = true;
}

