/*
 * encoder_tb.c
 *
 *  Created on: 23 sep. 2020
 *      Author: Msrt
 */

#include <drivers/headers/board.h>
#include <drivers/headers/encoder.h>
#include <drivers/headers/gpio.h>
#include <drivers/headers/timer.h>
#include <stdbool.h>

void encoder_test_init(void){

	encoder_init();

}

static bool warm_up = false;
static tim_id_t id_led;
static encoder_id id_encoder;

void blink_led(void){
	gpioToggle(PIN_LED_RED);
	gpioToggle(PIN_LED_AMA_EXT);

	ttick_t new_ticks = encoder_get_count(id_encoder);
	timerChangePeriod(id_led, new_ticks);
}
void encoder_test_infinite_count_cc(void){

	if(!warm_up){
		id_encoder = encoder_register(PIN_B2, PIN_B3);
		warm_up = true;
		gpioMode(PIN_LED_AMA_EXT, OUTPUT);
		gpioMode(PIN_LED_RED, OUTPUT);
		gpioWrite(PIN_LED_AMA_EXT, LOW);
		gpioWrite(PIN_LED_RED, LOW);
		id_led = timerGetId();
		timerStart(id_led, 20, TIM_MODE_PERIODIC, blink_led);
	}
}

