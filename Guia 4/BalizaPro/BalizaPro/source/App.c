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

void toggleColor(uint8_t a);

void App_Init(void) {
	//Interrupciones de pines

	timerInit();
	led_init_driver();

	idButton1 = initButton(PIN_SW2, INPUT_PULLUP);
	idButton2 = initButton(PIN_SW3, INPUT_PULLUP);

	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, HIGH);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_GREEN, HIGH);

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

	Button_Event evsw1 = getButtonEvent(idButton1);
	Button_Event evsw2 = getButtonEvent(idButton2);

	if (evsw1.inst_evs == PRESS) {
		if (balizaOn) {
			led_set_state(idLedRed, LOW);
			led_set_state(idLedBlue, LOW);
			led_set_state(idLedGreen, LOW);
		} else {
			toggleColor(balizaColor);
		}

		balizaOn = !balizaOn;
	}

	if (evsw2.inst_evs == PRESS) {
		if (++balizaColor == CANT_COLORS) {
			balizaColor = RED;
		}
		if (balizaOn) {
			toggleColor(balizaColor);
		}
	}

	led_poll();
	pwm_poll();
}

void toggleColor(uint8_t balizaColor) {
	switch (balizaColor) {
	case (RED):
		led_flash(idLedRed);
		led_set_state(idLedBlue, LOW);
		break;
	case (ORANGE):
		led_flash(idLedRed);

		led_configure_brightness(idLedBlue, 2);
		led_configure_brightness(idLedGreen, 2);

		led_flash(idLedBlue);
		led_flash(idLedGreen);
		break;
	case (YELLOW):
		led_configure_brightness(idLedBlue, 20);
		led_configure_brightness(idLedGreen, 20);
		led_set_state(idLedRed, LOW);
		led_flash(idLedBlue);
		led_flash(idLedGreen);
		break;
	case (GREEN):
		led_set_state(idLedBlue, LOW);
		led_flash(idLedGreen);
		break;
	case (BLUE):
		led_set_state(idLedGreen, LOW);
		led_flash(idLedBlue);
		break;
	case (PURPLE):
		led_flash(idLedBlue);
		led_set_state(idLedGreen, LOW);
		led_flash(idLedRed);
		break;
	default:
		break;
	}
}

