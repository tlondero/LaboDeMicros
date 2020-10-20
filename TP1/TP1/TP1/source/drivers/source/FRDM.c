/***************************************************************************//**
 @file     FRDM.c
 @brief    
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../headers/FRDM.h"
#include "../headers/board.h"
#include "../headers/gpio.h"
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static FRDMButton_t sw2;
static FRDMButton_t sw3;

static int8_t idLedRed;
static int8_t idLedGreen;
static int8_t idLedBlue;

static bool ledOn;
static bool ledFlash;
static uint8_t lastColor;

static bool isEvent = false;
static FRDMEv_t event = NO_FRDM_EV;
static uint8_t listEv[NO_FRDM_EV] = { 0 };

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

bool FRDMInit(void) {
	//Button Init
	sw2 = ButtonInit(BUTTON_SW2, INPUT_PULLUP);
	sw3 = ButtonInit(BUTTON_SW3, INPUT_PULLUP);

	isEvent = false;
	ledFlash = false;
	event = NO_FRDM_EV;

	//Led init
	led_init_driver();

	bool okLed = true;

	//PORTNUM2PIN(p,n)
	idLedRed = led_init_led(PORTNUM2PIN(PB, 22), TURNS_ON_WITH_0);
	idLedGreen = led_init_led(PORTNUM2PIN(PE, 26), TURNS_ON_WITH_0);
	idLedBlue = led_init_led(PORTNUM2PIN(PB, 21), TURNS_ON_WITH_0);

	/*
	 idLedRed = led_init_led(PB, 22, TURNS_ON_WITH_0);
	 idLedGreen = led_init_led(PE, 26, TURNS_ON_WITH_0);
	 idLedBlue = led_init_led(PB, 21, TURNS_ON_WITH_0);
	 */

	if ((idLedRed == -1) || (idLedGreen == -1) || (idLedBlue == -1)) {
		okLed = false;
	} else {

		ledOn = false;
		lastColor = CANT_COLORS;

		//Led default config
		uint32_t fade = 100;			//ms
		uint32_t dt = 50;				//%
		uint8_t flashes = 0;
		uint32_t period = 1000;		//ms
		uint32_t time = 0;		//ms

		led_configure_fade(idLedRed, fade);
		led_configure_time(idLedRed, time);
		led_configure_dt(idLedRed, dt);
		led_configure_flashes(idLedRed, flashes);
		led_configure_period(idLedRed, period);

		led_configure_fade(idLedBlue, fade);
		led_configure_time(idLedBlue, time);
		led_configure_dt(idLedBlue, dt);
		led_configure_flashes(idLedBlue, flashes);
		led_configure_period(idLedBlue, period);

		led_configure_fade(idLedGreen, fade);
		led_configure_time(idLedBlue, time);
		led_configure_dt(idLedGreen, dt);
		led_configure_flashes(idLedGreen, flashes);
		led_configure_period(idLedGreen, period);

		led_set_state(idLedRed, LOW);
		led_set_state(idLedBlue, LOW);
		led_set_state(idLedGreen, LOW);

	}
	return okLed;
}

void FRDMToggleOnOff(void) {
	if (lastColor != CANT_COLORS) {
		if (ledOn) {
			FRDMLedOff();
		} else if (ledFlash) {
			FRDMLedFlash(lastColor);
		} else {
			FRDMLedColor(lastColor);
		}
	}
}

void FRDMLedOff(void) {
	led_set_state(idLedRed, LOW);
	led_set_state(idLedBlue, LOW);
	led_set_state(idLedGreen, LOW);
	ledOn = false;
}

bool FRDMLedSetFade(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		led_configure_fade(idLedRed, value);
		led_configure_fade(idLedGreen, value);
		led_configure_fade(idLedBlue, value);
		valid = true;
	}
	return valid;
}

bool FRDMLedSetDt(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		uint8_t realdt = 100 - value;
		led_configure_dt(idLedRed, realdt);
		led_configure_dt(idLedGreen, realdt);
		led_configure_dt(idLedBlue, realdt);
		valid = true;
	}
	return valid;
}

bool FRDMLedSetFlash(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		led_configure_flashes(idLedRed, value);
		led_configure_flashes(idLedGreen, value);
		led_configure_flashes(idLedBlue, value);
		valid = true;
	}
	return valid;
}

bool FRDMLedSetPeriod(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		led_configure_period(idLedRed, value);
		led_configure_period(idLedGreen, value);
		led_configure_period(idLedBlue, value);
		valid = true;
	}
	return valid;
}

bool FRDMLedSetTime(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		led_configure_time(idLedRed, value);
		led_configure_time(idLedGreen, value);
		led_configure_time(idLedBlue, value);
		valid = true;
	}
	return valid;
}

void FRDMLedColor(uint8_t color) {

	FRDMLedOff();
	ledOn = true;
	ledFlash = true;

	switch (color) {
	case (RED):
		led_set_state(idLedRed, HIGH);
		lastColor = RED;
		break;
	case (YELLOW):
		led_set_state(idLedRed, HIGH);
		led_set_state(idLedGreen, HIGH);
		lastColor = YELLOW;
		break;
	case (GREEN):
		led_set_state(idLedGreen, HIGH);
		lastColor = GREEN;
		break;
	case (BLUE):
		led_set_state(idLedBlue, HIGH);
		lastColor = BLUE;
		break;
	case (CYAN):
		led_set_state(idLedGreen, HIGH);
		led_set_state(idLedBlue, HIGH);
		lastColor = CYAN;
		break;
	case (PURPLE):
		led_set_state(idLedRed, HIGH);
		led_set_state(idLedBlue, HIGH);
		lastColor = PURPLE;
		break;
	case (WHITE):
		led_set_state(idLedRed, HIGH);
		led_set_state(idLedBlue, HIGH);
		led_set_state(idLedGreen, HIGH);
		lastColor = WHITE;
		break;
	default:
		lastColor = CANT_COLORS;
		ledOn = false;
		ledFlash = false;
		break;
	}
}

bool FRDMLedFlash(uint8_t color) {

	bool valid = false;
	FRDMLedOff();
	ledOn = true;
	valid = true;
	switch (color) {
	case (RED):
		led_flash(idLedRed);
		lastColor = RED;
		break;
	case (YELLOW):
		led_flash(idLedRed);
		led_flash(idLedGreen);
		lastColor = YELLOW;
		break;
	case (GREEN):
		led_flash(idLedGreen);
		lastColor = GREEN;
		break;
	case (BLUE):
		led_flash(idLedBlue);
		lastColor = BLUE;
		break;
	case (CYAN):
		led_flash(idLedBlue);
		led_flash(idLedGreen);
		lastColor = CYAN;
		break;
	case (PURPLE):
		led_flash(idLedRed);
		led_flash(idLedBlue);
		lastColor = PURPLE;
		break;
	case (WHITE):
		led_flash(idLedRed);
		led_flash(idLedBlue);
		led_flash(idLedGreen);
		lastColor = WHITE;
		break;
	default:
		lastColor = CANT_COLORS;
		ledOn = false;
		valid = false;
		break;
	}
	return valid;
}

void FRDMLedPoll(void) {
	led_poll();
}

bool FRDMButtonIRQ(uint8_t button, uint8_t IRQ_mode, pinIrqFun_t fcallback) {
	if ((button == BUTTON_SW2) || (button == BUTTON_SW3)) {
		uint8_t IRQ = BT_CANT_MODES;
		bool correct_mode = true;
		switch (IRQ_mode) {
		case (BT_DISABLE):
			IRQ = GPIO_IRQ_MODE_DISABLE;
			break;
		case (BT_LSTATE):
			IRQ = GPIO_IRQ_MODE_LOW_STATE;
			break;
		case (BT_HSTATE):
			IRQ = GPIO_IRQ_MODE_HIGH_STATE;
			break;
		case (BT_REDGE):
			IRQ = GPIO_IRQ_MODE_RISING_EDGE;
			break;
		case (BT_FEDGE):
			IRQ = GPIO_IRQ_MODE_FALLING_EDGE;
			break;
		case (BT_BEDGES):
			IRQ = GPIO_IRQ_MODE_BOTH_EDGES;
			break;
		default:
			correct_mode = false;
			break;
		}

		if (correct_mode) {
			ButtonSetIRQ(button, IRQ, fcallback);
			return true;
		} else {
			return false;

		}
	} else {
		return false;
	}
}

void FRDMSuscribeEvent(FRDMEv_t ev, bool state) {
	listEv[ev] = state;
}

bool FRDMCheckEvent(void) {
	ButtonEvent ev = *ButtonGetEvent(sw2);
	ButtonEvent ev2 = *ButtonGetEvent(sw3);
	if ((ev != NO_EV) && (ev != EOQ)) {
		if ((ev == PRESS) && (listEv[PRESS_SW2])) {
			event = PRESS_SW2;
		} else if ((ev == RELEASE) && (listEv[RELEASE_SW2])) {
			event = RELEASE_SW2;
		} else if ((ev == LKP) && (listEv[LKP_SW2])) {
			event = LKP_SW2;
		} else if ((ev == SKP) && (listEv[SKP_SW2])) {
			event = SKP_SW2;
		} else {
			event = NO_FRDM_EV;
		}

	} else if ((ev2 != NO_EV) && (ev2 != EOQ)) {
		if ((ev2 == PRESS) && (listEv[PRESS_SW3])) {
			event = PRESS_SW3;
		} else if ((ev2 == RELEASE) && (listEv[RELEASE_SW3])) {
			event = RELEASE_SW3;
		} else if ((ev2 == LKP) && (listEv[LKP_SW3])) {
			event = LKP_SW3;
		} else if ((ev2 == SKP) && (listEv[SKP_SW3])) {
			event = SKP_SW3;
		} else {
			event = NO_FRDM_EV;
		}
	} else {
		event = NO_FRDM_EV;
	}

	if (event != NO_FRDM_EV) {
		isEvent = true;
	} else {
		isEvent = false;
	}
	return isEvent;
}

FRDMEv_t FRDMGetEv(void) {
	return event;
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/
