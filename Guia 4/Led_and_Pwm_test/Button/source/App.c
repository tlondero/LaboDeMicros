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





static tim_id_t idButton1;
static int8_t idLed1;
static uint8_t br=100;
static tim_id_t timer_trucho;
static bool bool_trucho;

void callback(void){
	bool_trucho = !bool_trucho;
	if(bool_trucho == false)
		led_configure_brightness(idLed1, 0);
	else
		led_configure_brightness(idLed1, br);
}
void App_Init (void)
{
	timer_trucho = timerGetId();
	timerStart(timer_trucho, 8, TIM_MODE_PERIODIC, callback);
	//Interrupciones de pines
	led_init_driver();
	idButton1= initButton(PIN_SW2, INPUT_PULLUP);
	idLed1 = led_init_led(PORTNUM2PIN(PC,1), TURNS_ON_WITH_1); //PORTNUM2PIN(2, 3)
	led_configure_brightness(idLed1, br);
	led_set_state(idLed1, HIGH);
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
		br = br - 20;
		led_configure_brightness(idLed1, 1);
	}
	led_poll();
}
