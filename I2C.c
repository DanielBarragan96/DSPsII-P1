/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    I2C.c
 * @brief   I2C module.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_i2c.h"

#include "MEM24LC256.h"
#include "I2C.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"

//Bits used for the I2C
#define BIT2 2
#define BIT3 3

//Flag to check the I2C status
volatile bool g_MasterCompletionFlag = false;
//I2C variables
uint8_t data_buffer = 0x01;
uint8_t read_data;

//I2C handle
i2c_master_handle_t g_m_handle;
//indicate if the I2C isn't responding
volatile bool g_i2c_nw = false;
//mutex to protect the I2C
SemaphoreHandle_t mutex_i2c;
//timer to indicate if the I2C isn't working
TimerHandle_t g_timer;

void TimerCallback (TimerHandle_t timeIn)
{ //when the I2C isn't working
    g_i2c_nw = true;
}

static void i2c_release_bus_delay (void)
{
    uint32_t i = 0;
    for (i = 0; i < 100; i++)
    {
        __NOP();
    }
}

void i2c_ReleaseBus ()
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config =
    { 0 };

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    PORT_SetPinConfig (PORTB, 19, &i2c_pin_config);
    PORT_SetPinConfig (PORTB, 18, &i2c_pin_config);

    GPIO_PinInit (GPIOB, 19, &pin_config);
    GPIO_PinInit (GPIOB, 18, &pin_config);

    GPIO_PinWrite (GPIOB, 18, 0U);
    i2c_release_bus_delay ();

    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite (GPIOB, 19, 0U);
        i2c_release_bus_delay ();

        GPIO_PinWrite (GPIOB, 18, 1U);
        i2c_release_bus_delay ();

        GPIO_PinWrite (GPIOB, 19, 1U);
        i2c_release_bus_delay ();
        i2c_release_bus_delay ();
    }

    GPIO_PinWrite (GPIOB, 19, 0U);
    i2c_release_bus_delay ();

    GPIO_PinWrite (GPIOB, 18, 0U);
    i2c_release_bus_delay ();

    GPIO_PinWrite (GPIOB, 19, 1U);
    i2c_release_bus_delay ();

    GPIO_PinWrite (GPIOB, 18, 1U);
    i2c_release_bus_delay ();
}

static void i2c_master_callback (I2C_Type *base, i2c_master_handle_t *handle,
        status_t status, void * userData)
{ //when there is an interruption
    if (status == kStatus_Success)
    { //if the I2C worked
        g_MasterCompletionFlag = true; //indicate end of waiting
    }
}

/*
 * @brief   Application entry point.
 */
int8_t init_i2c ()
{
    /* Init FSL debug console. */
    i2c_ReleaseBus ();
    //initialize I2C for using the I2C
    CLOCK_EnableClock (kCLOCK_I2c0);

    //I2C configuration
    port_pin_config_t config_i2c =
    { kPORT_PullUp, kPORT_FastSlewRate, kPORT_PassiveFilterDisable,
            kPORT_OpenDrainEnable, kPORT_LowDriveStrength, kPORT_MuxAlt2,
            kPORT_UnlockRegister, };
    //I2C Pin configuration
    PORT_SetPinConfig (PORTB, BIT2, &config_i2c); //SCL
    PORT_SetPinConfig (PORTB, BIT3, &config_i2c); //SDA

    // Get default configuration for master.
    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig (&masterConfig);
    //initialize I2C
    I2C_MasterInit (I2C0, &masterConfig, CLOCK_GetFreq (kCLOCK_BusClk));
    //create I2C handle
    I2C_MasterTransferCreateHandle (I2C0, &g_m_handle, i2c_master_callback,
    NULL);
    //enable I2C interrupts
    I2C_Enable (I2C0, true);
    I2C_EnableInterrupts (I2C0, kI2C_GlobalInterruptEnable);
    //I2C nw configuration
    const TickType_t g_xTimerPeriod = pdMS_TO_TICKS(1000); //periodo a interrumpir
    //Interrupt I2C nw
    const char *pcTimerName = "Timer";    //nombre
    const UBaseType_t uxAutoReload = pdFALSE;    //si se hace auto reload
    void * const pvTimerID = NULL; //handle de las tareas, regresa un valor para identificar la tarea
    TimerCallbackFunction_t pxCallbackFunction = TimerCallback; //callback function

    //se crea el timer, es global, tipo TimerHandle_t
    g_timer = xTimerCreate (pcTimerName, g_xTimerPeriod, uxAutoReload,
            pvTimerID, pxCallbackFunction);

    //create mutex
    mutex_i2c = xSemaphoreCreateMutex();
    //start mutex in signilized status
    xSemaphoreGive(mutex_i2c);

    return 0;
}

int8_t i2c_read (uint8_t slaveAdress, uint8_t subaddress, uint8_t dataSize,
        uint8_t* bufferOut, uint8_t subaddressSize)
{
    //Master transfer that can be modified
    i2c_master_transfer_t masterXfer;
    masterXfer.slaveAddress = slaveAdress;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = subaddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data = bufferOut;
    masterXfer.dataSize = dataSize;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    //tkae the I2C mutex
    xSemaphoreTake(mutex_i2c, portMAX_DELAY);
    // Get default configuration for slave.
    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig (&masterConfig);
    //initialize I2C
    I2C_MasterInit (I2C0, &masterConfig, CLOCK_GetFreq (kCLOCK_BusClk));
    //start timer to verify I2C working
    xTimerStart(g_timer, portMAX_DELAY);
    //initialize transmit
    I2C_MasterTransferNonBlocking (I2C0, &g_m_handle, &masterXfer);
    while (!g_MasterCompletionFlag && !g_i2c_nw)
    { //while the I2C hasn't finishi¿ed or the timer is still counting
        vTaskDelay (pdMS_TO_TICKS(I2C_TRANS_DELAY));
    }
    //stop I2C and timer
    xTimerStop(g_timer, portMAX_DELAY);
    I2C_MasterStop (I2C0);
    g_MasterCompletionFlag = false;
    //give I2C mutex
    xSemaphoreGive(mutex_i2c);
    //handle if the I2C didn't worked as desired
    if (g_i2c_nw)
    {
        g_i2c_nw = false;
        return -1;    //Error
    }
    return 0;
}

int8_t i2c_writes (uint8_t slaveAdress, uint8_t subaddress, uint8_t dataSize,
        uint8_t* buffer, uint8_t subaddressSize)
{

    //Master transfer that can be modified
    i2c_master_transfer_t masterXfer;
    masterXfer.slaveAddress = slaveAdress;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = subaddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data = buffer;
    masterXfer.dataSize = dataSize;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    //tkae the I2C mutex
    xSemaphoreTake(mutex_i2c, portMAX_DELAY);
    // Get default configuration for master.
    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig (&masterConfig);
    //initialize I2C
    I2C_MasterInit (I2C0, &masterConfig, CLOCK_GetFreq (kCLOCK_BusClk));
    //start timer to verify I2C working
    xTimerStart(g_timer, portMAX_DELAY);
    //initialize transmit
    I2C_MasterTransferNonBlocking (I2C0, &g_m_handle, &masterXfer);
    while (!g_MasterCompletionFlag && !g_i2c_nw)
    {    //while the I2C hasn't finishi¿ed or the timer is still counting
        vTaskDelay (pdMS_TO_TICKS(I2C_TRANS_DELAY));
    }
    //stop I2C and timer
    xTimerStop(g_timer, portMAX_DELAY);
    I2C_MasterStop (I2C0);
    g_MasterCompletionFlag = false;
    //give I2C mutex
    xSemaphoreGive(mutex_i2c);
    //handle if the I2C didn't worked as desired
    if (g_i2c_nw)
    {
        g_i2c_nw = false;
        return -1;
    }
    return 0;
}
