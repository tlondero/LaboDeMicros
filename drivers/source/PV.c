/***************************************************************************//**
 @file     PV.c
 @brief    
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../headers/PV.h"
#include "../headers/board.h"
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define	PV_BUTTON	PORTNUM2PIN(PC,6)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C3		PORTNUM2PIN(PC,3)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C5		PORTNUM2PIN(PC,5)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/

static PVButton button;
static PVEncoder idEncoder;

static uint8_t brightness;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void PVInit(void) {

	button = ButtonInit(PV_BUTTON, INPUT_PULLUP);

	encoder_init();
	idEncoder = encoder_register(PIN_C3, PIN_C5);

	dispInit();
	brightness = 20;
	dispBrightness(brightness);

	//led	
}

PVEv PVGetEv(void) {

	if (encoder_event_avb(idEncoder)) {
		event_t ev = encoder_pop_event(idEncoder);
		switch (ev) {
		case (RIGHT_TURN):
			return ENC_RIGHT;
		case (LEFT_TURN):
			return ENC_LEFT;
		default:
			return NO_PV_EV;
		}
	} else {
		uint8_t btnev = *ButtonGetEvent(button);
		switch (btnev) {
		case (PRESS):
			return BTN_PRESS;
		case (RELEASE):
			return BTN_RELEASE;
		case (LKP):
			return BTN_LKP;
		case (SKP):
			return BTN_SKP;
		case (NO_EV):
			return NO_PV_EV;
		default:
			return NO_PV_EV;
		}
	}

}

void PVDisplayClear(void) {
	dispClearAll();
}

bool PVDisplaySetBright(uint8_t br) {
	bool valid = true;
	if ((br >= 1) && (br <= 100)) {
		brightness = br;
		dispBrightness(brightness);
	} else if (br == 0) {
		dispClearAll();
	} else {
		valid = false;
	}
	return valid;
}

bool PVDisplayIncBright(void) {
	bool topValue = false;
	brightness += 20;
	if (brightness >= 100) {
		brightness = 100;
		topValue = true;
	}
	dispBrightness(brightness);
	return topValue;
}

bool PVDisplayDecBright(void) {
	bool bottomValue = false;
	brightness -= 20;
	if (brightness <= 0) {
		brightness = 0;
		bottomValue = true;
		dispClearAll();
	} else {
		dispBrightness(brightness);
	}
	return bottomValue;
}

//INCREASE BRIGHT AND DECREASE

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/
