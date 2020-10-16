///***************************************************************************//**
// @file     App.c
// @brief    Application functions
// @author   Nicolás Magliola
// ******************************************************************************/
//
////////// MAIN DE PROGRAMA??//////////////////

#include "drivers/headers/Card.h"
#include "drivers/headers/FRDM.h"
#include "drivers/headers/PV.h"
#include "drivers/headers/timer.h"
#include "app/headers/fsm.h"
#include "app/headers/FEData.h"
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
	FEData  mydata = *(FSMGetFEData());
//  doStuffFrontEnd(mydata);
}


///*******************************************************************************
// * INCLUDE HEADER FILES
// ******************************************************************************/
//
//#include <stdbool.h>
//#include <stdio.h>
//#include "drivers/headers/FTM.h"
//#include "drivers/headers/FRDM.h"
//#include "drivers/headers/PV.h"
//#include "drivers/headers/PORT.h"
//#include "drivers/headers/board.h"
//
///*******************************************************************************
// * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
// ******************************************************************************/
//#define PIN_PTC1 71
///*******************************************************************************
// * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
// ******************************************************************************/
//
///*******************************************************************************
// *******************************************************************************
// GLOBAL FUNCTION DEFINITIONS
// *******************************************************************************
// *******************************************************************************
// ******************************************************************************/
//
///*******************************************************************************
// * INTERRUPCIONES
// ******************************************************************************/
//
//uint8_t brightUp(uint8_t b) {
//	b += 20;
//	if (b >= 100) {
//		b = 100;
//	}
//	return b;
//}
//
//uint8_t brightDown(uint8_t b) {
//	b -= 20;
//	if (b <= 0) {
//		b = 0;
//	}
//	return b;
//}
//
//static int8_t bright;
//static PVEv ev;
//
//void App_Init (void) {
//	FRDMInit();
//	PVInit();
//	bright = 20;
//	FRDMLedBright(R, 100);
//}
//
//void App_Run(void) {
//
//	ev = PVGetEv();
//	if (ev == ENC_RIGHT) {
//		//FRDMLedFlash(R);
//		bright = brightUp(bright);
//		FRDMLedRGB(bright, 0, 0);
//	}
//	else if (ev == ENC_LEFT) {
//		//FRDMLedOff();
//		bright = brightDown(bright);
//		FRDMLedRGB(bright, 0, 0);
//	}
//	FRDMLedPoll();
//}
//
//
//
//
//
//
//
//
//






/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

//#include "stdbool.h"
//#include <stdio.h>
//#include "drivers/headers/FTM.h"
//#include "drivers/headers/FRDM.h"
//#include "drivers/headers/PORT.h"
//#include "drivers/headers/board.h"
//#include "drivers/headers/gpio.h"
//
///*******************************************************************************
// * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
// ******************************************************************************/
//#define PIN_PTC1 71
//#define PIN_PTC3 73
//#define PIN_PTB21 67
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
//static uint8_t xd;
//static int8_t button_id1;
//static uint8_t ledid;
//void callback(void){
//	xd = 10;
//}

//void OC_TB_AppInit(void){
//	FTM_DATA data;
//			data.CNT = 0;
//			data.CNTIN = 0;
//			data.CNV = 10000;
//			data.MODE = FTM_mOutputCompare;
//			data.MODULO = 0xFFFF;
//			data.OC_EFFECT = FTM_eToggle;
//			data.PSC=FTM_PSC_x128;
//			data.CALLBACK = callback;
//			FTMInit(71, data); //ptc1
//			FTMStartClock(0);
//}

//void IC_TB_AppInit(void){
//	FTM_DATA data;
//		data.CNT = 0;
//		data.CNTIN = 0;
//		data.CNV = 0;
//		data.MODE = FTM_mInputCapture;
//		data.MODULO = 0xFFFF;
//		data.IC_EDGE = FTM_eRising;
//		data.PSC=FTM_PSC_x128;
//		data.CALLBACK = callback;
//		FTMInit(71, data); //ptc1
//		FTMStartClock(0);
//}



//void EPWM_TB_AppInit(void){
//	FTM_DATA data;
//			data.CNT = 0;
//			data.CNTIN = 0;
//			data.CNV = 9000;
//			data.MODE = FTM_mPulseWidthModulation;
//			data.MODULO = 10000;
//			data.EPWM_LOGIC = FTM_lAssertedLow;
//			data.PSC=FTM_PSC_x128;
//			data.CALLBACK = callback;
//			FTMInit(71, data); //ptc1
//			FTMStartClock(0);
//}

//void App_Init (void)
//{
//    PORT_Init();
//    button_id1 = ButtonInit(PIN_SW2, INPUT_PULLUP);
//    led_init_driver();
//    ledid = led_init_led(PC, 3, TURNS_ON_WITH_0);
//    led_configure_brightness(ledid, 0.25);
//    led_configure_time(ledid, 3000);
//
//}
//
//
//void App_Run (void)
//{
//	const ButtonEvent * evsw1 = ButtonGetEvent(button_id1);
//	while((*evsw1) != EOQ){
//		switch (*evsw1){
//			case LKP:
//
//				break;
//			case PRESS:
//
//				break;
//
//			case RELEASE:
//
//
//				break;
//			case SKP:
//				led_set_state(ledid, LED_ON);
//				break;
//			default:
//
//				break;
//			}
//		evsw1++;
//	    }
//	led_poll();
//}
//
////void App_Run(void) {
////	int8_t bright = 20;
////
////	FRDMLedRGB(bright, 0, 0);
////
////	while (1) {
////		FRDMButtonEv ev1 = FRDMButtonGetEv(BUTTON_SW2);
////		FRDMButtonEv ev2 = FRDMButtonGetEv(BUTTON_SW3);
////		if (ev1 == PRESS) {
////			bright += 20;
////			if (bright >= 100) {
////				bright = 100;
////			}
////			FRDMLedRGB(bright, 0, 0);
////		}
////		if (ev2 == PRESS) {
////			bright -= 20;
////			if (bright <= 0) {
////				bright = 0;
////			}
////			FRDMLedRGB(bright, 0, 0);
////		}
////
////		FRDMLedPoll();
////	}
////}
