/*
 * SysTick.c
 *
 *  Created on: Sep 3, 2020
 *      Author: Guido
 */


/**
 * @brief Initialize SysTic driver
 * @param funcallback Function to be call every SysTick
 * @return Initialization and registration succeed
 */
#include "SysTick.h"
#include <stddef.h>
#include "core_cm4.h"

#define FCLK	1000000000U // Hz
bool SysTick_Init (void (*funcallback)(void)){
		NVIC_EnableIRQ(SysTick_IRQn);
		bool res = false;
		if (funcallback != NULL) {
			SysTick->CTRL = 0x00; 					//Enable sysT interrupt
			SysTick->LOAD = FCLK/SYSTICK_ISR_FREQUENCY_HZ - 1; // load value
			SysTick->VAL=0x00;
			SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk| SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
			res = true;
		}
	return res;

}
