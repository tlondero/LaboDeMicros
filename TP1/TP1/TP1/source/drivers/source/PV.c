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

static int8_t idLed1;
static int8_t idLed2;
static int8_t idLed3;

static bool ledOn;
static bool ledFlash;
static PVLed_t lastLed;

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

	idLed1 = led_init_led(PB, 22, TURNS_ON_WITH_0);
	idLed3 = led_init_led(PB, 21, TURNS_ON_WITH_0);
	idLed2 = led_init_led(PE, 26, TURNS_ON_WITH_0);

	ledOn = false;
	lastLed = PV_CANT_COLORS;

	//Led default config
	uint32_t bright = 100;
	uint32_t fade = 100;			//ms
	uint32_t dt = 50;				//%
	uint8_t flashes = 0;
	uint32_t period = 1000;		//ms

	led_configure_brightness(idLed1, bright);
	led_configure_fade(idLed1, fade);
	led_configure_dt(idLed1, dt);
	led_configure_flashes(idLed1, flashes);
	led_configure_period(idLed1, period);

	led_configure_brightness(idLed3, bright);
	led_configure_fade(idLed3, fade);
	led_configure_dt(idLed3, dt);
	led_configure_flashes(idLed3, flashes);
	led_configure_period(idLed3, period);

	led_configure_brightness(idLed2, bright);
	led_configure_fade(idLed2, fade);
	led_configure_dt(idLed2, dt);
	led_configure_flashes(idLed2, flashes);
	led_configure_period(idLed2, period);

	led_set_state(idLed1, LED_OFF);
	led_set_state(idLed3, LED_OFF);
	led_set_state(idLed2, LED_OFF);
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

bool PVLedSetBright(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		double newVal = value / 100;
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_fade(idLed1, newVal);
			break;
		case (PV_LED_2):
			led_configure_fade(idLed2, newVal);
			break;
		case (PV_LED_3):
			led_configure_fade(idLed3, newVal);
			break;
		case (PV_LED_ALL):
			led_configure_fade(idLed1, newVal);
			led_configure_fade(idLed2, newVal);
			led_configure_fade(idLed3, newVal);
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetFade(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_fade(idLed1, value);
			break;
		case (PV_LED_2):
			led_configure_fade(idLed2, value);
			break;
		case (PV_LED_3):
			led_configure_fade(idLed3, value);
			break;
		case (PV_LED_ALL):
			led_configure_fade(idLed1, value);
			led_configure_fade(idLed2, value);
			led_configure_fade(idLed3, value);
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetDt(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		uint8_t realdt = 100 - value;
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_fade(idLed1, realdt);
			break;
		case (PV_LED_2):
			led_configure_fade(idLed2, realdt);
			break;
		case (PV_LED_3):
			led_configure_fade(idLed3, realdt);
			break;
		case (PV_LED_ALL):
			led_configure_fade(idLed1, realdt);
			led_configure_fade(idLed2, realdt);
			led_configure_fade(idLed3, realdt);
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetFlash(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_fade(idLed1, value);
			break;
		case (PV_LED_2):
			led_configure_fade(idLed2, value);
			break;
		case (PV_LED_3):
			led_configure_fade(idLed3, value);
			break;
		case (PV_LED_ALL):
			led_configure_fade(idLed1, value);
			led_configure_fade(idLed2, value);
			led_configure_fade(idLed3, value);
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetPeriod(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_fade(idLed1, value);
			break;
		case (PV_LED_2):
			led_configure_fade(idLed2, value);
			break;
		case (PV_LED_3):
			led_configure_fade(idLed3, value);
			break;
		case (PV_LED_ALL):
			led_configure_fade(idLed1, value);
			led_configure_fade(idLed2, value);
			led_configure_fade(idLed3, value);
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedFlash(PVLed_t led, uint8_t color) {

	bool valid = false;
	ledOn = true;
	valid = true;
	switch (led) {
	case (PV_LED_1):
		led_flash(idLed1);
		lastLed = PV_LED_1;
		break;
	case (PV_LED_2):
		led_flash(idLed2);
		lastLed = PV_LED_2;
		break;
	case (PV_LED_3):
		led_flash(idLed3);
		lastLed = PV_LED_3;
		break;
	case (PV_LED_ALL):
		led_flash(idLed1);
		led_flash(idLed3);
		led_flash(idLed2);
		lastLed = PV_LED_ALL;
		break;
	default:
		lastLed = PV_CANT_COLORS;
		ledOn = false;
		valid = false;
		break;
	}
	return valid;
}

bool PVLedOn(PVLed_t led, uint8_t color) {

	bool valid = false;
	ledOn = true;
	ledFlash = true;

	switch (led) {
	case (PV_LED_1):
		led_set_state(idLed1, LED_ON);
		lastLed = PV_LED_1;
		break;
	case (PV_LED_2):
		led_set_state(idLed2, LED_ON);
		lastLed = PV_LED_2;
		break;
	case (PV_LED_3):
		led_set_state(idLed3, LED_ON);
		lastLed = PV_LED_3;
		break;
	case (PV_LED_ALL):
		led_set_state(idLed1, LED_ON);
		led_set_state(idLed3, LED_ON);
		led_set_state(idLed2, LED_ON);
		lastLed = PV_LED_ALL;
		break;
	default:
		lastLed = PV_CANT_COLORS;
		ledOn = false;
		ledFlash = false;
		valid = false;
		break;
	}
	return valid;
}

/*
void PVToggleOnOff(void) {
	if (lastLed != PV_CANT_COLORS) {
		if (ledOn) {
			PVLedOff(lastLed);
		} else if (ledFlash) {
			PVLedFlash(lastLed);
		} else {
			PVLedColor(lastLed);
		}
	}
}*/

void PVLedOff(PVLed_t led) {
	switch (led) {
	case (PV_LED_1):
		led_set_state(idLed1, LED_OFF);
		break;
	case (PV_LED_2):
		led_set_state(idLed2, LED_OFF);
		break;
	case (PV_LED_3):
		led_set_state(idLed3, LED_OFF);
		break;
	case (PV_LED_ALL):
		led_set_state(idLed1, LED_OFF);
		led_set_state(idLed3, LED_OFF);
		led_set_state(idLed2, LED_OFF);
		break;
	default:
		break;
	}
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
