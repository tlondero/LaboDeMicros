/*
 * pwm.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */

#include "header/pwm.h"

/*******************************************************************************
 * GLOBAL VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
static uint8_t timer_id;
static uint32_t timer;
static uint8_t INITIALIZED_PWMS[MAX_PWMS];
static pwm_t PWMS[MAX_PWMS];
static uint8_t init;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/
static void timer_callback(void);

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/
void timer_callback(void){
	timer++;
}

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void pwm_init_driver(void){
	if(!init){
		init = 1;
		timer_id = timerGetId();
		if(timer_id != TIMER_INVALID_ID){
			timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(PWM_TIMEBASE), TIM_MODE_PERIODIC, timer_callback); //base de tiempo hasta 1kHz
		}
	}
}

int8_t pwm_get_id(void){
	uint8_t i;
	uint8_t found_space = 0;
	int8_t id = -1;
	for(i = 0; i < MAX_PWMS; i++){
		if( !(INITIALIZED_PWMS[i]) ){
			INITIALIZED_PWMS[i] = 1;
			found_space = 1;
			break;
		}
	}
	if(found_space){
		id = i;
	}
	else{
		id = -1;
	}
	return id;
}

int8_t pwm_init_signal(pin_t pin){
	int8_t id = pwm_get_id();
	if(id != UNAVAILABLE_SPACE){
		PWMS[id].dt = 0;
		PWMS[id].freq = 0;
		PWMS[id].pin = pin;
		PWMS[id].pwm_id = id;
		PWMS[id].queried = 0;
	}
	return id;
}

void pwm_query(int8_t pwm_id, uint32_t freq, uint32_t dt, uint8_t initial_state){
	if(INITIALIZED_PWMS[pwm_id] == 1){
		PWMS[pwm_id].freq = freq;
		PWMS[pwm_id].dt = dt;
		PWMS[pwm_id].queried = 1;
		PWMS[pwm_id].last_updated = timer;
		gpioWrite(PWMS[pwm_id].pin, initial_state);
	}
}

void pwm_unquery(int8_t pwm_id, uint8_t final_state){
	if(INITIALIZED_PWMS[pwm_id] == 1){
		PWMS[pwm_id].queried = 0;
		gpioWrite(PWMS[pwm_id].pin, final_state);
	}
}

void pwm_poll(void){
	uint8_t i;
	uint32_t start, actual, wait;
	for(i=0; i<MAX_PWMS; i++){
		if(INITIALIZED_PWMS[i] == 1){
			if(PWMS[i].queried){
				if(gpioRead(PWMS[i].pin)){
					actual = ((float)timer);
					start = (float)PWMS[i].last_updated;
					wait = (((float)(((1.0/((float)PWMS[i].freq)))*1000.0*(((float)(100-PWMS[i].dt))/100.0)))/((float)PWM_TIMEBASE));
					if(actual - start > wait ){
						gpioToggle(PWMS[i].pin);
						PWMS[i].last_updated = timer;
					}
				}
				else{

					actual = ((float)timer);
					start = (float)PWMS[i].last_updated;
					wait = 	(((float)(((1.0/((float)PWMS[i].freq)))*1000.0*(((float)(PWMS[i].dt))/100.0)))/((float)PWM_TIMEBASE));
					//wait = (((float)(((1.0/((float)PWMS[i].freq)))*1000.0*(((float)(PWMS[i].dt))/100.0)))/((float)PWM_TIMEBASE));
					if(actual - start > wait){
						gpioToggle(PWMS[i].pin);
						PWMS[i].last_updated = timer;
					}
				}
			}
		}
	}
}
