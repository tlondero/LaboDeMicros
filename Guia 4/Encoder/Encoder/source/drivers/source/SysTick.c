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

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "../headers/SysTick.h"
#include <stddef.h>
#include <stdint.h>
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SYSTICK_ISR_FREQUENCY_HZ 1000U//12500000U

#define SYSTICK_LOAD_INIT ((__CORE_CLOCK__ / SYSTICK_ISR_FREQUENCY_HZ) - 1U)
#if SYSTICK_LOAD_INIT > (1 << 24)
#error Overflow de SysTick! Ajustar  __CORE_CLOCK__ y SYSTICK_ISR_FREQUENCY_HZ!
#endif // SYSTICK_LOAD_INIT > (1<<24)

//TODO: a chequear
//#define ISR_COUNT (SYSTICK_ISR_FREQUENCY_HZ / 100000U) //MAL
#define MS2COUNTS(periodo) periodo / ISR_COUNT

static systick_callback_t st_callback;
bool SysTick_Init(systick_callback_t callback)
{
	bool init_status = false;
	NVIC_EnableIRQ(SysTick_IRQn);

	if (callback != NULL)
	{
		SysTick->CTRL = 0x00; //Enable sysT interrupt
		SysTick->LOAD = SYSTICK_LOAD_INIT; //00100000L  - 1;
		SysTick->VAL = 0x00;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		st_callback = callback;
		init_status = true;
	}
	return init_status;
}

__ISR__ SysTick_Handler(void)
{
	st_callback();
}
