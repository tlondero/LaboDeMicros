/***************************************************************************//**
 @file     event_handler.h
 @brief
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <header/event_handlers/uartGetEvent.h>
#include "../header/event_handlers/event_handler.h"
#include "../header/event_handlers/AccelerometerEvents.h"
#include "../header/drivers/POTE_ADC.h"

#include "../header/event_handlers/paquetes.h"

#include "../header/drivers/uart.h"
#include "../header/tetris_game.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION DECLARATION WITH LOCAL SCOPE
 ******************************************************************************/
void processEvents(package * PEvents);
void resetFunction(void);
void leftFunction(void);
void rightFunction(void);
void downFunction(void);
void rotateFunction(void);
void brightnessFunction(uint8_t br);
void fallSpeedFunction(uint8_t fs);
void piecePropertyFunction(char p, uint8_t r, uint8_t g, uint8_t b);

void processEventsTB(package * PEvents);
/*******************************************************************************
 * FUNCTION DEFINITION WITH GLOBAL SCOPE
 ******************************************************************************/

void EvHandGetEvents(void) {
	package data = { 0 };
	accelerometerGetEvent(&data); //I2C events

	uartGetEvent(&data, U3); //Uart events
	//SPI events

	//ADC events
	PoteGetEvent(&data);

//	processEvents(&data);
	processEventsTB(&data);
}

/*******************************************************************************
 * FUNCTION DEFINITION WITH LOCAL SCOPE
 ******************************************************************************/
void processEvents(package * PEvents) {

	if (PEvents->reset) {
		resetFunction();	//reset function
	} else if(PEvents->pause){
		if(tetris_get_game_status() == TETRIS_PAUSED_ST)
			tetris_resume_game();
		else if(tetris_get_game_status() == TETRIS_RUNNING_ST)
			tetris_pause_game();
	}
	else if(tetris_get_game_status() == TETRIS_RUNNING_ST) {
		if (PEvents->action.down) {
			downFunction();	//Down function
		}
		if (PEvents->action.left) {
			leftFunction();	//Left function
		}
		if (PEvents->action.right) {
			rightFunction();	//Right function
		}
		if (PEvents->action.rotate) {
			rotateFunction();	//rotate function
		}
		if (PEvents->birghtness.change) {
			brightnessFunction(PEvents->birghtness.birghtness);	//brightness function
		}
		if (PEvents->fall_speed.change) {
			fallSpeedFunction(PEvents->fall_speed.fall_speedowagon);//fall speed function
		}
		if (PEvents->piece.changed) {
			piecePropertyFunction(PEvents->piece.piece, PEvents->piece.color.r,
					PEvents->piece.color.g, PEvents->piece.color.b);//update piece property
		}
	}
}

void processEventsTB(package * PEvents) {
	int a = -1;
	if (PEvents->reset) {
		a=0;
	} else {
		if (PEvents->action.down) {
			a = 1;
		}
		if (PEvents->action.left) {
			a = 2;
		}
		if (PEvents->action.right) {
			a = 3;
		}
		if (PEvents->action.rotate) {
			a = 4;
		}
		if (PEvents->birghtness.change) {
			a = 5;
		}
		if (PEvents->fall_speed.change) {
			a = 6;
		}
		if (PEvents->piece.changed) {
			a = 7;
		}
	}
}

void resetFunction(void) {
	tetris_restart_game();
}

void leftFunction(void) {
	tetris_move_left();
}

void rightFunction(void) {
	tetris_move_right();
}

void downFunction(void) {
	tetris_move_down();
}

void rotateFunction(void) {
	tetris_rotate_piece();
}

void brightnessFunction(uint8_t br) {
	drawer_change_brightness(br);
}

void fallSpeedFunction(uint8_t fs) {
	tetris_set_difficulty(fs);
	//ver que esto esté entre 0 y 30 para que sea humano
}

void piecePropertyFunction(char p, uint8_t r, uint8_t g, uint8_t b) {
	drawer_change_piece(p, r, g, b);
}

