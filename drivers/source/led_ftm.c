/*
 * led.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */


#include "drivers/headers/led.h"
#include "drivers/headers/PWM.h"
#include "drivers/headers/gpio.h"

/*******************************************************************************
 * GLOBAL VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/

typedef struct {
	double brightness;	//brightness, values from 0 to 1
	uint32_t dt; //from 0 to 100
	uint32_t curr_dt;
	uint32_t time_start;//time when the LED started waiting
	uint32_t time;		//time the LED will stay on the new state in ms. 0 means forever.
	uint32_t period;	//period for flashing LEDs in ms.
	uint32_t flashes;	//times the LED will flash in flashing mode. 0 means forever.
	uint32_t fade;		//time the LED will take to fully change states on a fade in ms. 0 means no fade.
	int8_t led_id; 	//ID of the initialized LED
	int8_t pwm_id;
	uint8_t led_pin;	//pin of the LED
	uint8_t led_pin_mode;//pin mode of the LED
	uint8_t flashing;
	uint8_t state;
	uint8_t curr_flashes;
	uint8_t gpiomode;
}led_t;

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
		timerInit();
		timer_id = timerGetId();	//genero una base de tiempo
		if(timer_id != TIMER_INVALID_ID){
			timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(LED_TIMEBASE), TIM_MODE_PERIODIC, timer_callback);
		}
	}
}

int8_t led_init_led(uint8_t port, uint8_t num, uint8_t pin_mode){

	int8_t id = led_get_id();
	int8_t pwm_id;
	if(id != LED_NO_SPACE){		//valores iniciales importantes
		LEDS[id].brightness = 0.5;
		LEDS[id].dt = 50;
		LEDS[id].led_id = id;
		LEDS[id].led_pin = PORTNUM2PIN(port, num);
		LEDS[id].led_pin_mode = pin_mode;
		pwm_id = PWMInitSignal(port, num, 70, 0.5, pin_mode);		//genero una pwm para el led

		if(pwm_id != PWM_NO_SPACE){
			LEDS[id].pwm_id = pwm_id;
			PWMStopSignal(pwm_id, unnormalize_state(LED_OFF, pin_mode));
		}
		else{
			LEDS[id].gpiomode = 1;
			gpioMode(LEDS[id].led_pin, OUTPUT);
			gpioWrite(LEDS[id].led_pin, unnormalize_state(LED_OFF, pin_mode));
		}
	}
	else{
		id = LED_NO_SPACE;
	}
	return id;

}

void led_destroy_led(int8_t led_id){
	if(led_id < MAX_LEDS){
		INITIALIZED_LEDS[led_id] = 0;		//libero espacio
		if(!LEDS[led_id].gpiomode){
			PWMDestroySignal(LEDS[led_id].pwm_id, unnormalize_state(LED_OFF, LEDS[led_id].led_pin_mode));  //destruyo dependencias
		}
		else{
			gpioWrite(LEDS[led_id].led_pin, unnormalize_state(LED_OFF, LEDS[led_id].led_pin_mode));
		}
	}
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

void led_configure_brightness(int8_t led_id, double brightness){
	uint8_t ok;
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id] && (!LEDS[led_id].gpiomode)){
			if(brightness >= 0 && brightness <= 1){
				LEDS[led_id].brightness = brightness;
				ok = PWMSetDT(LEDS[led_id].pwm_id, brightness);
			}
		}
	}
}

void led_configure_time(int8_t led_id, uint32_t time){

	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].time = time;
		}
	}
}

void led_configure_period(int8_t led_id, uint32_t period){
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].period = period;
		}
	}
}

void led_configure_flashes(int8_t led_id, uint32_t flashes){
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].flashes = flashes;
		}
	}
}

void led_configure_fade(int8_t led_id, uint32_t fade){
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id] && (!LEDS[led_id].gpiomode)){
			LEDS[led_id].fade = fade;
		}
	}
}

void led_configure_dt(int8_t led_id, uint8_t dt){
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].dt = dt;
		}
	}
}

void led_set_state(int8_t led_id, uint8_t norm_state){
	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){			//si esta inicializado el led
			LEDS[led_id].flashing = 0;

			switch(norm_state){
			case LED_OFF:
				if(!LEDS[led_id].gpiomode)
					PWMStopSignal(LEDS[led_id].pwm_id, unnormalize_state(LED_OFF, LEDS[led_id].led_pin_mode));
				else
					gpioWrite(LEDS[led_id].led_pin, unnormalize_state(LED_OFF, LEDS[led_id].led_pin_mode));
				LEDS[led_id].state = LED_OFF;
				break;
			case LED_ON:
				if(!LEDS[led_id].gpiomode)
					PWMStartSignal(LEDS[led_id].pwm_id);
				else
					gpioWrite(LEDS[led_id].led_pin, unnormalize_state(LED_ON, LEDS[led_id].led_pin_mode));
				LEDS[led_id].state = LED_ON;
				break;
			case LED_TOGGLE:
				if(LEDS[led_id].state){
					if(!LEDS[led_id].gpiomode)
						PWMStopSignal(LEDS[led_id].pwm_id, unnormalize_state(LED_OFF, LEDS[led_id].led_pin_mode));
					else
						gpioWrite(LEDS[led_id].led_pin, unnormalize_state(LED_OFF, LEDS[led_id].led_pin_mode));
					LEDS[led_id].state = LED_OFF;
				}
				else{
					if(!LEDS[led_id].gpiomode)
						PWMStartSignal(LEDS[led_id].pwm_id);
					else
						gpioWrite(LEDS[led_id].led_pin, unnormalize_state(LED_ON, LEDS[led_id].led_pin_mode));
					LEDS[led_id].state = LED_ON;
				}
				break;
			default: break;
			}

			if(LEDS[led_id].time){
				LEDS[led_id].time_start = led_timer;	//si time != 0 entonces guardo cuando empieza a contar
				LED_TIMERS[led_id] = 1;
			}
		}
	}
}

void led_flash(int8_t led_id){

	if(led_id >= 0 && led_id < MAX_LEDS){
		if(INITIALIZED_LEDS[led_id]){
			LEDS[led_id].curr_flashes = 0;		//reset contador de flashes
			LEDS[led_id].flashing = 1;			//levanto flag de flash
			LEDS[led_id].time_start = led_timer;
			LED_TIMERS[led_id] = 1;
			if(!LEDS[led_id].gpiomode)
				PWMStartSignal(LEDS[led_id].pwm_id);	//inicio la pwm
			else
				gpioWrite(LEDS[led_id].led_pin, unnormalize_state(LED_ON, LEDS[led_id].led_pin_mode));
			LEDS[led_id].state = LED_ON;
			if (LEDS[led_id].flashes != 0)
				LEDS[led_id].curr_flashes += 1;		//y ya registro el primer flash
		}
	}
}

void led_poll(void){
	uint8_t i;
	for(i = 0; i<MAX_LEDS; i++){ 	//De todos los leds
		if(INITIALIZED_LEDS[i]){ 	//si alguno esta inicializado
			if(LED_TIMERS[i]){  	//si alguno esta esperando
				if(LEDS[i].flashing){	//si esta en modo flash
					if(LEDS[i].state == LED_ON){	//si esta prendido
						if( (float)(led_timer) - (float)(LEDS[i].time_start) > (((float)((((float)LEDS[i].period))*(((float)(100-LEDS[i].dt))/100.0)))/((float)LED_TIMEBASE))){
							if(!LEDS[i].gpiomode)
								PWMStopSignal(LEDS[i].pwm_id, unnormalize_state(LED_OFF, LEDS[i].led_pin_mode));
							else
								gpioWrite(LEDS[i].led_pin, unnormalize_state(LED_OFF, LEDS[i].led_pin_mode));
							LEDS[i].state = LED_OFF;
							LEDS[i].time_start = led_timer; //vuelvo a tomar el tiempo
						}
					}
					else{	//si esta apagado
						if( (float)(led_timer) - (float)(LEDS[i].time_start) > (((float)((((float)LEDS[i].period))*(((float)(LEDS[i].dt))/100.0)))/((float)LED_TIMEBASE))){
							if((LEDS[i].flashes == 0) || (LEDS[i].flashes - LEDS[i].curr_flashes > 0)){ //si aun debo hacer mas flashes
								if(!LEDS[i].gpiomode)
									PWMStartSignal(LEDS[i].pwm_id);
								else
									gpioWrite(LEDS[i].led_pin, unnormalize_state(LED_ON, LEDS[i].led_pin_mode));
								LEDS[i].state = LED_ON;
								LEDS[i].time_start = led_timer; //vuelvo a tomar el tiempo
								if (LEDS[i].flashes != 0)
									LEDS[i].curr_flashes += 1;	//registro otro flash
							}
							else{
								LED_TIMERS[i] = 0; //si ya no me quedan mas flashes, registro que el led ya no esta esperando para prenderse/apagarse
							}
						}
					}
				}
				else if (LEDS[i].time != 0)
				{ //si no esta en modo flash
					if(((float)(led_timer)) - ((float)(LEDS[i].time_start)) > (((float)(LEDS[i].time))/((float)(LED_TIMEBASE)))){ //si ya paso el tiempo
						if(LEDS[i].state){
							if(!LEDS[i].gpiomode)
								PWMStopSignal(LEDS[i].pwm_id, unnormalize_state(LED_OFF, LEDS[i].led_pin_mode));
							else
								gpioWrite(LEDS[i].led_pin, unnormalize_state(LED_OFF, LEDS[i].led_pin_mode));
						}
						else{
							if(!LEDS[i].gpiomode)
								PWMStartSignal(LEDS[i].pwm_id);
							else
								gpioWrite(LEDS[i].led_pin, unnormalize_state(LED_ON, LEDS[i].led_pin_mode));
						}
						LEDS[i].state = !(LEDS[i].state);
						LED_TIMERS[i] = 0; //y marco que ya no debe cambiar con el tiempo
					}
				}
			}
		}
	}
}