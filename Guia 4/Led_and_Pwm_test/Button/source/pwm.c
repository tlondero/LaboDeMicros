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
void init_pwm_driver(void){
	timer_id = timerGetId();
	if(timer_id != TIMER_INVALID_ID){
		timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(PWM_TIMEBASE), TIM_MODE_PERIODIC, timer_callback); //base de tiempo hasta 1kHz
	}
}

int8_t get_pwm_id(void){
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

int8_t init_pwm(pin_t pin){
	int8_t id = get_pwm_id();
	if(id != UNAVAILABLE_SPACE){
		PWMS[id].dt = 0;
		PWMS[id].freq = 0;
		PWMS[id].pin = pin;
		PWMS[id].pwm_id = id;
		PWMS[id].queried = 0;
	}
	return id;
}

void query_pwm(int8_t pwm_id, uint32_t freq, uint32_t dt, uint8_t initial_state){
	if(INITIALIZED_PWMS[pwm_id] == 1){
		PWMS[pwm_id].freq = freq;
		PWMS[pwm_id].dt = dt;
		PWMS[pwm_id].queried = 1;
		PWMS[pwm_id].last_updated = timer;
		gpioWrite(PWMS[pwm_id].pin, initial_state);
	}
}

void unquery_pwm(int8_t pwm_id, uint8_t final_state){
	if(INITIALIZED_PWMS[pwm_id] == 1){
		PWMS[pwm_id].queried = 0;
		gpioWrite(PWMS[pwm_id].pin, final_state);
	}
}

void poll_pwms(void){
	uint8_t i;
	for(i=0; i<MAX_PWMS; i++){
		if(INITIALIZED_PWMS[i] == 1){
			if(PWMS[i].queried){
				if(gpioRead(PWMS[i].pin)){
					if(((float)timer) - (float)PWMS[i].last_updated > (((float)(((1.0/((float)PWMS[i].freq)))*1000.0*(((float)(100-PWMS[i].dt))/100.0)))/((float)PWM_TIMEBASE)) ){
						gpioToggle(PWMS[i].pin);
						PWMS[i].last_updated = timer;
					}
				}
				else{
					if(((float)timer) - (float)PWMS[i].last_updated >(((float)(((1.0/((float)PWMS[i].freq)))*1000.0*(((float)(PWMS[i].dt))/100.0)))/((float)PWM_TIMEBASE))){
						gpioToggle(PWMS[i].pin);
						PWMS[i].last_updated = timer;
					}
				}
			}
		}
	}
}
