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
#include "header/Button.h"
#include "uart.h"
#include "header/gpio.h"
#include "MK64F12.h"
#include "header/board.h"
#include "hardware.h"
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


int8_t bt1;
void App_Init (void)
{
	uart_init(UART0,__CORE_CLOCK__/1000,9600);
	bt1=ButtonInit(PIN_SW3 , INPUT_PULLUP);


}


/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
	const ButtonEvent * evsw1 = ButtonGetEvent(bt1);

    while((*evsw1) != EOQ){
	switch (*evsw1){
		case LKP:
			break;
		case PRESS:
			uart_putchar(UART0, 'X');
			break;

		case RELEASE:
			break;
		case SKP:
			break;
		default:

			break;
		}
	evsw1++;
    }
}

//
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

