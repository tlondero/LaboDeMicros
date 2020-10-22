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
#include "../headers/debug.h"
#if DEBUGGIN_MODE && DEBUGGIN_MODE_SYSTICK
#include "../headers/gpio.h"
#endif

#include <stddef.h>
#include <stdint.h>
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SYSTICK_LOAD_INIT ((__CORE_CLOCK__ / SYSTICK_ISR_FREQUENCY_HZ) - 1U)
#if SYSTICK_LOAD_INIT > (1 << 24)
#error Overflow de SysTick! Ajustar  __CORE_CLOCK__ y SYSTICK_ISR_FREQUENCY_HZ!
#endif // SYSTICK_LOAD_INIT > (1<<24)

static systick_callback_t st_callback;
bool SysTick_Init(systick_callback_t callback)
{
#if DEBUGGIN_MODE_SYSTICK
	gpioMode(DEBUG_PIN, OUTPUT);
	gpioWrite(DEBUG_PIN, LOW);
#endif

	bool init_status = false;
	NVIC_EnableIRQ(SysTick_IRQn);

	if (callback != NULL)
	{
		SysTick->CTRL = 0x00;			   //Enable sysT interrupt
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
#if DEBUGGIN_MODE_SYSTICK
	gpioWrite(DEBUG_PIN, HIGH)
#endif
		st_callback();

#if DEBUGGIN_MODE_SYSTICK
	gpioWrite(DEBUG_PIN, LOW)
#endif
}
