/*************************************************************************
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
#include "drivers/headers/PV.h"

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

/****************************************************************************
 * INTERRUPCIONES
 ***************************************************************************/

static uint8_t br;

void brightUp1(uint8_t b){
	uint8_t bb = b + 10;
	if (bb >= 100){
		bb = 100;
	}
	br = bb;
}

void brightDown1(uint8_t b){
	uint8_t bb = b - 10;
	if ((bb > 100) || (bb == 10)){
		bb = 0;
	}
	br = bb;
}

void PV_tb_Init(void) {
	PVInit();
	PVSuscribeEvent(ENC_RIGHT, true);
	PVSuscribeEvent(ENC_LEFT, true);
	br = 20;
	PVLedSetBright(PV_LED_1, br);
	PVLedOn(PV_LED_1);
}

void PV_tb_Run(void) {

	if (PVCheckEvent()) {
		if (PVGetEv() == ENC_RIGHT) {
			brightUp1(br);
			PVLedSetBright(PV_LED_1, br);
		}
		else if (PVGetEv() == ENC_LEFT) {
			brightDown1(br);
			PVLedSetBright(PV_LED_1, br);
		}
	}

	PVLedPoll();
}
