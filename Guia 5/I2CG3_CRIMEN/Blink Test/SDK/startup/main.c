/***************************************************************************//**
 @file     main.c
 @brief    FW main
 @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"

void I2CInit_tb(void);
void I2CRun_tb(void);
void I2CRunDeltas_tb(void);
int main(void) {
	hw_Init();
	hw_EnableInterrupts();
	I2CInit_tb(); /* Program-specific setup */
	//hw_EnableInterrupts();

	__FOREVER__
	I2CRunDeltas_tb(); /* Program-specific loop  */
}
