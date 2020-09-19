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
#include "header/Button.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 * INTERRUPCIONES
 ******************************************************************************/

static CallbackInterrupt BlinkIRQ;
static CallbackInterrupt SwitchCheckIRQ;
static CallbackInterrupt BalizaSystick;
static CallbackInterrupt  BlankInterrupt;



/*******************************************************************************
 * BALIZA
 ******************************************************************************/

//static void update_baliza(int period);
static unsigned long ms2UL(int number);
static int UL2ms(unsigned long numer);


int8_t idButton;
void App_Init (void)
{

	//Interrupciones de pines
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE,HIGH);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED,HIGH);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_GREEN,HIGH);
	idButton= initButton(PIN_SW3, INPUT);

}

/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
	//1-Do baliza sin interrupciones
	//do_baliza();

	//2-Do baliza con interrupciones
	//no code

	//3-Do baliza y chequeo del pulsador con interrupciones
	//no code
	Button_Event ev = getButtonEvent(idButton);
	switch (ev){
	case  TYPEMATIC:
	case LKP:
		break;
	case PRESS:
		gpioWrite(PIN_LED_RED, HIGH);
		gpioWrite(PIN_LED_BLUE, HIGH);
		gpioWrite(PIN_LED_GREEN, LOW);
	case RELEASE:
		gpioWrite(PIN_LED_RED, LOW);
		gpioWrite(PIN_LED_BLUE, HIGH);
		gpioWrite(PIN_LED_GREEN, HIGH);
		break;
	default:
		break;
	}


}


