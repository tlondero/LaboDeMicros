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
#include "drivers/headers/board.h"
#include "drivers/headers/led.h"
#include "stdbool.h"
#include <stdio.h>
#include "drivers/headers/SysTick.h"
#include "drivers/headers/timer.h"
#include "drivers/headers/Button.h"
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


//void blockTime(void){
//	timerDelay(5000);
//}
//void ButtonTB(void);

int8_t idButton1, idButton2;
void turn_on_led(void);
void App_Init (void)
{
	//Interrupciones de pines
	led_init_driver();
}


/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
//ButtonTB();
}


//void ButtonTB(void){
//
//	const ButtonEvent * evsw1 = ButtonGetEvent(idButton1);
//
//	static int a=0;
//
//    while((*evsw1) != EOQ){
//	switch (*evsw1){
//		case LKP:
//			gpioWrite(PIN_LED_RED, HIGH);
//			gpioWrite(PIN_LED_BLUE, LOW);
//			gpioWrite(PIN_LED_GREEN, HIGH);
//			a++;
//			break;
//		case PRESS:
//			gpioWrite(PIN_LED_RED, HIGH); //PRESS VERDEE
//			gpioWrite(PIN_LED_BLUE, HIGH);
//			gpioWrite(PIN_LED_GREEN, LOW);
//			break;
//
//		case RELEASE:
//			gpioWrite(PIN_LED_RED, LOW);  //ROJO RELEASE
//			gpioWrite(PIN_LED_BLUE, HIGH);
//			gpioWrite(PIN_LED_GREEN, HIGH);
//
//			break;
//		case SKP:
//			gpioWrite(PIN_LED_RED, LOW);  //ROJO RELEASE
//			gpioWrite(PIN_LED_BLUE, LOW);
//			gpioWrite(PIN_LED_GREEN, LOW);
//			break;
//		default:
//
//			break;
//		}
//	evsw1++;
//    }
//    if(a==2){
//    	a=0;
//    }
//	gpioWrite(PIN_LED_RED, HIGH);
//	gpioWrite(PIN_LED_BLUE, HIGH);
//	gpioWrite(PIN_LED_GREEN, HIGH);
//
//
//}
//
