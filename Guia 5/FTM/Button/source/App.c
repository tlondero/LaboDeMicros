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
#include "stdbool.h"
#include <stdio.h>
#include "header/SysTick.h"
#include "header/timer.h"
#include "header/Button.h"
#include "header/FTM.h"
#include "header/timer.h"
#include "header/PORT.h"
#include "header/WS2812B.h"
#include "header/gpio.h"
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

void timer_callback(void){
	gpioToggle(PORTNUM2PIN(PB,19));
}
void App_Init (void)
{
	gpioMode(PORTNUM2PIN(PB,19), OUTPUT);
	gpioWrite(PORTNUM2PIN(PB,19),LOW);
	tim_id_t test_timer =  timerGetId();
	timerStart(test_timer , 1, TIM_MODE_PERIODIC, timer_callback);
	//Esta puesto para que la señal FTM salga por el PC3, para cambiar qué se muestra en la matriz actualmente
	//hay que cambiarlo en el driver WS2812B en el init, esta ahi hardcodeado a mano que arranque to do apagado
	//y despues prende uno solo, el segundo led.
	WS2812B_init();
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

}
