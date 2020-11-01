/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

//#include <header/ejercicios_headers/BalizaSwitchSysTick/balizaSwitchSysTick.h>
//#include "header/ejercicios_headers/Baliza/baliza.h"
//#include "header/ejercicios_headers/BalizaSysTick/balizast.h"
#include "header/board.h"
#include "stdbool.h"
#include <stdio.h>
#include "header/SysTick.h"
#include "header/timer.h"
#include "header/Button.h"
#include "header/FTM.h"
#include "header/timer.h"
#include "header/PORT.h"
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




/*******************************************************************************
 * BALIZA
 ******************************************************************************/
static int8_t ftmid1;
static int8_t ftmid2;
static int8_t ftmid3;
static int8_t ftmid4;
static int8_t ftmid5;
static int8_t ftmid6;
static int8_t ftmid7;
static int8_t ftmid8;
void App_Init (void)
{
	FTM_DATA data;
	PORT_Init();
	data.CNTIN = 0;
	data.CNV = 0xFFFF/2;
	data.EPWM_LOGIC = FTM_lAssertedHigh;
	data.MODE = FTM_mPulseWidthModulation;
	data.MODULO = 0xFFFF;
	data.PSC = FTM_PSC_x1;
	ftmid4 = FTMInit(PC, 9, data);
	ftmid5 = FTMInit(PC, 8, data);
	ftmid6 = FTMInit(PC, 1, data);
	ftmid7 = FTMInit(PB, 19, data);
	ftmid8 = FTMInit(PB, 18, data);
	FTMStartClock(0);
	FTMStartClock(2);
	FTMStartClock(3);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

}
