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

void PV_tb_Init(void) {
	PVInit();
	PVSuscribeEvent(ENC_RIGHT, true);
	PVSuscribeEvent(ENC_LEFT, true);
	br = 0;
}

void PV_tb_Run(void) {

	/*
	//PVDisplaySetTime(1000);
	PVDispSetMess("Esto no funciona ni a ganchos");
	PVDisplaySetShift(PV_RIGHT);
	PVDispMessShiftOn();
	*/


	if (PVCheckEvent()) {
		if (PVGetEv() == ENC_RIGHT) {
			br += 5;
		} else if (PVGetEv() == ENC_LEFT) {
			br -= 5;
		}

		if (br <= 10){
			PVStatusLedSelect(ON_ST_ALL, false);
		}
		else if ((br > 10) && (br <= 20)) {
			PVStatusLedSelect(ON_ST_1, true);
		} else if ((br > 20) && (br <= 30)) {
			PVStatusLedSelect(ON_ST_2, true);
		} else {
			PVStatusLedSelect(ON_ST_3, true);
		}
	}

	PVLedPoll();

}
