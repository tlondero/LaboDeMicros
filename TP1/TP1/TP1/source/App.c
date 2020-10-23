
/////***************************************************************************//**
//// @file     App.c
//// @brief    Application functions
//// @author   MAGT
//// ******************************************************************************/
////

#include "drivers/headers/Card.h"
#include "drivers/headers/FRDM.h"
#include "drivers/headers/PV.h"
#include "drivers/headers/timer.h"
#include "app/headers/fsm.h"
#include "app/headers/FEData.h"
#include "app/headers/FrontEnd.h"
static state st;
void App_Init(void) {
	timerInit();//Inits all the drivers
	FRDMInit();
	cardInitDriver(0);
	PVInit();
	st=FSMInitState();//init FSM back end
}

void App_Run(void) {

	st = FSMRun(st); //Updates state of FSM
	drawFrontEnd((*(FSMGetFEData())),st);// Draws front end
	FRDMLedPoll();//Update leds

}
