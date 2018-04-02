/*
 * SPI.c
 *
 *  Created on: Mar 24, 2018
 *      Author: Sergio
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_dspi.h"
#include "board.h"
#include "SPI.h"

#include "pin_mux.h"
#include "clock_config.h"

#define TRANSFER_SIZE 64U
#define EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER kDSPI_MasterPcs0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* DSPI user callback */
void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
dspi_master_handle_t g_m_handleSPI;
volatile bool isTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        __NOP();
    }

    isTransferCompleted = true;
}

/* Inicializamos SPI
 * Creamos su handle
*/
void SPI_init(){
	dspi_master_config_t masterConfig;
	DSPI_MasterGetDefaultConfig(&masterConfig);

	DSPI_MasterInit(SPI0, &masterConfig, CLOCK_GetBusClkFreq());
    DSPI_MasterTransferCreateHandle(SPI0, &g_m_handleSPI, DSPI_MasterUserCallback, NULL);
}

/*Funcion para enviar datos*/
void SPI_send(uint8_t* string){
	 dspi_transfer_t masterXfer;
  /* Start master transfer, send data to slave */
	 isTransferCompleted = false;
	 masterXfer.txData = string;
	 masterXfer.rxData = NULL;
	 masterXfer.dataSize = sizeof(string);
	 masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;
	 DSPI_MasterTransferNonBlocking(SPI0, &g_m_handleSPI, &masterXfer);

	 /* Wait transfer complete */
	 while (!isTransferCompleted)
	 {
	 }
}

/*Funcion para enviar de byte en byte */
void SPI_sendOneByte (uint8_t Data){
	 uint8_t sendData[1];
	 sendData[0] = Data;
	 dspi_transfer_t masterXfer;
	 /* Start master transfer, send data to slave */
		 isTransferCompleted = false;
	 masterXfer.txData = (uint8_t*)sendData;
	 masterXfer.rxData = NULL;
	 masterXfer.dataSize = sizeof(sendData);
	 masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;

	 DSPI_MasterTransferNonBlocking(SPI0, &g_m_handleSPI, &masterXfer);
	 /* Wait transfer complete */
	         while (!isTransferCompleted)
	         {
	         }
	 DSPI_StopTransfer(SPI0);
}

