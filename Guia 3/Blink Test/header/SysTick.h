
/***************************************************************************//**
  @file     SysTick.h
  @brief    SysTick driver
  @author   MAGT
 ******************************************************************************/

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

//#define SYSTICK_ISR_FREQUENCY_HZ 1000U
#define BLINK_FREQ_HZ 2U
//#if SYSTICK_ISR_FREQUENCY_HZ % (2*BLINK_FREQ_HZ) != 0
//#warning BLINK cannot implement this exact frequency.Using floor(SYSTICK_ISR_FREQUENCY_HZ/BLINK_FREQ_HZ/2) instead.
//#endif

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void (*systick_callback_t)(void);

typedef struct CallbackInterrupt{
	systick_callback_t stick_callback;
	uint32_t periodo; //En ms, dentro de SysTickInit son convertidas a count.
	bool enable;	//enable flag
}CallbackInterrupt;


/*******************************************************************************
 * CALLBACKS STORAGE
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief Initialize SysTic driver
 * @param funcallback Function to be call every SysTick
 * @return Initialization and registration succeed
 */
//bool SysTick_Init (void (*funcallback)(void));

bool SysTick_Init (CallbackInterrupt *irqs_callback[], int cantidad);

/*******************************************************************************
 ******************************************************************************/

#endif // _SYSTICK_H_


