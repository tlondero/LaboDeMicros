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
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


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
void callback(void){
	xd = 10;
}

void OC_TB_AppInit(void){
	FTM_DATA data;
			data.CNT = 0;
			data.CNTIN = 0;
			data.CNV = 10000;
			data.MODE = FTM_mOutputCompare;
			data.MODULO = 0xFFFF;
			data.OC_EFFECT = FTM_eToggle;
			data.PSC=FTM_PSC_x128;
			data.CALLBACK = callback;
			FTMInit(71, data); //ptc1
			FTMStartClock(0);
}

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
	OC_TB_AppInit();
}


void App_Run (void)
{

}
