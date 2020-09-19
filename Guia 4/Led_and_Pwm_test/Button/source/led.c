/*
 * led.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */


#include "header/led.h"

/*******************************************************************************
 * GLOBAL VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
static led_t LEDS[16];
static uint8_t INITIALIZED_LEDS[16];
static uint8_t LED_TIMERS[16];
static uint8_t timer_id;
static uint32_t led_timer;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/
/**
 * @brief Get normalized state of a LED, where the normalized state 1 indicates the LED in ON
 *                and the normalized state 0 indicates the LED is OFF.
 * @param state the electrical state of the pin
 * @param pin_mode whether the led is turned on with 1 or 0
 */
bool get_normalized_state(bool state, uint8_t pin_mode);
bool unnormalize_state(bool norm_state, uint8_t pin_mode);
static void timer_callback(void);
/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/
bool get_normalized_state(bool state, uint8_t pin_mode){
	if(pin_mode == TURNS_ON_WITH_0){
		state = !state;
	}
	return state;
}

bool unnormalize_state(bool norm_state, uint8_t pin_mode){
	if(pin_mode == TURNS_ON_WITH_0){
		norm_state = !norm_state;
	}
	return norm_state;
}
void timer_callback(void){
	led_timer++;
}
/*******************************************************************************
 * FUNCTION DEFINITIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void init_led_driver(void){
	timer_id = timerGetId();
	if(timer_id != TIMER_INVALID_ID){
		timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(LED_TIMEBASE), TIM_MODE_PERIODIC, timer_callback);
	}
}

int8_t init_led(pin_t pin, uint8_t pin_mode){

	int8_t id = get_led_id();
	if(id != UNAVAILABLE_SPACE){
		LEDS[id].brightness = 100;
		LEDS[id].led_id = id;
		LEDS[id].led_pin = pin;
		LEDS[id].led_pin_mode = pin_mode;
	}
	return id;

}

void destroy_led(int8_t led_id){
#if (DEVELOPMENT_MODE == 1)
	if(led_id < MAX_LEDS){
		INITIALIZED_LEDS[led_id] = 0;
	}
#else
	INITIALIZED_LEDS[led_id] = 0;
#endif
}

int8_t get_led_id(void){
	uint8_t i;
	uint8_t found_space = 0;
	int8_t id = -1;
	for(i = 0; i < MAX_LEDS; i++){
		if( !(INITIALIZED_LEDS[i]) ){
			INITIALIZED_LEDS[i] = 1;
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

void configure_brightness(int8_t led_id, uint8_t brightness){

#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			if(brightness >= 0 && brightness <= 100){
				LEDS[led_id].brightness = brightness;
			}
		}
	}
#else
	LEDS[led_id].brightness = brightness;
#endif
}

void configure_time(int8_t led_id, uint32_t time){
#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].time = time;
		}
	}
#else
	LEDS[led_id].time = time;
#endif
}

void configure_period(int8_t led_id, uint32_t period){
#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].period = period;
		}
	}
#else
	LEDS[led_id].period = period;
#endif
}

void configure_flashes(int8_t led_id, uint32_t flashes){
#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].flashes = flashes;
		}
	}
#else
	LEDS[led_id].flashes = flashes;
#endif
}

void configure_fade(int8_t led_id, uint32_t fade){
#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].fade = fade;
		}
	}
#else
	LEDS[led_id].fade = fade;
#endif
}

void set_value(int8_t led_id, uint8_t norm_state){
#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			gpioWrite(LEDS[led_id].led_pin, unnormalize_state(norm_state, LEDS[led_id].led_pin_mode));//si estaba apagado, lo prendo
			if(LEDS[led_id].time){
				LEDS[led_id].time_start = led_timer;	//si time != 0 entonces guardo cuando empieza a contar
				LED_TIMERS[led_id] = 1;
			}
			if(LEDS[led_id].fade){
				//ver que hacer aca
			}
		}
	}
#else
	gpioWrite(LEDS[led_id].led_pin, unnormalize_state(norm_state, LEDS[led_id].led_pin_mode));//si estaba apagado, lo prendo
	if(LEDS[led_id].time){
		LEDS[led_id].time_start = led_timer;	//si time != 0 entonces guardo cuando empieza a contar
		LED_TIMERS[led_id] = 1;
	}
	if(LEDS[led_id].fade){
		//ver que hacer aca
	}
#endif
}

void flash(int8_t led_id){
	uint8_t cur_norm_state;
#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].flashing = 1;
			LEDS[led_id].time_start = led_timer;
			LED_TIMERS[led_id] = 1;
			cur_norm_state = get_normalized_state(gpioRead(LEDS[led_id].led_pin), LEDS[led_id].led_pin_mode);
			gpioWrite(LEDS[led_id].led_pin, unnormalize_state(!cur_norm_state, LEDS[led_id].led_pin_mode));
		}
	}

#else
	LEDS[led_id].flashing = 1;
	LEDS[led_id].time_start = led_timer;
	LED_TIMERS[led_id] = 1;
	cur_norm_state = get_normalized_state(gpioRead(LEDS[led_id].led_pin), LEDS[led_id].led_pin_mode);
	gpioWrite(LEDS[led_id].led_pin, unnormalize_state(!cur_norm_state, LEDS[led_id].led_pin_mode));
#endif
}

void poll_leds(void){
	uint8_t i;
	uint8_t cur_norm_state;
	for(i = 0; i<MAX_LEDS; i++){ //De todos los leds
		if(INITIALIZED_LEDS[i]){ //si alguno esta inicializado
			if(LED_TIMERS[i]){  //si alguno esta esperando
				if(LEDS[i].flashing){
					if( (float)(led_timer) - (float)(LEDS[i].time_start) > (((float)LEDS[i].period/2.0)/((float)LED_TIMEBASE))){
						cur_norm_state = get_normalized_state(gpioRead(LEDS[i].led_pin), LEDS[i].led_pin_mode);
						gpioWrite(LEDS[i].led_pin, unnormalize_state(!cur_norm_state, LEDS[i].led_pin_mode));
						LEDS[i].time_start = led_timer;
					}
				}
				else{
					if(((float)(led_timer)) - ((float)(LEDS[i].time_start)) > (((float)(LEDS[i].time))/((float)(LED_TIMEBASE)))){
						cur_norm_state = get_normalized_state(gpioRead(LEDS[i].led_pin), LEDS[i].led_pin_mode);
						gpioWrite(LEDS[i].led_pin, unnormalize_state(!cur_norm_state, LEDS[i].led_pin_mode));
						LED_TIMERS[i] = 0;
					}
				}
			}
		}
	}
}
