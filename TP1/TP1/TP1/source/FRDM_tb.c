/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
//
////#include <header/ejercicios_headers/BalizaSwitchSysTick/balizaSwitchSysTick.h>
////#include "header/ejercicios_headers/Baliza/baliza.h"
////#include "header/ejercicios_headers/BalizaSysTick/balizast.h"
//#include "drivers/headers/board.h"
////#include "drivers/headers/led.h"
//#include "stdbool.h"
//#include <stdio.h>
//#include "drivers/headers/SysTick.h"
//#include "drivers/headers/timer.h"
////#include "drivers/headers/Button.h"
//#include "drivers/headers/FRDM.h"
//
///*******************************************************************************
// * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
// ******************************************************************************/
//
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
///*******************************************************************************
// * BALIZA
// ******************************************************************************/
//
//static FRDMButtonEv_t ev1;
//static FRDMButtonEv_t ev2;
//
//static uint8_t count = 0;
//
//void turn_on_led(void);
//void App_Init(void) {
//
//	//Interrupciones de pines
//
//	FRDMInit();
//}
//
///* Función que se llama constantemente en un ciclo infinito */
//
//void App_Run(void) {
//
//	ev1 = FRDMButtonGetEv(BUTTON_SW2);
//	ev2 = FRDMButtonGetEv(BUTTON_SW3);
//	if (ev1 == PRESS) {
//		FRDMLedToggle();
//	}
//	if (ev2 == PRESS) {
//		if(++count == CANT_COLORS){
//			count = 0;
//		}
//		FRDMLedColor(count);
//	}
//
//	FRDMLedPoll();
//}
