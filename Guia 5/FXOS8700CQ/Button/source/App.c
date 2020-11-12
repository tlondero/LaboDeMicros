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
#include "timer.h"
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
void UartTB(void);
void UartTBInit(void);
void UartTB2(void);

static int8_t bt1;
static bool free_running=true;

void App_Init(void) {
	UartTBInit();
}

/* Función que se llama constantemente en un ciclo infinito */


void App_Run(void) {
	UartTB2();
}
void UartTBInit(void){
	uart_cfg_t init_config;
	init_config.baudRate = 9600;
	init_config.mode = NON_BLOCKING;
	init_config.parity = NO_PARITY_UART;
	init_config.nBits = NBITS_8;
	uartInit(U3, init_config);
	timerInit();
	bt1 = ButtonInit(PIN_SW3, INPUT_PULLUP);
}
void UartTB(void) {
	static char msg_buffer[5000];
	static bool first = true;
	uint8_t len = 0;
	if (first) {
		first = false;
		uartWriteMsg(U0, "Welcome to the UART Test bench, There are two switches in the FRDM board, the SW3 is used to alternate between free running mode and buffer mode\r\n", 147);
		uartWriteMsg(U0, "Free Running mode: Every character typed will be sent to the screen\r\n",70);//nice
		uartWriteMsg(U0, "Buffer mode: Will collect every character typed until the SW2 is pressed, by pressing the buffer will be sent to screen\r\n",122);
	}
	const ButtonEvent * evsw2 = ButtonGetEvent(bt1);
	if (free_running && uartIsRxMsg(U0)) {

			len = uartGetRxMsgLength(U0);
			len = uartReadMsg(U0, msg_buffer, len);
			uartWriteMsg(U0, msg_buffer, len);
	}
	while ((*evsw2) != EOQ) {
				switch (*evsw2) {
				case PRESS:
					free_running = !free_running;
					break;
				default:
					break;
				}
				evsw2++;
			}
}
void UartTB2(void) {
	static char msg_buffer[5000];
	uint8_t len = 0;
//	if (uartIsRxMsg(U3)) {
//		len = uartGetRxMsgLength(U3);
//		len = uartReadMsg(U3, msg_buffer, len);
//	}
	const ButtonEvent * evsw2 = ButtonGetEvent(bt1);
	while ((*evsw2) != EOQ) {
				switch (*evsw2) {
				case PRESS:
					uartWriteMsg(U3, "Pizzina", 7);
					break;
				case RELEASE:
					if (uartIsRxMsg(U3)) {
						len = uartGetRxMsgLength(U3);
						len = uartReadMsg(U3, msg_buffer, len);
					}
					break;
				default:
					break;
				}
				evsw2++;
			}

}
