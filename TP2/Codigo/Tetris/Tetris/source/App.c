/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "header/event_handlers/AccelerometerEvents.h"
#include "header/event_handlers/uartGetEvent.h"
#include "header/event_handlers/event_handler.h"
#include "header/event_handlers/spiEventHandler.h"
#include "header/event_handlers/paquetes.h"

#include "header/drivers/FXOS8700CQ.h"
#include "header/drivers/WS2812B.h"
#include "header/drivers/gpio.h"
#include "header/drivers/i2c.h"
#include "header/drivers/uart.h"
#include "header/drivers/timer.h"

#include "header/Drawer.h"



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

board_ptr front_end_tetris;
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
	uartInit(U3, init_config);

	//SPI init
	SPI_589_init();

	//Matriz de led
	//WS2812B_init();



	//Pote
	PoteInit();


	static uint8_t nFieldWidth = 10;
	static uint8_t nFieldHeight = 9;

	//Drawer


	color_t color[7];

	color[0].r = 0;			// PIECE_I_H
	color[0].g = 240;
	color[0].b = 240;

	color[1].r = 160;			// PIECE_T_H
	color[1].g = 0;
	color[1].b = 240;

	color[2].r = 240;			// PIECE_O_H
	color[2].g = 240;
	color[2].b = 0;

	color[3].r = 0;			// PIECE_S_H
	color[3].g = 240;
	color[3].b = 0;

	color[4].r = 240;			// PIECE_Z_H
	color[4].g = 0;
	color[4].b = 0;

	color[5].r = 240;			// PIECE_L_H
	color[5].g = 160;
	color[5].b = 0;

	color[6].r = 0;			// PIECE_J_H
	color[6].g = 0;
	color[6].b = 240;


	drawer_init(nFieldWidth, nFieldHeight, color);

	//Tetris init
	tetris_init(nFieldWidth, nFieldHeight);

	tetris_begin_game(); //Sets game mode to RUNNING
	tetris_set_difficulty(EASY);
	tim_id_t teturisuID = timerGetId();
	//
	timerStart(teturisuID, TIMER_MS2TICKS(200), TIM_MODE_PERIODIC, tetrisRun);
	front_end_tetris =tetris_get_board();
	//


}


/* Función que se llama constantemente en un ciclo infinito */

void tetrisRun(void){

	EvHandGetEvents();

	if((((tetris_get_game_status() == TETRIS_RUNNING_ST) || (tetris_get_game_status() ==  TETRIS_PAUSED_ST) ))){
		tetris_update_board();
	}
	else if(tetris_get_game_status()  == TETRIS_GAME_OVER_ST){
		drawer_draw_gameover(tetris_get_score());
	}
	drawer_update_board(front_end_tetris);
}
void appRun(void){
}

