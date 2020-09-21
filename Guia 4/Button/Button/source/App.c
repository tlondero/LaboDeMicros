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





int8_t idButton1, idButton2;
void turn_on_led(void);
void App_Init (void)
{

	//Interrupciones de pines

	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE,HIGH);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED,HIGH);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_GREEN,HIGH);
	idButton1= initButton(PIN_SW2, INPUT_PULLUP);
	idButton2= initButton(PIN_SW3, INPUT_PULLUP);
	setIRQ_button(idButton2, GPIO_IRQ_MODE_RISING_EDGE, turn_on_led);
}

 void turn_on_led(void){
	gpioWrite(PIN_LED_RED, LOW);
	gpioWrite(PIN_LED_BLUE, LOW);
	gpioWrite(PIN_LED_GREEN, HIGH);

 }
/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
	Button_Event evsw1 = getButtonEvent(idButton1);



	switch (evsw1.hold_evs){
		case LKP:
			gpioWrite(PIN_LED_RED, HIGH);
			gpioWrite(PIN_LED_BLUE, LOW);
			gpioWrite(PIN_LED_GREEN, HIGH);
			break;
		case PRESS:
			gpioWrite(PIN_LED_RED, HIGH); //PRESS VERDEE
			gpioWrite(PIN_LED_BLUE, HIGH);
			gpioWrite(PIN_LED_GREEN, LOW);
			break;

		case RELEASE:
			gpioWrite(PIN_LED_RED, LOW);  //ROJO RELEASE
			gpioWrite(PIN_LED_BLUE, HIGH);
			gpioWrite(PIN_LED_GREEN, HIGH);
			break;
		case SKP:
			gpioWrite(PIN_LED_RED, LOW);  //ROJO RELEASE
			gpioWrite(PIN_LED_BLUE, LOW);
			gpioWrite(PIN_LED_GREEN, LOW);
			break;
		default:

			break;
		}


}


