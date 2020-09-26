/*
 * led.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */


#include "headers/led.h"

/*******************************************************************************
 * GLOBAL VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
static led_t LEDS[16];
static uint8_t INITIALIZED_LEDS[16];
static uint8_t LED_TIMERS[16];
static uint8_t timer_id;
static uint32_t led_timer;
static uint8_t init;

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
void led_init_driver(void){
	if(!init){
		init = 1;					//me aseguro que no se haya inicializado ya
		pwm_init_driver();			//inicializo dependencias
		timerInit();
		timer_id = timerGetId();	//genero una base de tiempo
		if(timer_id != TIMER_INVALID_ID){
			timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(LED_TIMEBASE), TIM_MODE_PERIODIC, timer_callback);
		}
	}
}

int8_t led_init_led(pin_t pin, uint8_t pin_mode){

	int8_t id = led_get_id();
	int8_t pwm_id;
	if(id != UNAVAILABLE_SPACE){		//valores iniciales importantes
		LEDS[id].brightness = 100;
		LEDS[id].dt = 50;
		LEDS[id].led_id = id;
		LEDS[id].led_pin = pin;
		LEDS[id].led_pin_mode = pin_mode;
		pwm_id = pwm_init_signal(pin);		//genero una pwm para el led
		if(pwm_id != UNAVAILABLE_SPACE){
			LEDS[id].pwm_id = pwm_id;
		}
		else{
			id = UNAVAILABLE_SPACE;
		}
	}
	else{
		id = UNAVAILABLE_SPACE;
	}
	return id;

}

void led_destroy_led(int8_t led_id){
#if (DEVELOPMENT_MODEE == 1)
	if(led_id < MAX_LEDS){
		INITIALIZED_LEDS[led_id] = 0;		//libero espacio
		pwm_destroy(LEDS[led_id].pwm_id);  //destruyo dependencias
	}
#else
	INITIALIZED_LEDS[led_id] = 0;
	pwm_destroy(LEDS[led_id].pwm_id);
#endif
}

int8_t led_get_id(void){
	uint8_t i;
	uint8_t found_space = 0;
	int8_t id = -1;
	for(i = 0; i < MAX_LEDS; i++){		//me fijo si hay espacio
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

void led_configure_brightness(int8_t led_id, uint8_t brightness){

#if (DEVELOPMENT_MODEE == 1)
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

void led_configure_time(int8_t led_id, uint32_t time){
#if (DEVELOPMENT_MODEE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].time = time;
		}
	}
#else
	LEDS[led_id].time = time;
#endif
}

void led_configure_period(int8_t led_id, uint32_t period){
#if (DEVELOPMENT_MODEE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].period = period;
		}
	}
#else
	LEDS[led_id].period = period;
#endif
}

void led_configure_flashes(int8_t led_id, uint32_t flashes){
#if (DEVELOPMENT_MODEE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].flashes = flashes;
		}
	}
#else
	LEDS[led_id].flashes = flashes;
#endif
}

void led_configure_fade(int8_t led_id, uint32_t fade){
#if (DEVELOPMENT_MODEE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].fade = fade;
		}
	}
#else
	LEDS[led_id].fade = fade;
#endif
}

void led_configure_dt(int8_t led_id, uint8_t dt){
#if (DEVELOPMENT_MODEE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].dt = dt;
		}
	}
#else
	LEDS[led_id].dt = dt;
#endif
}

void led_set_state(int8_t led_id, uint8_t norm_state){
#if (DEVELOPMENT_MODEE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){			//si esta inicializado el led
			if(norm_state){
				pwm_query(LEDS[led_id].pwm_id, PWM_FREQ, LEDS[led_id].brightness, HIGH); //si me piden prender el led, activo la pwm
			}
			else{
				pwm_unquery(LEDS[led_id].pwm_id, unnormalize_state(LOW, LEDS[led_id].led_pin_mode)); //si me piden apagar, desactivo la pwm
			}
			LEDS[led_id].state = norm_state;	//guardo el estao
			if(LEDS[led_id].time){
				LEDS[led_id].time_start = led_timer;	//si time != 0 entonces guardo cuando empieza a contar
				LED_TIMERS[led_id] = 1;
			}
		}
	}
#else
	if(norm_state){
		pwm_query(LEDS[led_id].pwm_id, PWM_FREQ, LEDS[led_id].brightness, HIGH);
	}
	else{
		pwm_unquery(LEDS[led_id].pwm_id, unnormalize_state(LOW, LEDS[led_id].led_pin_mode));
	}
	LEDS[led_id].state = norm_state;
	if(LEDS[led_id].time){
		LEDS[led_id].time_start = led_timer;	//si time != 0 entonces guardo cuando empieza a contar
		LED_TIMERS[led_id] = 1;
	}
#endif
}

void led_flash(int8_t led_id){
	uint8_t cur_norm_state;
#if (DEVELOPMENT_MODEE == 1)
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].curr_flashes = 0;		//reset contador de flashes
			LEDS[led_id].flashing = 1;			//levanto flag de flash
			LEDS[led_id].time_start = led_timer;
			LED_TIMERS[led_id] = 1;
			pwm_query(LEDS[led_id].pwm_id, PWM_FREQ, LEDS[led_id].brightness, HIGH);	//inicio la pwm
			LEDS[led_id].state = HIGH;
			LEDS[led_id].curr_flashes += 1;		//y ya registro el primer flash
		}
	}

#else
	LEDS[led_id].curr_flashes = 0;
	LEDS[led_id].flashing = 1;
	LEDS[led_id].time_start = led_timer;
	LED_TIMERS[led_id] = 1;
	cur_norm_state = get_normalized_state(gpioRead(LEDS[led_id].led_pin), LEDS[led_id].led_pin_mode);
	if(!cur_norm_state){
		pwm_query(LEDS[led_id].pwm_id, PWM_FREQ, LEDS[led_id].brightness, HIGH);
	}
	else{
		pwm_unquery(LEDS[led_id].pwm_id, unnormalize_state(LOW, LEDS[led_id].led_pin_mode));
	}
	LEDS[led_id].state = !cur_norm_state;
	LEDS[led_id].curr_flashes += 1;
#endif
}

void led_poll(void){
	uint8_t i;
	uint8_t cur_norm_state;
	for(i = 0; i<MAX_LEDS; i++){ 	//De todos los leds
		if(INITIALIZED_LEDS[i]){ 	//si alguno esta inicializado
			if(LED_TIMERS[i]){  	//si alguno esta esperando
				if(LEDS[i].flashing){	//si esta en modo flash
					if(LEDS[i].state == HIGH){	//si esta prendido
						if( (float)(led_timer) - (float)(LEDS[i].time_start) > (((float)((((float)LEDS[i].period))*(((float)(100-LEDS[i].dt))/100.0)))/((float)LED_TIMEBASE))){
							pwm_unquery(LEDS[i].pwm_id, unnormalize_state(LOW, LEDS[i].led_pin_mode)); //si ya paso el tiempo, apago la pwm
							LEDS[i].state = LOW;
							LEDS[i].time_start = led_timer; //vuelvo a tomar el tiempo
						}
					}
					else{	//si esta apagado
						if( (float)(led_timer) - (float)(LEDS[i].time_start) > (((float)((((float)LEDS[i].period))*(((float)(LEDS[i].dt))/100.0)))/((float)LED_TIMEBASE))){
							if(LEDS[i].flashes - LEDS[i].curr_flashes > 0){ //si aun debo hacer mas flashes
								pwm_query(LEDS[i].pwm_id, PWM_FREQ, LEDS[i].brightness, HIGH); //si ya paso el tiempo, activo la pwm
								LEDS[i].state = HIGH;
								LEDS[i].time_start = led_timer; //vuelvo a tomar el tiempo
								LEDS[i].curr_flashes += 1;	//registro otro flash
							}
							else{
								LED_TIMERS[i] = 0; //si ya no me quedan mas flashes, registro que el led ya no esta esperando para prenderse/apagarse
							}
						}
					}
				}
				else{ //si no esta en modo flash
					if(((float)(led_timer)) - ((float)(LEDS[i].time_start)) > (((float)(LEDS[i].time))/((float)(LED_TIMEBASE)))){ //si ya paso el tiempo
						cur_norm_state = get_normalized_state(gpioRead(LEDS[i].led_pin), LEDS[i].led_pin_mode);
						if(!cur_norm_state){
							pwm_query(LEDS[i].pwm_id, PWM_FREQ, LEDS[i].brightness, HIGH); //si lo que habia hecho era apagarlo, lo vuelvo a prender un tiempo @time despues
						}
						else{
							pwm_unquery(LEDS[i].pwm_id, unnormalize_state(LOW, LEDS[i].led_pin_mode)); //idem si lo que habia hecho fue prenderlo
						}
						LEDS[i].state = !cur_norm_state;
						LED_TIMERS[i] = 0; //y marco que ya no debe cambiar con el tiempo
					}
				}
			}
		}
	}
}
