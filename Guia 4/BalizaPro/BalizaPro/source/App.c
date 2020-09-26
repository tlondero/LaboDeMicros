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
#include "header/board.h"
#include "header/gpio.h"
#include "header/SysTick.h"
#include "header/timer.h"
#include "header/Button.h"
#include "header/led.h"
#include "header/pwm.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

enum {
	RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, CANT_COLORS
};

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

//static void update_baliza(int period);
static int8_t idButton1;
static int8_t idButton2;

static int8_t idLedRed;
static int8_t idLedBlue;
static int8_t idLedGreen;

static int8_t idTimer;

static uint32_t timer;

void timerCallback(void) {
	timer++;
}

void selectLedColor(uint8_t a);

void turnOffLeds(void);

void App_Init(void) {
	//Interrupciones de pines

	timerInit();
	led_init_driver();

	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, HIGH);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_GREEN, HIGH);

	idButton1 = ButtonInit(PIN_SW2, INPUT_PULLUP);
	idButton2 = ButtonInit(PIN_SW3, INPUT_PULLUP);

	idTimer = timerGetId();

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

/* Función que se llama constantemente en un ciclo infinito */

void App_Run(void) {
	static uint8_t balizaOn = 0;
	static uint8_t balizaColor = RED;

	const ButtonEvent *evsw1 = ButtonGetEvent(idButton1);
	const ButtonEvent *evsw2 = ButtonGetEvent(idButton2);

	while ((*evsw1) != EOQ) {
		switch (*evsw1) {
		case PRESS:
			if (balizaOn) {
				turnOffLeds();
			} else {
				selectLedColor(balizaColor);
			}
			balizaOn = !balizaOn;
			break;
		default:
			break;
		}
		evsw1++;
	}

	while ((*evsw2) != EOQ) {
		switch (*evsw2) {
		case PRESS:
			if (++balizaColor == CANT_COLORS) {
				balizaColor = RED;
			}
			if (balizaOn) {
				selectLedColor(balizaColor);
			}
			break;
		default:
			break;
		}
		evsw2++;
	}

	led_poll();
	pwm_poll();
}

void selectLedColor(uint8_t balizaColor) {

	turnOffLeds();
	led_configure_brightness(idLedGreen, 20);
	led_configure_brightness(idLedRed, 20);
	led_configure_brightness(idLedBlue, 20);

	switch (balizaColor) {
	case (RED):
		led_flash(idLedRed);
		break;
	case (ORANGE):
		led_flash(idLedRed);
		led_configure_brightness(idLedBlue, 2);
		led_configure_brightness(idLedGreen, 15);
		led_configure_brightness(idLedRed, 35);
		led_flash(idLedBlue);
		led_flash(idLedGreen);
		break;
	case (YELLOW):
		led_configure_brightness(idLedBlue, 2);
		led_configure_brightness(idLedGreen, 25);
		led_configure_brightness(idLedRed, 30);
		led_flash(idLedBlue);
		led_flash(idLedGreen);
		break;
	case (GREEN):
		led_flash(idLedGreen);
		break;
	case (BLUE):
		led_flash(idLedBlue);
		break;
	case (PURPLE):
		led_flash(idLedBlue);
		led_flash(idLedRed);
		break;
	default:
		break;
	}
}

void turnOffLeds(void) {
	led_set_state(idLedRed, LOW);
	led_set_state(idLedBlue, LOW);
	led_set_state(idLedGreen, LOW);
}

