/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v1.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/*******************************************************************************
 * Included files
 ******************************************************************************/
#include "peripherals.h"
#include "UART_BT.h"
#include "UART_TeraTerm.h"
#include "SPI.h"
#include "LCDNokia5110.h"

/*******************************************************************************
 * BOARD_InitBootPeripherals function
 ******************************************************************************/
void BOARD_InitBootPeripherals(void)
{
	BOARD_InitPins();
	uart_TeraTerm_init();
	uart_BT_init();
	SPI_init();
	LCDNokia_init();
}
