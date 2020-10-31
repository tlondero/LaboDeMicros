/**************************************************************************
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ****************************************************************************/

/*****************************************************************************
 * INCLUDE HEADER FILES
 ***************************************************************************/

//#include <header/ejercicios_headers/BalizaSwitchSysTick/balizaSwitchSysTick.h>
//#include "header/ejercicios_headers/Baliza/baliza.h"
//#include "header/ejercicios_headers/BalizaSysTick/balizast.h"
#include "drivers/headers/board.h"
//#include "drivers/headers/led.h"
#include "stdbool.h"
#include <stdio.h>
#include "drivers/headers/SysTick.h"
#include "drivers/headers/timer.h"
//#include "drivers/headers/Button.h"
#include "drivers/headers/FRDM.h"


/*****************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ****************************************************************************/

/*****************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ****************************************************************************/

/*****************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 ****************************************************************************/

/*****************************************************************************
 * INTERRUPCIONES
 ****************************************************************************/

/*****************************************************************************
 * BALIZA
 ****************************************************************************/

static uint8_t br;

void valueUp(uint8_t b) {
	uint8_t bb = b + 10;
	if (bb >= 100) {
		bb = 100;
	}
	br = bb;
}

void valuetDown(uint8_t b) {
	uint8_t bb = b - 10;
	if ((bb == 0) || (bb > 100)) {
		bb = 0;
	}
	br = bb;
}

void FRDM_tb_Init(void) {
	FRDMInit();
	FRDMSuscribeEvent(PRESS_SW2, true);
	FRDMSuscribeEvent(PRESS_SW3, true);
	br = 50;

	FRDMLedSetDt(br);
	FRDMLedFlash(PURPLE);
}

void FRDM_tb_Run(void) {

	if (FRDMCheckEvent()) {
		if (FRDMGetEv() == PRESS_SW2) {
			valueUp(br);
			FRDMLedSetDt(br);
		} else if (FRDMGetEv() == PRESS_SW3) {
			valuetDown(br);
			FRDMLedSetDt(br);
		}
	}

	FRDMLedPoll();
}
