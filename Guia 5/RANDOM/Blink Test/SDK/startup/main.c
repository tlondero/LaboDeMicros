/***************************************************************************//**
 @file     main.c
 @brief    FW main
 @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"

#include "header/random.h"

void I2CInit_tb(void);
void I2CRun_tb(void);

int main(void) {
	hw_Init();
	hw_EnableInterrupts();
	RandInit();

	while (1) {

		if (RandCheck()) {

			uint32_t tuvi = RandGet();
			tuvi++;
		}

	}
}
