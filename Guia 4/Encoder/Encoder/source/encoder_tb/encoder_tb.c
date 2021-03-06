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

static bool warm_up = false;
static tim_id_t id_led;
static encoder_id id_encoder;

static uint32_t new_ticks = 1;
void blink_led(void){
	gpioToggle(PIN_LED_RED);
	gpioToggle(DEBUG_PIN);
}

void do_always(void){
	if(EncoderEventAVB(id_encoder) == EVENT_AVB){
		event_t ev = EncoderPopEvent(id_encoder);
		if(ev == RIGHT_TURN){
			new_ticks++;
		}
		else if(ev == LEFT_TURN){
			if(new_ticks>1)
				new_ticks--;
		}
		timerChangePeriod(id_led, new_ticks*15);
	}

}
void encoder_test_infinite_count_cc(void){

	if(!warm_up){

		id_encoder = EncoderRegister(PIN_C3, PIN_C5);


		warm_up = true;
		gpioMode(DEBUG_PIN, OUTPUT);
		gpioMode(PIN_LED_RED, OUTPUT);
		gpioWrite(DEBUG_PIN, LOW);
		gpioWrite(PIN_LED_RED, LOW);
		id_led = timerGetId();
		timerStart(id_led, 15, TIM_MODE_PERIODIC, blink_led);
	}
}

