/***************************************************************************//**
 @file     main.c
 @brief    FW main
 @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"

void appInit(void);
void appRun(void);

int main(void) {
	hw_Init();
	hw_EnableInterrupts();
	appInit(); /* Program-specific setup */

	__FOREVER__
	I2CRunDeltas_tb(); /* Program-specific loop  */
}
