/*
 * led.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */

#include "../headers/led_old.h"
#include "../headers/pwm_old.h"
#include "../headers/gpio.h"
#include "../headers/debug.h"
#include "../headers/board.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
	uint32_t time_start;
	uint32_t time;		//time the LED will stay on the new state in ms. 0 means forever.
	uint32_t period;	//period for flashing LEDs in ms.
	uint32_t flashes;	//times the LED will flash in flashing mode. 0 means forever.
	uint32_t fade;		//time the LED will take to fully change states on a fade in ms. 0 means no fade.
	int8_t led_id; 	//ID of the initialized LED
	int8_t pwm_id;
	uint8_t led_pin;	//pin of the LED
	uint8_t led_activation_mode;//pin mode of the LED
	uint8_t brightness;	//brightness, values from 0 to 100 and has a 1:1 ratio with duty cycle
	uint8_t flashing;
	uint8_t dt;
	uint32_t curr_dt;
	uint8_t state;
	uint8_t curr_flashes;
}led_t;

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
 * @param activation_mode whether the led is turned on with 1 or 0. Use the provided defines.
 */
bool get_normalized_state(bool state, uint8_t activation_mode);

/**
 * @brief Get the unnormalized state of a LED, where the normalized state 1 indicates the LED in ON
 *                and the normalized state 0 indicates the LED is OFF. This is taking into consideration
 *                that the LED can be turned on with either a 1 or 0.
 * @param state the electrical state of the pin
 * @param activation_mode whether the led is turned on with 1 or 0. Use the provided defines.
 */
bool unnormalize_state(bool norm_state, uint8_t activation_mode);

static void timer_callback(void);

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/
bool get_normalized_state(bool state, uint8_t activation_mode)
{
	if (activation_mode == TURNS_ON_WITH_0)
	{
		state = !state;
	}
	return state;
}

bool unnormalize_state(bool norm_state, uint8_t activation_mode)
{
	if (activation_mode == TURNS_ON_WITH_0)
	{
		norm_state = !norm_state;
	}
	return norm_state;
}
void timer_callback(void)
{
#if DEBUGGIN_MODE_LED
	gpioWrite(DEBUG_PIN, HIGH);
#endif

		led_timer++;
		
#if DEBUGGIN_MODE_LED
	gpioWrite(DEBUG_PIN, LOW);
#endif
}
/*******************************************************************************
 * FUNCTION DEFINITIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void led_init_driver(void)
{
#if DEBUGGIN_MODE_LED == 1
	gpioMode(DEBUG_PIN, OUTPUT);
	gpioWrite(DEBUG_PIN, LOW);
#endif
	if (!init)
	{
		init = 1;		   //me aseguro que no se haya inicializado ya
		pwm_init_driver(); //inicializo dependencias
		timerInit();
		timer_id = timerGetId(); //genero una base de tiempo
		if (timer_id != TIMER_INVALID_ID)
		{
			timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(LED_TIMEBASE), TIM_MODE_PERIODIC, timer_callback);
		}
	}
}

int8_t led_init_led(pin_t pin, uint8_t activation_mode)
{

	int8_t id = led_get_id();									//Pido un ID
	gpioMode(pin, OUTPUT);
	gpioWrite(pin, unnormalize_state(LOW, activation_mode));	//configuro el pin
	int8_t pwm_id;
	if (id != UNAVAILABLE_SPACE)
	{ 															//configuro valores iniciales importantes != 0.
		LEDS[id].brightness = 100;
		LEDS[id].dt = 50;
		LEDS[id].led_id = id;
		LEDS[id].led_pin = pin;
		LEDS[id].led_activation_mode = activation_mode;
		pwm_id = pwm_init_signal(pin); 							//genero una pwm para el led
		if (pwm_id != UNAVAILABLE_SPACE)
		{
			LEDS[id].pwm_id = pwm_id;
		}
		else
		{
			id = UNAVAILABLE_SPACE;
		}
	}
	else
	{
		id = UNAVAILABLE_SPACE;
	}
	return id;
}

void led_destroy_led(int8_t led_id)
{
	if (led_id < MAX_LEDS)
	{
		INITIALIZED_LEDS[led_id] = 0;	  //libero espacio
		pwm_destroy(LEDS[led_id].pwm_id); //destruyo dependencias
	}
}

int8_t led_get_id(void)
{
	uint8_t i;
	uint8_t found_space = 0;
	int8_t id = -1;
	for (i = 0; i < MAX_LEDS; i++)
	{ 										//me fijo si hay espacio
		if (!(INITIALIZED_LEDS[i]))
		{
			INITIALIZED_LEDS[i] = 1;
			found_space = 1;
			break;
		}
	}
	if (found_space)
	{
		id = i;
	}
	else
	{
		id = -1;
	}
	return id;
}

void led_configure_brightness(int8_t led_id, uint8_t brightness)
{
	if (led_id >= 0 && led_id < MAX_LEDS)
	{
		if (INITIALIZED_LEDS[led_id])
		{
			if (brightness >= 0 && brightness <= 100)
			{
				if (LEDS[led_id].led_activation_mode == TURNS_ON_WITH_0)		//Si el led se prende con 0
				{
					LEDS[led_id].brightness = brightness;
					if (LEDS[led_id].state)										//Si estaba prendido, reinicio la pwm para ver los efectos
					{
						if (LEDS[led_id].brightness)
						{
							pwm_query(LEDS[led_id].pwm_id, PWM_FREQ, LEDS[led_id].brightness, HIGH);
						}
						else		//Si el brightness vale cero, apago el led
						{
							pwm_unquery(LEDS[led_id].pwm_id, unnormalize_state(LOW, LEDS[led_id].led_activation_mode));
						}
					}
				}
				else								//Si el led se prende con 1
				{
					LEDS[led_id].brightness = 100 - brightness;		//Por como se hacen las cuentas en poll, invierto el brightness
					if (LEDS[led_id].state)
					{
						if (LEDS[led_id].brightness == 100)//Si el brightness vale cien, apago el led
						{
							pwm_unquery(LEDS[led_id].pwm_id, unnormalize_state(LOW, LEDS[led_id].led_activation_mode));
						}
						else
						{
							pwm_query(LEDS[led_id].pwm_id, PWM_FREQ, LEDS[led_id].brightness, HIGH);
						}
					}
				}
			}
		}
	}
}

void led_configure_time(int8_t led_id, uint32_t time)
{
	if (led_id >= 0 && led_id < MAX_LEDS)
	{
		if (INITIALIZED_LEDS[led_id])
		{
			LEDS[led_id].time = time;
		}
	}
}

void led_configure_period(int8_t led_id, uint32_t period)
{
	if (led_id >= 0 && led_id < MAX_LEDS)
	{
		if (INITIALIZED_LEDS[led_id])
		{
			LEDS[led_id].period = period;
		}
	}
}

void led_configure_flashes(int8_t led_id, uint32_t flashes)
{
	if (led_id >= 0 && led_id < MAX_LEDS)
	{
		if (INITIALIZED_LEDS[led_id])
		{
			LEDS[led_id].flashes = flashes;
		}
	}
}

void led_configure_fade(int8_t led_id, uint32_t fade)
{
	if (led_id >= 0 && led_id < MAX_LEDS)
	{
		if (INITIALIZED_LEDS[led_id])
		{
			LEDS[led_id].fade = fade;
		}
	}
}

void led_configure_dt(int8_t led_id, uint8_t dt)
{
	if (led_id >= 0 && led_id < MAX_LEDS)
	{
		if (INITIALIZED_LEDS[led_id])
		{
			LEDS[led_id].dt = dt;
		}
	}
}

void led_set_state(int8_t led_id, uint8_t norm_state)
{
	if (led_id >= 0 && led_id < MAX_LEDS)
	{
		if (INITIALIZED_LEDS[led_id])
		{									 //si esta inicializado el led
			LEDS[led_id].flashing = 0;		//digo que no esta flasheando mas
			//Si me pidieron prenderlo lo prendo, sino lo apago
			if (norm_state)
			{
				pwm_query(LEDS[led_id].pwm_id, PWM_FREQ, LEDS[led_id].brightness, HIGH); //si me piden prender el led, activo la pwm
			}
			else
			{
				pwm_unquery(LEDS[led_id].pwm_id, unnormalize_state(LOW, LEDS[led_id].led_activation_mode)); //si me piden apagar, desactivo la pwm
			}
			LEDS[led_id].state = norm_state; //guardo el estado
			if (LEDS[led_id].time)
			{
				LEDS[led_id].time_start = led_timer; //si time != 0 entonces guardo cuando empieza a contar
				LED_TIMERS[led_id] = 1;
			}
		}
	}
}

void led_flash(int8_t led_id)
{
	uint8_t cur_norm_state;
	if (led_id >= 0 && led_id < MAX_LEDS)
	{
		if (INITIALIZED_LEDS[led_id])
		{
			LEDS[led_id].curr_flashes = 0; //reset contador de flashes
			LEDS[led_id].flashing = 1;	   //levanto flag de flash
			LEDS[led_id].time_start = led_timer;
			LED_TIMERS[led_id] = 1;
			pwm_query(LEDS[led_id].pwm_id, PWM_FREQ, LEDS[led_id].brightness, HIGH); //inicio la pwm
			LEDS[led_id].state = HIGH;
			if (LEDS[led_id].flashes != 0)
				LEDS[led_id].curr_flashes += 1; //y ya registro el primer flash
		}
	}
}

void led_poll(void)
{
	uint8_t i;
	uint8_t cur_norm_state;
	for (i = 0; i < MAX_LEDS; i++)
	{ //De todos los leds
		if (INITIALIZED_LEDS[i])
		{ //si alguno esta inicializado
			if (LED_TIMERS[i])
			{ //si alguno esta esperando
				if (LEDS[i].flashing)
				{ //si esta en modo flash
					if (LEDS[i].state == HIGH)
					{ //si esta prendido
						if ((float)(led_timer) - (float)(LEDS[i].time_start) > (((float)((((float)LEDS[i].period)) * (((float)(100 - LEDS[i].dt)) / 100.0))) / ((float)LED_TIMEBASE)))
						{
							pwm_unquery(LEDS[i].pwm_id, unnormalize_state(LOW, LEDS[i].led_activation_mode)); //si ya paso el tiempo, apago la pwm
							LEDS[i].state = LOW;
							LEDS[i].time_start = led_timer; //vuelvo a tomar el tiempo
						}
					}
					else
					{ //si esta apagado
						if ((float)(led_timer) - (float)(LEDS[i].time_start) > (((float)((((float)LEDS[i].period)) * (((float)(LEDS[i].dt)) / 100.0))) / ((float)LED_TIMEBASE)))
						{
							if ((LEDS[i].flashes == 0) || (LEDS[i].flashes - LEDS[i].curr_flashes > 0))
							{																   //si aun debo hacer mas flashes
								pwm_query(LEDS[i].pwm_id, PWM_FREQ, LEDS[i].brightness, HIGH); //si ya paso el tiempo, activo la pwm
								LEDS[i].state = HIGH;
								LEDS[i].time_start = led_timer; //vuelvo a tomar el tiempo
								if (LEDS[i].flashes != 0)
									LEDS[i].curr_flashes += 1; //registro otro flash
							}
							else
							{
								LED_TIMERS[i] = 0; //si ya no me quedan mas flashes, registro que el led ya no esta esperando para prenderse/apagarse
							}
						}
					}
				}
				else if (LEDS[i].time != 0)
				{ //si no esta en modo flash
					if (((float)(led_timer)) - ((float)(LEDS[i].time_start)) > (((float)(LEDS[i].time)) / ((float)(LED_TIMEBASE))))
					{ //si ya paso el tiempo
						cur_norm_state = get_normalized_state(gpioRead(LEDS[i].led_pin), LEDS[i].led_activation_mode);
						if (!cur_norm_state)
						{
							pwm_query(LEDS[i].pwm_id, PWM_FREQ, LEDS[i].brightness, HIGH); //si lo que habia hecho era apagarlo, lo vuelvo a prender un tiempo @time despues
						}
						else
						{
							pwm_unquery(LEDS[i].pwm_id, unnormalize_state(LOW, LEDS[i].led_activation_mode)); //idem si lo que habia hecho fue prenderlo
						}
						LEDS[i].state = !cur_norm_state;
						LED_TIMERS[i] = 0; //y marco que ya no debe cambiar con el tiempo
					}
				}
			}
		}
	}
	pwm_poll();
}
