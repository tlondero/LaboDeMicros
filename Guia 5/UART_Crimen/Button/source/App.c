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
#include "header/uart.h"
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

void App_Init(void) {
	uart_cfg_t init_config;
	init_config.baudRate = 9600;
	init_config.mode = NON_BLOCKING;
	init_config.parity = NO_PARITY_UART;
	init_config.nBits = NBITS_8;
	uartInit(U0, init_config);
}

/* Función que se llama constantemente en un ciclo infinito */
char msg_buffer[100];
void App_Run(void) {
	uint8_t len = 0;

	if (uartIsRxMsg(U0)) {
		len = uartGetRxMsgLength(U0);
		len =uartReadMsg(U0, msg_buffer, len);
		uartWriteMsg(U0, msg_buffer, len);
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

