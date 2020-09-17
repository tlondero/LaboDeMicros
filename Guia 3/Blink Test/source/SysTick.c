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
#include "header/SysTick.h"
#include <stddef.h>
#include <stdint.h>
#include "MK64F12.h"

#define SYSTICK_ISR_FREQUENCY_HZ 12500000U
#define ISR_COUNT (SYSTICK_ISR_FREQUENCY_HZ/100000U)
#define MS2COUNTS(periodo) periodo/ISR_COUNT


int flag_systick;
static CallbackInterrupt *irqs_callback_systick[2];

bool SysTick_Init (CallbackInterrupt *irqs_callback[], int cantidad){
		NVIC_EnableIRQ(SysTick_IRQn);
		bool res = false;
		if (irqs_callback != NULL) {

			int i = 0;
			//Convert period from milisenconds to counts
			for (i=0; i<cantidad; i++)
			{
				irqs_callback_systick[i] = irqs_callback[i];
				int a = (irqs_callback_systick[i]->periodo);
					a = MS2COUNTS(a);
				irqs_callback_systick[i]->periodo=MS2COUNTS(irqs_callback_systick[i]->periodo);
			}
			SysTick->CTRL = 0x00; 					//Enable sysT interrupt
			SysTick->LOAD = SYSTICK_ISR_FREQUENCY_HZ - 1; //125 ms @1Mhz
			SysTick->VAL=0x00;
			SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk| SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;


			res = true;
		}
	return res;

}


static int index = 0;

__ISR__ SysTick_Handler(void){
	static uint64_t timeElapsed = 1;
		if (timeElapsed == UINT64_MAX)
			timeElapsed = 1;
		else
			timeElapsed ++;
	index = 0;
	for (index = 0; index < 2; index++){
		if( (timeElapsed%irqs_callback_systick[index]->periodo) == 0){
			irqs_callback_systick[index]->stick_callback();
		}
	}

}


















