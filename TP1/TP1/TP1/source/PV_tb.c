**************************************************************************
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 *****************************************************************************

******************************************************************************
 * INCLUDE HEADER FILES
 *****************************************************************************

#include <stdbool.h>
#include <stdio.h>
#include "drivers/headers/FTM.h"
#include "drivers/headers/FRDM.h"
#include "drivers/headers/PV.h"
#include "drivers/headers/PORT.h"
#include "drivers/headers/board.h"

******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 *****************************************************************************
#define PIN_PTC1 71
******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 *****************************************************************************

******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 *****************************************************************************

******************************************************************************
 * INTERRUPCIONES
 *****************************************************************************

static int8_t bright;
static PVEv ev;

void App_Init (void) {
	FRDMInit();
	PVInit();
	bright = 20;
	FRDMLed(R, 100);
	PVSuscribeEvent(ENC_RIGHT,true);
	PVSuscribeEvent(ENC_LEFT,true);
}

void App_Run(void) {

	ev = PVGetEv();
	if (PVCheckEvent()){
		if(PVGetEv() == ENC_RIGHT)) {
			FRDMLedFlash(PURPLE);
		}
		else if (PVGetEv() == ENC_LEFT){
			FRDMLedOff();
		}
	}
	
	FRDMLedPoll();
}
