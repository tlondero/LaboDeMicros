/*
 * pwm.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */

#include "../headers/pwm_old.h"
#include "../headers/debug.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
	uint32_t freq;
	uint8_t pwm_id;
	uint32_t dt;
	uint8_t pin;
	uint8_t queried;
	uint32_t last_updated;
}pwm_t;

/*******************************************************************************
 * GLOBAL VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
static uint8_t timer_id;
static uint32_t timer;
static uint8_t INITIALIZED_PWMS[MAX_PWMS];
static pwm_t PWMS[MAX_PWMS];
static uint8_t init;

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/
static void timer_callback(void);

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/
void timer_callback(void)
{
#if DEBUGGIN_MODE_PWM && DEBUGGIN_MODE
	gpioWrite(DEBUG_PIN, HIGH);
#endif


	timer++;

#if DEBUGGIN_MODE_PWM && DEBUGGIN_MODE
	gpioWrite(DEBUG_PIN, LOW);
#endif

}

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void pwm_init_driver(void)
{
#if DEBUGGIN_MODE_PWM && DEBUGGIN_MODE
	gpioMode(DEBUG_PIN, OUTPUT);
	gpioWrite(DEBUG_PIN, LOW);
#endif

	if (!init)
	{
		init = 1;
		timer_id = timerGetId(); //inicializo dependencias
		if (timer_id != TIMER_INVALID_ID)
		{
			timerStart(timer_id, (uint32_t)TIMER_MS2TICKS(PWM_TIMEBASE), TIM_MODE_PERIODIC, timer_callback); //genero una base de tiempo para el driver
		}
	}
}

int8_t pwm_get_id(void)
{
	uint8_t i;
	uint8_t found_space = 0;
	int8_t id = -1;
	for (i = 0; i < MAX_PWMS; i++)
	{
		if (!(INITIALIZED_PWMS[i]))
		{ //me fijo si hay espacio
			INITIALIZED_PWMS[i] = 1;
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

int8_t pwm_init_signal(pin_t pin)
{
	int8_t id = pwm_get_id();
	if (id != UNAVAILABLE_SPACE)
	{
		PWMS[id].pin = pin;
		PWMS[id].pwm_id = id;
	}
	return id;
}

void pwm_destroy(uint8_t pwm_id)
{
	if (pwm_id >= 0 && pwm_id < MAX_PWMS)
	{
		INITIALIZED_PWMS[pwm_id] = 0; //libero espacio
	}
}

void pwm_query(int8_t pwm_id, uint32_t freq, uint32_t dt, uint8_t initial_state)
{
	if (pwm_id >= 0 && pwm_id < MAX_PWMS)
	{
		if (freq > 0 && freq < (uint32_t)(1000.0 / (2.0 * (float)PWM_TIMEBASE)))
		{ //no anda si la base de tiempo no es al menos dos veces mas chica que el periodo
			if (dt >= 0 && dt <= 100)
			{
				if (INITIALIZED_PWMS[pwm_id] == 1)
				{
					PWMS[pwm_id].freq = freq;
					PWMS[pwm_id].dt = dt;
					PWMS[pwm_id].queried = 1; //registro que la pwm esta activa
					PWMS[pwm_id].last_updated = timer;
					gpioWrite(PWMS[pwm_id].pin, initial_state); //ya seteo el pin en el estado inicial pedido
				}
			}
		}
	}
}

void pwm_unquery(int8_t pwm_id, uint8_t final_state)
{
	if (pwm_id >= 0 && pwm_id < MAX_PWMS)
	{
		if (INITIALIZED_PWMS[pwm_id] == 1)
		{
			PWMS[pwm_id].queried = 0;				  //registro que la pwm esta desactivada
			gpioWrite(PWMS[pwm_id].pin, final_state); //dejo el pin en su estado final
		}
	}
}

void pwm_poll(void)
{
	uint8_t i;
	for (i = 0; i < MAX_PWMS; i++)
	{
		if (INITIALIZED_PWMS[i] == 1)
		{
			if (PWMS[i].queried)
			{ //si la pwm del pin esta activada
				if (gpioRead(PWMS[i].pin))
				{ //si estaba prendido
					if (((float)timer) - (float)PWMS[i].last_updated > (((float)(((1.0 / ((float)PWMS[i].freq))) * (1000.0 / PWM_TIMEBASE) * (((float)(100 - PWMS[i].dt)) / 100.0))) / ((float)PWM_TIMEBASE)))
					{
						gpioToggle(PWMS[i].pin);	  //si ya paso periodo*(100-duty cycle) apago el pin
						PWMS[i].last_updated = timer; //reseteo el contador
					}
				}
				else
				{ //si estaba apagado
					if (((float)timer) - (float)PWMS[i].last_updated > (((float)(((1.0 / ((float)PWMS[i].freq))) * (1000.0 / PWM_TIMEBASE) * (((float)(PWMS[i].dt)) / 100.0))) / ((float)PWM_TIMEBASE)))
					{
						gpioToggle(PWMS[i].pin);	  //si ya paso periodo*(duty cycle) prendo el pin
						PWMS[i].last_updated = timer; //reseteo el contador
					}
				}
			}
		}
	}
}
