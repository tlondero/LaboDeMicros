/***************************************************************************//**
 @file     main.c
 @brief    FW main
 @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"

void I2CInit_tb(void);
void I2CRun_tb(void);

int main(void) {
	hw_Init();
	hw_DisableInterrupts();
	I2CInit_tb(); /* Program-specific setup */
	hw_EnableInterrupts();

	__FOREVER__
		I2CRun_tb(); /* Program-specific loop  */
}
