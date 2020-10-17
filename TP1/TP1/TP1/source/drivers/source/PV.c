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

#define	PV_BUTTON		PORTNUM2PIN(PC,0)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C2_EN		PORTNUM2PIN(PC,2)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C7_EN		PORTNUM2PIN(PC,7)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
 #define P_RED
 #define P_RED_NUM

 #define P_GREEN
 #define P_GREEN_NUM

 #define P_BLUE
 #define P_BLUE_NUM
 */

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/

static PVButton_t button;
static PVEncoder_t idEncoder;

static int8_t dispBright;

static int8_t idLedRed;
static int8_t idLedGreen;
static int8_t idLedBlue;

static bool ledOn;
static bool ledFlash;
static uint8_t lastColor;

static bool isEvent = false;
static PVEv_t event = NO_PV_EV;
static uint8_t listEv[NO_PV_EV] = { 0 };

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

	//Button init
	button = ButtonInit(PV_BUTTON, INPUT_PULLUP);

	//Encoder init
	idEncoder = EncoderRegister(PIN_C2_EN, PIN_C7_EN);

	//Display init
	dispInit();
	dispBright = 20;

	led_init_driver();

	idLedRed = led_init_led(PB, 22, TURNS_ON_WITH_0);
	idLedBlue = led_init_led(PB, 21, TURNS_ON_WITH_0);
	idLedGreen = led_init_led(PE, 26, TURNS_ON_WITH_0);

	ledOn = false;
	lastColor = PV_CANT_COLORS;

	//Led default config
	uint32_t bright = 100;
	uint32_t fade = 100;			//ms
	uint32_t dt = 50;				//%
	uint8_t flashes = 0;
	uint32_t period = 1000;		//ms

	led_configure_brightness(idLedRed, bright);
	led_configure_fade(idLedRed, fade);
	led_configure_dt(idLedRed, dt);
	led_configure_flashes(idLedRed, flashes);
	led_configure_period(idLedRed, period);

	led_configure_brightness(idLedBlue, bright);
	led_configure_fade(idLedBlue, fade);
	led_configure_dt(idLedBlue, dt);
	led_configure_flashes(idLedBlue, flashes);
	led_configure_period(idLedBlue, period);

	led_configure_brightness(idLedGreen, bright);
	led_configure_fade(idLedGreen, fade);
	led_configure_dt(idLedGreen, dt);
	led_configure_flashes(idLedGreen, flashes);
	led_configure_period(idLedGreen, period);

	led_set_state(idLedRed, LED_OFF);
	led_set_state(idLedBlue, LED_OFF);
	led_set_state(idLedGreen, LED_OFF);
}

void PVSuscribeEvent(PVEv_t ev, bool state) {
	listEv[ev] = state;
}

bool PVCheckEvent(void) {

	if (EncoderEventAVB(idEncoder)) {
		event_t ev = EncoderPopEvent(idEncoder);
		if ((ev == RIGHT_TURN) && (listEv[ENC_RIGHT])) {
			event = ENC_RIGHT;
		} else if ((ev == LEFT_TURN) && (listEv[ENC_LEFT])) {
			event = ENC_LEFT;
		} else {
			event = NO_PV_EV;
		}
	} else {
		uint8_t btnev = *ButtonGetEvent(button);
		if ((btnev != NO_EV) && (btnev != EOQ)) {
			if ((btnev == PRESS) && (listEv[BTN_PRESS])) {
				event = BTN_PRESS;
			} else if ((btnev == RELEASE) && (listEv[BTN_RELEASE])) {
				event = BTN_RELEASE;
			} else if ((btnev == LKP) && (listEv[BTN_LKP])) {
				event = BTN_LKP;
			} else if ((btnev == SKP) && (listEv[BTN_SKP])) {
				event = BTN_SKP;
			} else if ((btnev == NO_EV) && (listEv[NO_PV_EV])) {
				event = NO_PV_EV;
			} else {
				event = NO_PV_EV;
			}
		}
	}
	if (event != NO_PV_EV) {
		isEvent = true;
	} else {
		isEvent = false;
	}
	return isEvent;
}

PVEv_t PVGetEv(void) {
	return event;
}

bool PVButtonIRQ(PVIRQMode_t IRQ_mode, pinIrqFun_t fcallback) {
	uint8_t IRQ = PV_CANT_MODES;
	bool correct_mode = true;
	switch (IRQ_mode) {
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
	} else {
		return false;
	}
}

void PVDisplayClear(void) {
	dispClearAll();
}

bool PVDisplaySetBright(uint8_t br) {
	bool valid = true;
	if ((br >= 1) && (br <= 100)) {
		dispBright = br;
		PVDisplaySetBright(dispBright);
	} else if (br == 0) {
		dispClearAll();
	} else {
		valid = false;
	}
	return valid;
}

bool PVIncreaseBrightness(void) {
	bool topValue = false;
	dispBright += 20;
	if (dispBright >= 100) {
		dispBright = 100;
		topValue = true;
	}
	dispBrightness(dispBright);
	return topValue;
}

bool PVDecreaseBrightness(void) {
	bool bottomValue = false;
	dispBright -= 20;
	if (dispBright <= 0) {
		dispBright = 0;
		bottomValue = true;
		dispClearAll();
	} else {
		dispBrightness(dispBright);
	}
	return bottomValue;
}

bool PVDisplaySendChar(char ch, uint8_t seven_seg_module) {

	bool valid = false;

	if (seven_seg_module < 4) {
		dispSendChar(ch, seven_seg_module);
		valid = true;
	}

	return valid;

}

bool PVDisplayShift(PVDirection_t direction) {

	bool valid = true;

	if (direction == PV_RIGHT) {
		dispShift(RIGHT);
	} else if (direction == PV_LEFT) {
		dispShift(LEFT);
	} else {
		valid = false;
	}
	return valid;
}

bool PVAnimation(animation_t animation, bool activate) {
	bool valid = true;
	if (activate) {
		switch (animation) {
		case IDDLE_ANIMATION:
			break;
		case ASK_PIN_ANIMATION:
			break;
		case ACCESS_ANIMATION:
			break;
		case OPEN_ANIMATION:
			break;
		case USERS_ANIMATION:
			break;
		case BRIGHTNESS_ANIMATION:
			break;
		case INVALID_ID_ANIMATION:
			break;
		default:
			valid = false;
			break;
		}
	}
	return valid;
}

bool PVLedSetBright(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		double newVal = value/100;
		led_configure_fade(idLedRed, newVal);
		led_configure_fade(idLedGreen, newVal);
		led_configure_fade(idLedBlue, newVal);
		valid = true;
	}
	return valid;
}

bool PVLedSetFade(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		led_configure_fade(idLedRed, value);
		led_configure_fade(idLedGreen, value);
		led_configure_fade(idLedBlue, value);
		valid = true;
	}
	return valid;
}

bool PVLedSetDt(uint8_t value) {
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

bool PVLedSetFlash(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		led_configure_flashes(idLedRed, value);
		led_configure_flashes(idLedGreen, value);
		led_configure_flashes(idLedBlue, value);
		valid = true;
	}
	return valid;
}

bool PVLedSetPeriod(uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		led_configure_period(idLedRed, value);
		led_configure_period(idLedGreen, value);
		led_configure_period(idLedBlue, value);
		valid = true;
	}
	return valid;
}

bool PVLedFlash(uint8_t color) {

	bool valid = false;
	PVLedOff();
	ledOn = true;
	valid = true;
	switch (color) {
	case (PV_RED):
		led_flash(idLedRed);
		lastColor = PV_RED;
		break;
	case (PV_YELLOW):
		led_flash(idLedRed);
		led_flash(idLedGreen);
		lastColor = PV_YELLOW;
		break;
	case (PV_GREEN):
		led_flash(idLedGreen);
		lastColor = PV_GREEN;
		break;
	case (PV_BLUE):
		led_flash(idLedBlue);
		lastColor = PV_BLUE;
		break;
	case (PV_CYAN):
		led_flash(idLedBlue);
		led_flash(idLedGreen);
		lastColor = PV_CYAN;
		break;
	case (PV_PURPLE):
		led_flash(idLedRed);
		led_flash(idLedBlue);
		lastColor = PV_PURPLE;
		break;
	case (PV_WHITE):
		led_flash(idLedRed);
		led_flash(idLedBlue);
		led_flash(idLedGreen);
		lastColor = PV_WHITE;
		break;
	default:
		lastColor = PV_CANT_COLORS;
		ledOn = false;
		valid = false;
		break;
	}
	return valid;
}

bool PVLedColor(uint8_t color) {

	bool valid = false;
	PVLedOff();
	ledOn = true;
	ledFlash = true;

	switch (color) {
	case (PV_RED):
		led_set_state(idLedRed, LED_ON);
		lastColor = PV_RED;
		break;
	case (PV_YELLOW):
		led_set_state(idLedRed, LED_ON);
		led_set_state(idLedGreen, LED_ON);
		lastColor = PV_YELLOW;
		break;
	case (PV_GREEN):
		led_set_state(idLedGreen, LED_ON);
		lastColor = PV_GREEN;
		break;
	case (PV_BLUE):
		led_set_state(idLedBlue, LED_ON);
		lastColor = PV_BLUE;
		break;
	case (PV_CYAN):
		led_set_state(idLedGreen, LED_ON);
		led_set_state(idLedBlue, LED_ON);
		lastColor = PV_CYAN;
		break;
	case (PV_PURPLE):
		led_set_state(idLedRed, LED_ON);
		led_set_state(idLedBlue, LED_ON);
		lastColor = PV_PURPLE;
		break;
	case (PV_WHITE):
		led_set_state(idLedRed, LED_ON);
		led_set_state(idLedBlue, LED_ON);
		led_set_state(idLedGreen, LED_ON);
		lastColor = PV_WHITE;
		break;
	default:
		lastColor = PV_CANT_COLORS;
		ledOn = false;
		ledFlash = false;
		valid = false;
		break;
	}
	return valid;
}

void PVToggleOnOff(void) {
	if (lastColor != PV_CANT_COLORS) {
		if (ledOn) {
			PVLedOff();
		} else if (ledFlash) {
			PVLedFlash(lastColor);
		} else {
			PVLedColor(lastColor);
		}
	}
}

void PVLedOff(void) {
	led_set_state(idLedRed, LED_OFF);
	led_set_state(idLedBlue, LED_OFF);
	led_set_state(idLedGreen, LED_OFF);
	ledOn = false;
}

void PVLedPoll(void) {
	led_poll();
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
