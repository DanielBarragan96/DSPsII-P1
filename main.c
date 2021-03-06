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
 * @file    Practica01.c
 * @brief   Application entry point.
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

#include "SPI.h"
#include "DataTypeDefinitions.h"
#include "PantallaPC.h"
#include "UART_BT.h"
#include "UART_TeraTerm.h"
#include "LCDNokia5110.h"
#include "MEM24LC256.h"
#include "PCF8563.h"
#include "queue.h"
#include "init.h"
#include "timers.h"

/*Valor que retorn el TeraTerm al no presionar ninguna tecla */
#define nullValue 208

/*
 * Utilizando un arreglo de funciones el cual contiene la funcion correspondiente a los
 * cinco menus disponibles, mediante su apuntador a funciones ingresamos a cada manu segun
 * sea el valor ingresado ya se por medio del TeraTerm o BT(BlueTooth)
 */
void menus_task (void* args)
{
    init_clk ();
    /*Va a contener el valor de las UARTs a utilizar (UART0, UART4) */
    UART_Type * uart = (UART_Type *) args;

    void (*Pantallas[9]) (UART_Type *) =
    {   LeerM, EscribirM, Ehora,
        Efecha, Fhora, Lhora, Lfecha, Comunicacion, Eco
    };

    /*Imprimimos el menu inicial en las dos terminales */
    MenuInicial (uart);

    while (1)
    {
        /*Se obtiene el valor del Menu seleccionado */
        uint8_t x = escogerMenu (uart);

        if ((0 != x) && (nullValue != x))
        {
            Pantallas[x - 1] (uart);
            MenuInicial (uart); //Salimos del menu seleccionado y volvemos al inicial
        }
        vTaskDelay (pdMS_TO_TICKS(10));
    }
}

int main (void)
{

    /* Init board hardware. */
    BOARD_InitBootClocks ();
    BOARD_InitBootPeripherals ();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole ();
    /*Creamos tareas e inicializamos el scheduler */
    initTasks ();

    return 0;
}
