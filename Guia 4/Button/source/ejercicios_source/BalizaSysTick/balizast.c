/*
 * balizast.c
 *
 *  Created on: 16 sep. 2020
 *      Author: Msrt
 */
#include "header/ejercicios_headers/BalizaSysTick/balizast.h"

void do_baliza_systick(void){
	//static bool baliza_state = false;
	int prev;
	static int current = HIGH;
	static bool baliza_state = false;
	prev = current;
	current = gpioRead(PIN_SW3);//gpioRead(PIN_SW2);//gpioRead(PIN_SW3);

	if ( current==HIGH && prev==LOW ){
		//delayLoop(40000UL); #Para
		baliza_state = !baliza_state;
		gpioWrite(PIN_LED_RED, !baliza_state);
		gpioWrite(PIN_LED_BLUE, !baliza_state);
		gpioWrite(PIN_LED_AMA_EXT, !baliza_state);
	}

	if (baliza_state == true){
		gpioToggle(PIN_LED_BLUE);
		gpioToggle(PIN_LED_AMA_EXT);
	}

}
