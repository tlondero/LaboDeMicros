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
//#include "drivers/headers/board.h"
////#include "drivers/headers/led.h"
//#include "stdbool.h"
//#include <stdio.h>
//#include "drivers/headers/SysTick.h"
//#include "drivers/headers/timer.h"
////#include "drivers/headers/Button.h"
//#include "drivers/headers/FRDM.h"
//
///******************************************************************************
// * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
// *****************************************************************************/
//
///******************************************************************************
// * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
// *****************************************************************************/
//
///******************************************************************************
// *******************************************************************************
// GLOBAL FUNCTION DEFINITIONS
// *******************************************************************************
// *******************************************************************************
// *****************************************************************************/
//
///******************************************************************************
// * INTERRUPCIONES
// *****************************************************************************/
//
///******************************************************************************
// * BALIZA
// *****************************************************************************/
//
//static uint8_t count = 0;
//
//void turn_on_led(void);
//void App_Init(void) {
//
//	//Interrupciones de pines
//
//	FRDMInit();
//	FRDMSuscribeEvent(PRESS_SW2, true);
//	FRDMSuscribeEvent(PRESS_SW3, true);
//
//}
//
////Función que se llama constantemente en un ciclo infinito
//
//void App_Run(void) {
//
//	if (FRDMCheckEvent()) {
//		if (PVGetEv() == PRESS_SW2) {
//			FRDMLedFlash(PURPLE);
//		} else if (PVGetEv() == PRESS_SW3) {
//			if (++count == CANT_COLORS) {
//				count = 0;
//			}
//			FRDMLedColor(count);
//		}
//	}
//
//	FRDMLedPoll();
//
//}

//                           NO BORRAR                ////
/////***************************************************************************//**
//// @file     App.c
//// @brief    Application functions
//// @author   MAGT
//// ******************************************************************************/
////
//////////// MAIN DE PROGRAMA//////////////////
//                           NO BORRAR                ////
#include "drivers/headers/Card.h"
#include "drivers/headers/FRDM.h"
#include "drivers/headers/PV.h"
#include "drivers/headers/timer.h"
#include "app/headers/fsm.h"
#include "app/headers/FEData.h"
#include "app/headers/FrontEnd.h"
static state st;
void App_Init(void) {
	timerInit();
	FRDMInit();
	cardInitDriver(0);
	PVInit();
	st=FSMInitState();
}

void App_Run(void) {

	st = FSMRun(st);
	drawFrontEnd((*(FSMGetFEData())),st);
	FRDMLedPoll();
}

//                           NO BORRAR                ////
