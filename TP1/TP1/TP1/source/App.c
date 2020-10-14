/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "stdbool.h"
#include <stdio.h>
#include "drivers/headers/FTM.h"
#include "drivers/headers/FRDM.h"
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
static uint8_t xd;
static int8_t button_id;
static int8_t pwm_id;
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

void App_Init (void)
{
    PORT_Init();
    button_id = ButtonInit(PIN_SW2, INPUT_PULLUP);
    pwm_id = PWMInitSignal(PIN_PTC1, 10000, 0.25, 1);
    if(pwm_id == PWM_NO_SPACE){
    	xd = 20;
    }


	//OC_TB_AppInit();
	//FRDMInit();

}


void App_Run (void)
{
	uint8_t ayche = *(ButtonGetEvent(button_id));
	if(ayche == PRESS){
		PWMStopSignal(pwm_id, 0);
	}
	else if(ayche == RELEASE){
		PWMStartSignal(pwm_id);
	}
}

//void App_Run(void) {
//	int8_t bright = 20;
//
//	FRDMLedRGB(bright, 0, 0);
//
//	while (1) {
//		FRDMButtonEv ev1 = FRDMButtonGetEv(BUTTON_SW2);
//		FRDMButtonEv ev2 = FRDMButtonGetEv(BUTTON_SW3);
//		if (ev1 == PRESS) {
//			bright += 20;
//			if (bright >= 100) {
//				bright = 100;
//			}
//			FRDMLedRGB(bright, 0, 0);
//		}
//		if (ev2 == PRESS) {
//			bright -= 20;
//			if (bright <= 0) {
//				bright = 0;
//			}
//			FRDMLedRGB(bright, 0, 0);
//		}
//
//		FRDMLedPoll();
//	}
//}
