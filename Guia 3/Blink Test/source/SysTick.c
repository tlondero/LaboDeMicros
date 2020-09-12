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

#include "MK64F12.h"

#include "hardware.h"

#define FCLK	__CORE_CLOCK__  // Hz
#define SYSTICK_ISR_FREQUENCY_HZ 12500000U
#define ISR_COUNT SYSTICK_ISR_FREQUENCY_HZ/100000U
#define MS2COUNTS(periodo) periodo/ISR_COUNT

//typedef void (*systick_callback_t)(void);
//systick_callback_t callback;

int flag_systick;

bool SysTick_Init (void (*funcallback)(void)){
		NVIC_EnableIRQ(SysTick_IRQn);
		bool res = false;
		if (funcallback != NULL) {
			SysTick->CTRL = 0x00; 					//Enable sysT interrupt
			SysTick->LOAD = SYSTICK_ISR_FREQUENCY_HZ - 1; //125 ms @1Mhz
			SysTick->VAL=0x00;
			SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk| SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
			callbacks = funcallback;

			//Convert period from milisenconds to counts
			while(callbacks[i] != NULL){
				callbacks[i].periodo = MS2COUNTS(callbacks[i].periodo);
			}

			res = true;
		}
	return res;

}

static uint32_t count_veces = 0;
__ISR__ SysTick_Handler(void){
	int i = 0;
	while(callbacks[i] != NULL)
	{
		if(count_veces%callbacks[i].periodo){

		}

	}


	if (count_veces == 8)
		{
		callback();
		count_veces = 0;
		}
	else{
		count_veces++;
	}


}

















