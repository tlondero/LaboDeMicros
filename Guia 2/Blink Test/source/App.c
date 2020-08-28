/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);

//static void guido_delay(double period);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, HIGH);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);
	gpioMode(PIN_SW3, INPUT);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

	if (!gpioRead(PIN_SW3)){
		gpioToggle(PIN_LED_BLUE);
	}
    //delayLoop(4000000UL);
	//guido_delay(2);
    //gpioToggle(PIN_LED_RED);
    //guido_delay();
    //gpioToggle(PIN_LED_BLUE);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/*
static void guido_delay(double period){
	double wait = period / 2.0;
	time_t start, end;
	time(&start);
	do time(&end); while(difftime(end, start) <= wait);
}*/


static void delayLoop(uint32_t veces)
{
    while (veces--);
}


/*******************************************************************************
 ******************************************************************************/
