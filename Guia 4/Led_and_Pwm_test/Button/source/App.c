/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

//#include <header/ejercicios_headers/BalizaSwitchSysTick/balizaSwitchSysTick.h>
//#include "header/ejercicios_headers/Baliza/baliza.h"
//#include "header/ejercicios_headers/BalizaSysTick/balizast.h"
#include "header/board.h"
#include "header/gpio.h"
#include "stdbool.h"
#include <stdio.h>
#include "header/SysTick.h"
#include "timer.h"
#include "header/Button.h"
#include "header/led.h"
#include "header/pwm.h"
#include "timer.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 * INTERRUPCIONES
 ******************************************************************************/




/*******************************************************************************
 * BALIZA
 ******************************************************************************/

//static void update_baliza(int period);





static int8_t idButton1, idLed1, idLed2, idLed3, idTimer1;
static uint32_t timer=3;
static uint32_t rainbow_start;
static uint8_t rainbow;
void callbacktimer(void);

void App_Init (void)
{

	//Interrupciones de pines
	timerInit();
	led_init_driver();
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE,HIGH);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED,HIGH);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_GREEN,HIGH);
	idButton1= initButton(PIN_SW2, INPUT_PULLUP);
	idLed1 = led_init_led(PIN_LED_RED, TURNS_ON_WITH_0);
	idLed2 = led_init_led(PIN_LED_GREEN, TURNS_ON_WITH_0);
	idLed3 = led_init_led(PIN_LED_BLUE, TURNS_ON_WITH_0);
	led_configure_period(idLed1, 1000);
	led_configure_flashes(idLed1, 10);
	led_configure_dt(idLed1, 50);
	led_configure_brightness(idLed1, 2);
	led_configure_period(idLed2, 1000);
	led_configure_flashes(idLed2, 10);
	led_configure_dt(idLed2, 50);
	led_configure_brightness(idLed2, 2);
	led_configure_period(idLed3, 1000);
	led_configure_flashes(idLed3, 10);
	led_configure_dt(idLed3, 50);
	led_configure_brightness(idLed3, 2);
	idTimer1 = timerGetId();
	timerStart(idTimer1, 250, TIM_MODE_PERIODIC, callbacktimer);
}

void callbacktimer(void){
	timer++;
	if(rainbow == 1){
		if(timer == rainbow_start+1){
			led_flash(idLed1);
		}
		else if(timer == rainbow_start+4){
			led_flash(idLed2);
		}
		else if(timer == rainbow_start+7){
			led_flash(idLed3);
		}
		else if(timer == rainbow_start+8){
			rainbow = 0;
		}
	}
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	/*
	 * TIENE QUE PRENDER EL LED ROJO POR 1 SEG (esta configurado en configure_time(idLed1, 1000) en el app init
	 * despues de tocar el SW2 (el que no esta al lado del boton) de la placa.
	 */
	Button_Event evsw1 = getButtonEvent(idButton1);
	if(evsw1 == PRESS){
		rainbow = 1;
		rainbow_start = timer;
	}
	led_poll();
	pwm_poll();
}
