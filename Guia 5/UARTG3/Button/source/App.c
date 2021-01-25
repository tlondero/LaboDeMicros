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
#include <string.h>
#include "header/uartPackHand.h"
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
void UartPackageTB(void);
void UartPackageTBinit(void);

static int8_t bt1;
static bool free_running = true;
static char msg_buffer[5000];
void App_Init(void) {
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioMode(PIN_LED_GREEN, OUTPUT);

	UartPackageTBinit();

}

/* Función que se llama constantemente en un ciclo infinito */
package data={0};
void App_Run(void) {
	UartPackageTB();
//	uartPackProcess(&data,U0);
}

void UartEspTBinit(void) {
	uart_cfg_t init_config;
	init_config.baudRate = 9600;
	init_config.mode = NON_BLOCKING;
	init_config.parity = NO_PARITY_UART;
	init_config.nBits = NBITS_8;
	uartInit(U0, init_config);

	gpioWrite(PIN_LED_RED, HIGH);
	gpioWrite(PIN_LED_GREEN, HIGH);
	gpioWrite(PIN_LED_BLUE, HIGH);
}


void UartPackageTBinit(void) {
	uart_cfg_t init_config;
	init_config.baudRate = 9600;
	init_config.mode = NON_BLOCKING;
	init_config.parity = NO_PARITY_UART;
	init_config.nBits = NBITS_8;
	uartInit(U0, init_config);
	uartWriteMsg(U0, "UART initialized\n\r", 19);
	gpioWrite(PIN_LED_RED, HIGH);
	gpioWrite(PIN_LED_GREEN, HIGH);
	gpioWrite(PIN_LED_BLUE, HIGH);
}

static bool first = true;
//typedef struct{
//	uint8_t r;
//	uint8_t g;
//	uint8_t b;
//}color_t;
//typedef struct{
//	char piece;
//	color_t color;
//}piece_prop;
//
//typedef struct{
//	bool left;
//	bool right;
//	bool down;
//	bool rotate;
//}piece_movement;
//typedef struct{
//	uint8_t birghtness;
//	uint8_t fall_speed;
//	piece_movement action;
//	piece_prop piece;
//	bool reset;
//}package;

void UartPackageTB(void) {

	if (uartIsRxMsg(U0) && (uartGetRxMsgLength(U0) >= 5)) {
		uint8_t len = 0;
		len = uartReadMsg(U0, msg_buffer, 4);
		switch (msg_buffer[0]) {
		case 'B':
			uartWriteMsg(U0, "Brightness changed\r\n", 21);
			gpioToggle(PIN_LED_GREEN);
			break;
		case 'F':
			uartWriteMsg(U0, "Fall speed changed\r\n", 21);
			gpioToggle(PIN_LED_BLUE);
			break;
		case 'E':
			uartWriteMsg(U0, "Action Left\r\n", 14);
			break;
		case 'D':
			uartWriteMsg(U0, "Action Right\r\n", 15);
			break;
		case 'A':
			uartWriteMsg(U0, "Action Down\r\n", 14);
			break;
		case 'G':
			uartWriteMsg(U0, "Action Rotate\r\n", 16);
			break;
		case 'R':
			uartWriteMsg(U0, "Action Reset\r\n", 15);
			gpioToggle(PIN_LED_RED);
			break;
		case 'L':
		case 'J':
		case 'T':
		case 'I':
		case 'Z':
		case 'O':
		case 'S':
			uartWriteMsg(U0, "Piece prop changed\r\n", 21);
			break;
		default:
			break;
		}
	}

}

void UartEspTB(void) {

	if (uartIsRxMsg(U3)) {
		uint8_t len = 0;
		len = uartGetRxMsgLength(U3);
		len = uartReadMsg(U3, msg_buffer, len);

		if (msg_buffer[0] == 'r') {
			gpioToggle(PIN_LED_RED);
		} else if (msg_buffer[0] == 'g') {
			gpioToggle(PIN_LED_GREEN);
		} else if (msg_buffer[0] == 'b') {
			gpioToggle(PIN_LED_BLUE);
		}

	}

}

void UartTBInit(void) {
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
		uartWriteMsg(U0,
				"Welcome to the UART Test bench, There are two switches in the FRDM board, the SW3 is used to alternate between free running mode and buffer mode\r\n",
				147);
		uartWriteMsg(U0,
				"Free Running mode: Every character typed will be sent to the screen\r\n",
				70); //nice
		uartWriteMsg(U0,
				"Buffer mode: Will collect every character typed until the SW2 is pressed, by pressing the buffer will be sent to screen\r\n",
				122);
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
