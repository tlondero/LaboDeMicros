/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include <header/event_handlers/uartPackHand.h>

#include <header/event_handlers/uartPackHand.h>
#include "header/event_handlers/AccelerometerEvents.h"


#include <header/drivers/FXOS8700CQ.h>
#include <header/drivers/gpio.h>
#include <header/drivers/i2c.h>
#include <header/drivers/uart.h>
#include "header/drivers/timer.h"
#include "header/event_handlers/paquetes.h"
#include "header/board.h"

#include "MK64F12.h"
#include <math.h>
#include "hardware.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define THRESHOLD (0.5)
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

void processEvents( package * PEvents);

void appInit(void) {
	//Accelerometer init
	FXOS8700CQInit();

	//UART init
	uart_cfg_t init_config;
	init_config.baudRate = 9600;
	init_config.mode = NON_BLOCKING;
	init_config.parity = NO_PARITY_UART;
	init_config.nBits = NBITS_8;
	uartInit(U0, init_config);

	//leds init
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);
	gpioWrite(PIN_LED_GREEN, HIGH);
	gpioWrite(PIN_LED_BLUE, HIGH);

	//timer init
	timerInit();
	tim_id_t tid = timerGetId();
	timerStart(tid, TIMER_MS2TICKS(200),TIM_MODE_PERIODIC,FXOS8700CQ_ReadAccelMagnData);
}


/* Función que se llama constantemente en un ciclo infinito */
SRAWDATA_f ac;
void appRun(void){
	package data={0};
	accelerometerGetEvent(&data);
	uartPackProcess(&data, U0);
	processEvents(&data);
}


void processEvents( package * PEvents){
	if(PEvents->action.down== true){
		gpioToggle(PIN_LED_RED);
		gpioToggle(PIN_LED_BLUE);
	}
	else if(PEvents->action.left == true){
		gpioToggle(PIN_LED_GREEN);
	}
	else if(PEvents->action.right== true){
		gpioToggle(PIN_LED_RED);
	}
	else if(PEvents->action.rotate== true){
		gpioToggle(PIN_LED_GREEN);
		gpioToggle(PIN_LED_RED);
	}
	else if(PEvents->reset == true){
		gpioToggle(PIN_LED_GREEN);
		gpioToggle(PIN_LED_RED);
		gpioToggle(PIN_LED_BLUE);
	}
}

