/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "header/event_handlers/AccelerometerEvents.h"
#include "header/drivers/POTE_ADC.h"

#include <header/drivers/FXOS8700CQ.h>
#include <header/drivers/gpio.h>
#include <header/drivers/i2c.h>
#include <header/drivers/uart.h>
#include <header/event_handlers/uartGetEvent.h>
#include <header/event_handlers/uartGetEvent.h>
#include "header/drivers/timer.h"
#include "header/event_handlers/paquetes.h"
#include "header/board.h"
#include "header/tetris_game.h"
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

//void processEvents( package * PEvents);
void tetrisRun(void);
void appInit(void) {

	timerInit();

	//Accelerometer init
	FXOS8700CQInit();


	//UART init
	uart_cfg_t init_config;
	init_config.baudRate = 9600;
	init_config.mode = NON_BLOCKING;
	init_config.parity = NO_PARITY_UART;
	init_config.nBits = NBITS_8;
	uartInit(U0, init_config); //ESTE ES EL USB, el 3 es el que hay que iniciar para el ESP


	static uint8_t nFieldWidth = 10;
	static uint8_t nFieldHeight = 9;



	tetris_init(nFieldWidth, nFieldHeight);

	tetris_begin_game(); //Sets game mode to RUNNING
	tetris_set_difficulty(EASY);
	tim_id_t teturisuID = timerGetId();
	//
//	timerStart(teturisuID, TIMER_MS2TICKS(50),TIM_MODE_PERIODIC,tetrisRun);
	//

	//Pote
	PoteInit();
}


/* Función que se llama constantemente en un ciclo infinito */

void tetrisRun(void){
	static bool bGameOver = false;
	if(((tetris_get_game_status() == TETRIS_RUNNING_ST) && !bGameOver)){
		EvHandGetEvents();
		tetris_update_board();
	}
	else if(tetris_get_game_status()  == TETRIS_GAME_OVER_ST){
		drawer_draw_gameover(tetris_get_score());
	}
	drawer_update_board(tetris_get_board());
}
appRun(){
	//do nothing
	EvHandGetEvents();
}

