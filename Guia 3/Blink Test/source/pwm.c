/*
 * pwm.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */

#include "header/pwm.h"

////////////////////////////////////////////////////////////////////////////////////////////////DEBUG
/////////////ELIMINAR ESTO UNA VEZ QUE TENGAMOS TIMER
////////////////////////////////////////////////////
// Timer Modes
enum { TIM_MODE_SINGLESHOT, TIM_MODE_PERIODIC, CANT_TIM_MODES };

// Timer alias
typedef uint32_t ttick_t;
typedef uint8_t tim_id_t;
typedef void (*tim_callback_t)(void);
#define TIMER_TICK_MS       1
#define TIMER_MS2TICKS(ms)  ((ms)/TIMER_TICK_MS)
#define TIMERS_MAX_CANT     16
#define TIMER_INVALID_ID    255
extern uint8_t timerGetId();
extern void timerStart(tim_id_t id, ttick_t ticks, uint8_t mode, tim_callback_t callback);
////////////////////////////////////////////////////////////////////////////////////////////////

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
static void timer_callback();

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/
void timer_callback(){
	timer++;
}

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void init_pwm_driver(){
	timer_id = timerGetId();
	if(timer_id != TIMER_INVALID_ID){
		timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(PWM_TIMEBASE), TIM_MODE_PERIODIC, timer_callback); //base de tiempo hasta 1kHz
	}
}

int8_t get_pwm_id(){
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

void query(int8_t pwm_id, uint32_t freq, uint32_t dt, uint8_t initial_state){
	if(INITIALIZED_PWMS[pwm_id] == 1){
		PWMS[pwm_id].freq = freq;
		PWMS[pwm_id].dt = dt;
		PWMS[pwm_id].queried = 1;
		PWMS[pwm_id].last_updated = timer;
		gpioWrite(PWMS[pwm_id].pin, initial_state);
	}
}

void unquery(int8_t pwm_id, uint8_t final_state){
	if(INITIALIZED_PWMS[pwm_id] == 1){
		PWMS[pwm_id].queried = 0;
		gpioWrite(PWMS[pwm_id].pin, final_state);
	}
}

void poll_pwms(){
	uint8_t i;
	for(i=0; i<MAX_PWMS; i++){
		if(INITIALIZED_PWMS[i] == 1){
			if(PWMS[i].queried){
				if(gpioRead(PWMS[i].pin)){
					if((float)PWMS[i].last_updated + (((float)(PWMS[i].dt)/100.0)*((float)(PWMS[i].freq)*1000.0))/(float)(PWM_TIMEBASE)){
						gpioToggle(PWMS[i].pin);
						PWMS[i].last_updated = timer;
					}
				}
				else{
					if((float)PWMS[i].last_updated + (((float)((100-PWMS[i].dt))/100.0)*((float)(PWMS[i].freq)*1000.0))/(float)(PWM_TIMEBASE)){
						gpioToggle(PWMS[i].pin);
						PWMS[i].last_updated = timer;
					}
				}
			}
		}
	}
}
