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
uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};

dspi_master_handle_t g_m_handle;
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

void SPI_init(){
	dspi_master_config_t masterConfig;
	BOARD_InitPins_SPI();
	DSPI_MasterGetDefaultConfig(&masterConfig);

	uint32_t srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
    DSPI_MasterTransferCreateHandle(SPI0, &g_m_handle, DSPI_MasterUserCallback, NULL);
}

void SPI_send(SPI_Type *base, uint8_t* string){
	 dspi_transfer_t masterXfer;
	  /* Start master transfer, send data to slave */
	        isTransferCompleted = false;
	        masterXfer.txData = string;
	        masterXfer.rxData = NULL;
	        masterXfer.dataSize = sizeof(string);
	        masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;
	        DSPI_MasterTransferNonBlocking(SPI0, &g_m_handle, &masterXfer);

	        /* Wait transfer complete */
	        while (!isTransferCompleted)
	        {
	        }
}

void SPI_sendOneByte (SPI_Type *base, uint8_t Data){
	base->PUSHR = Data;
	while(0 == (base->SR & SPI_SR_TCF_MASK));
	base->SR |= SPI_SR_TCF_MASK;
}

