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

void App_Init (void)
{
	//Esta puesto para que la señal FTM salga por el PC3, para cambiar qué se muestra en la matriz actualmente
	//hay que cambiarlo en el driver WS2812B en el init, esta ahi hardcodeado a mano que arranque to do apagado
	//y despues prende uno solo, el segundo led.
	WS2812B_init();
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

}
