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

static FRDMButton_t sw2;
static FRDMButton_t sw3;

static int8_t idLedRed;
static int8_t idLedGreen;
static int8_t idLedBlue;

static bool ledOn;
static uint8_t lastColor;

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void FRDMInit(void) {
	//Button Init
	sw2 = ButtonInit(BUTTON_SW2, INPUT_PULLUP);
	sw3 = ButtonInit(BUTTON_SW3, INPUT_PULLUP);

	led_init_driver();

	idLedRed = led_init_led(PB, 22, TURNS_ON_WITH_0);
	idLedBlue = led_init_led(PB, 21, TURNS_ON_WITH_0);
	idLedGreen = led_init_led(PE, 26, TURNS_ON_WITH_0);

	ledOn = false;
	lastColor = CANT_COLORS;

	led_set_state(idLedRed, LED_OFF);
	led_set_state(idLedBlue, LED_OFF);
	led_set_state(idLedGreen, LED_OFF);
}

void FRDMLedToggle(void) {
	if (lastColor != CANT_COLORS) {
		if (ledOn) {
			FRDMLedOff();
		} else {
			FRDMLedColor(lastColor);

		}
	}
}

void FRDMLedOff(void) {
	led_set_state(idLedRed, LED_OFF);
	led_set_state(idLedBlue, LED_OFF);
	led_set_state(idLedGreen, LED_OFF);
	ledOn = false;
}

void FRDMLedColor(uint8_t color) {

	FRDMLedOff();
	ledOn = true;

	switch (color) {
	case (RED):
		led_set_state(idLedRed, LED_ON);
		lastColor = RED;
		break;
	case (YELLOW):
		led_set_state(idLedRed, LED_ON);
		led_set_state(idLedGreen, LED_ON);
		lastColor = YELLOW;
		break;
	case (GREEN):
		led_set_state(idLedGreen, LED_ON);
		lastColor = GREEN;
		break;
	case (BLUE):
		led_set_state(idLedBlue, LED_ON);
		lastColor = BLUE;
		break;
	case (CYAN):
		led_set_state(idLedGreen, LED_ON);
		led_set_state(idLedBlue, LED_ON);
		lastColor = CYAN;
		break;
	case (PURPLE):
		led_set_state(idLedRed, LED_ON);
		led_set_state(idLedBlue, LED_ON);
		lastColor = PURPLE;
		break;
	case (WHITE):
		led_set_state(idLedRed, LED_ON);
		led_set_state(idLedBlue, LED_ON);
		led_set_state(idLedGreen, LED_ON);
		lastColor = WHITE;
		break;
	default:
		lastColor = CANT_COLORS;
		ledOn = false;
		break;
	}
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

FRDMButtonEv_t FRDMButtonGetEv(uint8_t button) {
	switch (button) {
	case (BUTTON_SW2):
		return *ButtonGetEvent(sw2);
	case (BUTTON_SW3):
		return *ButtonGetEvent(sw3);
	default:
		return NULL;
	}
}

/*

 int8_t getId(uint8_t led) {
 int8_t ledPort = NO_LED;
 switch (led) {
 case (R):
 ledPort = idLedRed;
 break;
 case (G):
 ledPort = idLedGreen;
 break;
 case (B):
 ledPort = idLedBlue;
 break;
 default:
 break;
 }
 return ledPort;
 }

 bool FRDMLedRGB(uint8_t r, uint8_t g, uint8_t b) {

 if ((r > 255) || (g > 255) || (b > 255)) {
 return false;
 } else {
 if ((r > 0) && (r <= 255)) {
 led_configure_brightness(idLedRed, r);
 led_set_state(idLedRed, LED_ON);
 } else if (r == 0) {
 led_set_state(idLedRed, LED_OFF);
 }

 if ((g > 0) && (g <= 255)) {
 led_configure_brightness(idLedGreen, g);
 led_set_state(idLedRed, LED_ON);
 } else if (g == 0) {
 led_set_state(idLedGreen, LED_OFF);
 }

 if ((b > 0) && (b <= 255)) {
 led_configure_brightness(idLedBlue, b);
 led_set_state(idLedBlue, LED_ON);
 } else if (b == 0) {
 led_set_state(idLedBlue, LED_OFF);
 }
 return true;
 }
 }

 bool FRDMLedBright(uint8_t led, uint8_t value) {
 bool valid = false;
 int8_t ledPort = getId(led);
 if (ledPort != NO_LED) {
 if ((value >= 1) && (value <= 100)) {
 led_configure_brightness(ledPort, value);
 led_set_state(ledPort, LED_ON);
 valid = true;
 } else if (value == 0) {
 led_set_state(ledPort, LED_OFF);
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
 led_set_state(ledPort, LED_ON);
 setState(ledPort, true);
 valid = true;
 }
 return valid;
 }

 bool FRDMLedDt(uint8_t led, uint8_t value) {
 bool valid = false;
 int8_t ledPort = getId(led);
 if ((ledPort != NO_LED) && ((value >= 0) && (value <= 100))) {
 led_configure_dt(ledPort, value);
 led_set_state(ledPort, LED_ON);
 setState(ledPort, true);
 valid = true;
 }
 return valid;
 }

 bool FRDMLedFlash(uint8_t led, uint8_t value) {
 bool valid = false;
 int8_t ledPort = getId(led);
 if ((ledPort != NO_LED) && ((value >= 0) && (value <= 100))) {
 led_set_state(ledPort, LED_ON);
 led_configure_flashes(ledPort, value);
 setState(ledPort, true);
 valid = true;
 }
 return valid;
 }

 bool FRDMLedPeriod(uint8_t led, uint8_t value) {
 bool valid = false;
 int8_t ledPort = getId(led);
 if ((ledPort != NO_LED) && ((value >= 0) && (value <= 100))) {
 led_configure_period(ledPort, value);
 led_set_state(ledPort, LED_ON);
 setState(ledPort, true);
 valid = true;
 }
 return valid;
 }
 */

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/
