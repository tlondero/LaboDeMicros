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

/*
 #define PIN_LED_RED     	PORTNUM2PIN(PB,22) // PTB22
 #define PIN_LED_GREEN   	PORTNUM2PIN(PE,26) // PTE26
 #define PIN_LED_BLUE    	PORTNUM2PIN(PB,21) // PTB21
 */

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

static FRDMButton sw2;
static FRDMButton sw3;

static int8_t idLedRed;
static int8_t idLedBlue;
static int8_t idLedGreen;

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int8_t getId(uint8_t led) {
	int8_t ledPort = NO_LED;
	switch (led) {
	case (R):
		ledPort = idLedRed;
		break;
	case (G):
		ledPort = idLedRed;
		break;
	case (B):
		ledPort = idLedRed;
		break;
	default:
		break;
	}
	return ledPort;
}

void FRDMInit(void) {
	//Button Init
	sw2 = ButtonInit(BUTTON_SW2, INPUT_PULLUP);
	sw3 = ButtonInit(BUTTON_SW3, INPUT_PULLUP);

	//LED Init
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, HIGH);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_GREEN, HIGH);

	led_init_driver();

	idLedRed = led_init_led(PIN_LED_RED, TURNS_ON_WITH_0);
	idLedBlue = led_init_led(PIN_LED_BLUE, TURNS_ON_WITH_0);
	idLedGreen = led_init_led(PIN_LED_GREEN, TURNS_ON_WITH_0);

	const uint8_t brightness = 20;
	const uint32_t fade = 100;			//ms
	const uint32_t dt = 50;				//%
	const uint8_t flashes = 0;
	const uint32_t period = 1000;		//ms

	led_configure_brightness(idLedRed, brightness);
	led_configure_fade(idLedRed, fade);
	led_configure_dt(idLedRed, dt);
	led_configure_flashes(idLedRed, flashes);
	led_configure_period(idLedRed, period);

	led_configure_brightness(idLedBlue, brightness);
	led_configure_fade(idLedBlue, fade);
	led_configure_dt(idLedBlue, dt);
	led_configure_flashes(idLedBlue, flashes);
	led_configure_period(idLedBlue, period);

	led_configure_brightness(idLedGreen, brightness);
	led_configure_fade(idLedGreen, fade);
	led_configure_dt(idLedGreen, dt);
	led_configure_flashes(idLedGreen, flashes);
	led_configure_period(idLedGreen, period);
}

bool FRDMLedBright(uint8_t led, uint8_t value) {
	bool valid = false;
	int8_t ledPort = getId(led);
	if (ledPort != NO_LED) {
		if ((value >= 1) && (value <= 100)) {
			led_configure_brightness(ledPort, value);
			valid = true;
		} else if (value == 0) {
			led_set_state(ledPort, LOW);
			valid = true;
		}
	}
	return valid;
}

bool FRDMLedFade(uint8_t led, uint8_t value) {
	bool valid = false;
	int8_t ledPort = getId(led);
	if ((ledPort != NO_LED) && ((value >= 0) && (value <= 100))) {
		led_configure_fade(ledPort, value);
		bool valid = false;
	}
	return valid;
}

bool FRDMLedDt(uint8_t led, uint8_t value) {
	bool valid = false;
	int8_t ledPort = getId(led);
	if ((ledPort != NO_LED) && ((value >= 0) && (value <= 100))) {
		led_configure_dt(ledPort, value);
		bool valid = false;
	}
	return valid;
}

bool FRDMLedFlash(uint8_t led, uint8_t value) {
	bool valid = false;
	int8_t ledPort = getId(led);
	if ((ledPort != NO_LED) && ((value >= 0) && (value <= 100))) {
		led_configure_flashes(ledPort, value);
		bool valid = false;
	}
	return valid;
}

bool FRDMLedPeriod(uint8_t led, uint8_t value) {
	bool valid = false;
	int8_t ledPort = getId(led);
	if ((ledPort != NO_LED) && ((value >= 0) && (value <= 100))) {
		led_configure_period(ledPort, value);
		bool valid = false;
	}
	return valid;
}

bool FRDMButtonIRQ(uint8_t button, uint8_t IRQ_mode, pinIrqFun_t fcallback) {
	if ((button == BUTTON_SW2) || (button == BUTTON_SW3)) {
		uint8_t IRQ = BT_CANT_MODES;
		bool correct_mode = true;
		switch(IRQ_mode){
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
		}
		return true;
	} else {
		return false;
	}
}

FRDMButtonEv FRDMButtonGetEv(uint8_t button) {
	switch (button) {
	case (BUTTON_SW2):
		return *ButtonGetEvent(sw2);
	case (BUTTON_SW3):
		return *ButtonGetEvent(sw3);
	default:
		return NULL;
	}
}

void FRDMLedColor(uint8_t color) {

	FRDMLedOff();
	led_configure_brightness(idLedGreen, 20);
	led_configure_brightness(idLedRed, 20);
	led_configure_brightness(idLedBlue, 20);

	switch (color) {
	case (RED):
		led_set_state(idLedRed, HIGH);
		break;
	case (ORANGE):
		led_flash(idLedRed);
		led_configure_brightness(idLedBlue, 2);
		led_configure_brightness(idLedGreen, 15);
		led_configure_brightness(idLedRed, 35);
		led_set_state(idLedBlue, HIGH);
		led_set_state(idLedGreen, HIGH);
		break;
	case (YELLOW):
		led_configure_brightness(idLedBlue, 2);
		led_configure_brightness(idLedGreen, 25);
		led_configure_brightness(idLedRed, 30);
		led_set_state(idLedBlue, HIGH);
		led_set_state(idLedGreen, HIGH);
		break;
	case (GREEN):
		led_set_state(idLedGreen, HIGH);
		break;
	case (BLUE):
		led_set_state(idLedBlue, HIGH);
		break;
	case (PURPLE):
		led_set_state(idLedRed, HIGH);
		led_set_state(idLedBlue, HIGH);
		break;
	default:
		break;
	}

}

bool FRDMLedRGB(uint8_t r, uint8_t g, uint8_t b) {

	if ((r > 255) || (g > 255) || (b > 255)) {
		return false;
	} else {
		if ((r > 0) && (r <= 255)) {
			led_configure_brightness(idLedRed, r);
			led_set_state(idLedRed, HIGH);
		} else if (r == 0) {
			led_set_state(idLedRed, LOW);
		}

		if ((g > 0) && (g <= 255)) {
			led_configure_brightness(idLedGreen, g);
			led_set_state(idLedRed, HIGH);
		} else if (g == 0) {
			led_set_state(idLedGreen, LOW);
		}

		if ((b > 0) && (b <= 255)) {
			led_configure_brightness(idLedBlue, b);
			led_set_state(idLedBlue, HIGH);
		} else if (b == 0) {
			led_set_state(idLedBlue, LOW);
		}
		return true;
	}
}

void FRDMLedOff(void) {
	led_set_state(idLedRed, LOW);
	led_set_state(idLedBlue, LOW);
	led_set_state(idLedGreen, LOW);
}

void FRDMLedPoll(void) {
	led_poll();
	pwm_poll();
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/
