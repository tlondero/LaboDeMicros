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
#include "../headers/gpio.h"
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define	PV_BUTTON	PORTNUM2PIN(PC,0)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C2		PORTNUM2PIN(PC,2)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C7		PORTNUM2PIN(PC,7)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

	EncoderInit();
	idEncoder = EncoderRegister(PIN_C2,PIN_C7);

	dispInit();
	brightness = 20;
	//TODO: implementar
	//dispBrightness(brightness);

	//led	
}

PVEv PVGetEv(void) {

	if (EncoderEventAVB(idEncoder)) {
		event_t ev = EncoderPopEvent(idEncoder);
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

void PVButtonIRQ(uint8_t IRQ_mode, pinIrqFun_t fcallback) {
	uint8_t IRQ = PV_CANT_MODES;
	bool correct_mode = true;
	switch(IRQ_mode){
	case (PV_DISABLE):
		IRQ = GPIO_IRQ_MODE_DISABLE;
		break;
	case (PV_LSTATE):
		IRQ = GPIO_IRQ_MODE_LOW_STATE;
		break;
	case (PV_HSTATE):
		IRQ = GPIO_IRQ_MODE_HIGH_STATE;
		break;
	case (PV_REDGE):
		IRQ = GPIO_IRQ_MODE_RISING_EDGE;
		break;
	case (PV_FEDGE):
		IRQ = GPIO_IRQ_MODE_FALLING_EDGE;
		break;
	case (PV_BEDGES):
		IRQ = GPIO_IRQ_MODE_BOTH_EDGES;
		break;
	default:
		correct_mode = false;
		break;
	}
	
	if (correct_mode) {
		ButtonSetIRQ(button, IRQ, fcallback);
		return true;
	}
	else{
		return false;
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

bool PVIncreaseBrightness(void) {
	bool topValue = false;
	brightness += 20;
	if (brightness >= 100) {
		brightness = 100;
		topValue = true;
	}
	dispBrightness(brightness);
	return topValue;
}

bool PVDecreaseBrightness(void) {
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
