/*
 * led.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */


#include "header/led.h"
//#include "header/timer.h"
//#include "header/pwm.h"
#include "header/gpio.h"

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
 * @param pin_mode whether the pin is pullup or pulldown
 */
bool get_normalized_state(bool state, uint8_t pin_mode);
bool unnormalize_state(bool norm_state, uint8_t pin_mode);
void timer_callback();
/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/
bool get_normalized_state(bool state, uint8_t pin_mode){
	if(pin_mode == INPUT_PULLUP){
		state = !state;
	}
	return state;
}

bool unnormalize_state(bool norm_state, uint8_t pin_mode){
	if(pin_mode == INPUT_PULLUP){
		norm_state = !norm_state;
	}
	return norm_state;
}
void timer_callback(){
	led_timer++;
}
/*******************************************************************************
 * FUNCTION DEFINITIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void init_driver(){
	uint8_t i = 0;
	INITIALIZED_LEDS[i] = 0;
	LED_TIMERS[i] = 0;
	timer_id = timerGetId();//pongo un timer singleshot con @time
	if(timer_id != TIMER_INVALID_ID){
		timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(25), TIM_MODE_PERIODIC, timer_callback); //25ms es 40Hz, una burrada ya
	}
}

int8_t init_led(pin_t pin, uint8_t pin_mode, int8_t led_id){

	int8_t id = get_id();
	if(id != UNAVAILABLE_SPACE){
		gpioMode(pin, pin_mode);
		LEDS[id].brightness = 100;
		LEDS[id].time = 0;
		LEDS[id].period = 0;
		LEDS[id].flashes = 0;
		LEDS[id].fade = 0;
		LEDS[id].led_id = id;
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

int8_t get_id(){
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

void configure_time(int8_t led_id, uint16_t time){
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

void configure_period(int8_t led_id, uint16_t period){
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

void configure_flashes(int8_t led_id, uint16_t flashes){
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

void configure_fade(int8_t led_id, uint16_t fade){
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
	tim_id_t timer;
	led_t led;
	uint8_t cur_norm_state;
#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){


			led = LEDS[led_id];
			gpioWrite(led.led_pin, unnormalize_state(norm_state, led.led_pin_mode));//si estaba apagado, lo prendo
			if(led.time){
				led.time_start = led_timer;	//si time != 0 entonces guardo cuando empieza a contar
			}
			if(led.fade){
				//ver que hacer aca
			}
		}
	}
#else
	led = LEDS[led_id];
	gpioWrite(led.led_pin, unnormalize_state(norm_state, led.led_pin_mode));//si estaba apagado, lo prendo
	if(led.time){
		led.time_start = led_timer;	//si time != 0 entonces guardo cuando empieza a contar
	}
	if(led.fade){
		//ver que hacer aca
	}
#endif
}

void flash(int8_t led_id){
	led_t led;
	uint8_t cur_norm_state;
#if (DEVELOPMENT_MODE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			led = LEDS[led_id];
			led.flashing = 1;
			led.time_start = led_timer;
			cur_norm_state = get_normalized_state(gpioRead(led.led_pin), led.led_pin_mode);
			gpioWrite(led.led_pin, unnormalize_state(!cur_norm_state, led.led_pin_mode));
		}
	}

#else
	led = LEDS[led_id];
	led.flashing = 1;
	led.time_start = led_timer;
	cur_norm_state = get_normalized_state(gpioRead(led.led_pin), led.led_pin_mode);
	gpioWrite(led.led_pin, unnormalize_state(!cur_norm_state, led.led_pin_mode));
#endif

void poll_leds(){
	uint8_t i;
	for(i = 0; i<MAX_LEDS; i++){ //De todos los leds
		if(INITIALIZED_LEDS[i]){ //si alguno esta inicializado
			if(LED_TIMERS[i]){  //si alguno esta esperando
				if(LEDS[i].flashing){
					if(LEDS[i].time_start + (uint32_t)(LEDS[i].period/2) > led_timer){
						cur_norm_state = get_normalized_state(gpioRead(LEDS[i].led_pin), LEDS[i].led_pin_mode);
						gpioWrite(LEDS[i].led_pin, unnormalize_state(!cur_norm_state, LEDS[i].led_pin_mode));
						LEDS[i].time_start = led_timer;
					}
				}
				else{
					if(LEDS[i].time_start + LEDS[i].time > led_timer){
						cur_norm_state = get_normalized_state(gpioRead(LEDS[i].led_pin), LEDS[i].led_pin_mode);
						gpioWrite(LEDS[i].led_pin, unnormalize_state(!cur_norm_state, LEDS[i].led_pin_mode));
						LED_TIMERS[i] = 0;
					}
				}
			}
		}
	}
}

}
