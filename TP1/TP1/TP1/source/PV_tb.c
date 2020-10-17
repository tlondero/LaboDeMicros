/**************************************************************************
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 *****************************************************************************/

/******************************************************************************
 * INCLUDE HEADER FILES
 ****************************************************************************/

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
#include "drivers/headers/PV.h"

/******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 *****************************************************************************/

/******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 *****************************************************************************/

/*****************************************************************************
 * INTERRUPCIONES
 ****************************************************************************/

void App_Init(void) {
	PVInit();
	PVSuscribeEvent(ENC_RIGHT, true);
	PVSuscribeEvent(ENC_LEFT, true);
}

void App_Run(void) {

	if (PVCheckEvent()) {
		if (PVGetEv() == ENC_RIGHT) {
			PVLedFlash(PV_LED_1,0);
		}
		else if (PVGetEv() == ENC_LEFT) {
			PVLedOff(PV_LED_1);
		}
	}

	PVLedPoll();
}
