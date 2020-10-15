/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include "drivers/headers/FTM.h"
#include "drivers/headers/FRDM.h"
#include "drivers/headers/PV.h"
#include "drivers/headers/PORT.h"
#include "drivers/headers/board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PIN_PTC1 71
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 * INTERRUPCIONES
 ******************************************************************************/

uint8_t brightUp(uint8_t b) {
	b += 20;
	if (b >= 100) {
		b = 100;
	}
	return b;
}

uint8_t brightDown(uint8_t b) {
	b -= 20;
	if (b <= 0) {
		b = 0;
	}
	return b;
}


//void App_Init (void) {
//	FRDMInit();
//	PVInit();
//}

//void App_Run(void) {
//
//	int8_t bright = 20;
//
//	FRDMLedBright(R, 100);
//
//	while (1) {
//		PVEv ev = PVGetEv();
//		if (ev == ENC_RIGHT) {
//			//FRDMLedFlash(R);
//			bright = brightUp(bright);
//			FRDMLedRGB(bright, 0, 0);
//		}
//		else if (ev == ENC_LEFT) {
//			//FRDMLedOff();
//			bright = brightDown(bright);
//			FRDMLedRGB(bright, 0, 0);
//		}
//		FRDMLedPoll();
//	}
//}
