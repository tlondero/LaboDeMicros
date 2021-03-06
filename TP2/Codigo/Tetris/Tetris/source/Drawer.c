/*
 * Drawer.c
 *
 *  Created on: Nov 29, 2020
 *      Author: Acer
 */

#include "header/tetris_game.h"
#include "header/drivers/WS2812B.h"
#include "header/event_handlers/paquetes.h"
#include "header/Drawer.h"
#include "header/drivers/timer.h"
#define DEFAULT_BRIGHTNESS 50

static piece_prop pieces[PKG_CANT_PIECES];
static uint8_t br = DEFAULT_BRIGHTNESS;
static uint8_t board_w;
static uint8_t board_h;
static uint8_t timerperid;
static uint8_t gv_aux = 0;
static uint8_t gv_first = 0;

void clean_board(void){
	uint8_t row;
	uint8_t col;
	uint8_t col_board;
	for(row = 0; row < board_h-1 ; row++){
		for(col = 1; col < board_w-1; col++){
			col_board = col-1;
			WS2812B_matrix_set(row, col_board, 0, 0, 0);
		}
	}
}

void draw_cross(void){
	WS2812B_matrix_set(1, 1, 120, 0, 0);
	WS2812B_matrix_set(2, 2, 120, 0, 0);
	WS2812B_matrix_set(3, 3, 120, 0, 0);
	WS2812B_matrix_set(4, 4, 120, 0, 0);
	WS2812B_matrix_set(5, 5, 120, 0, 0);
	WS2812B_matrix_set(6, 6, 120, 0, 0);
	WS2812B_matrix_set(1, 6, 120, 0, 0);
	WS2812B_matrix_set(2, 5, 120, 0, 0);
	WS2812B_matrix_set(3, 4, 120, 0, 0);
	WS2812B_matrix_set(4, 3, 120, 0, 0);
	WS2812B_matrix_set(5, 2, 120, 0, 0);
	WS2812B_matrix_set(6, 1, 120, 0, 0);
}

void tim_per_cb(void){
	gv_aux = !gv_aux;
	if(!gv_aux){
		clean_board();
		draw_cross();
	}
	else{
		clean_board();
	}
}

void drawer_change_brightness(uint8_t br_new){
	br = br_new;
}
void drawer_change_piece(char p, uint8_t r, uint8_t g, uint8_t b){
	uint8_t i;
	for(i = 0; i < PKG_CANT_PIECES; i++){
		if(pieces[i].piece == p){
			pieces[i].color.r = r;
			pieces[i].color.g = g;
			pieces[i].color.b = b;
		}
	}
}
void drawer_draw_gameover(uint8_t score){
	if(gv_first){
		timerReset(timerperid);
		gv_first = false;
	}
}
void drawer_update_board(board_ptr board){
	uint8_t row;
	uint8_t col;
	uint8_t col_board;
	if(gv_first == 0){
		gv_first = 1;
		timerStop(timerperid);
	}
	for(row = 0; row < board_h-1 ; row++){
		for(col = 1; col < board_w-1; col++){
			col_board = col-1;
			switch(board[board_w*row+col]){
			case EMPTY:
				WS2812B_matrix_set(row, col_board, 0, 0, 0);
				break;
			case T1:
				WS2812B_matrix_set(row, col_board, (uint8_t)(pieces[T1-1].color.r*(br/100.0)), (uint8_t)(pieces[T1-1].color.g*(br/100.0)), (uint8_t)(pieces[T1-1].color.b*(br/100.0)));
				break;
			case T2:
				WS2812B_matrix_set(row, col_board, (uint8_t)(pieces[T2-1].color.r*(br/100.0)), (uint8_t)(pieces[T2-1].color.g*(br/100.0)), (uint8_t)(pieces[T2-1].color.b*(br/100.0)));
				break;
			case T3:
				WS2812B_matrix_set(row, col_board, (uint8_t)(pieces[T3-1].color.r*(br/100.0)), (uint8_t)(pieces[T3-1].color.g*(br/100.0)), (uint8_t)(pieces[T3-1].color.b*(br/100.0)));
				break;
			case T4:
				WS2812B_matrix_set(row, col_board, (uint8_t)(pieces[T4-1].color.r*(br/100.0)), (uint8_t)(pieces[T4-1].color.g*(br/100.0)), (uint8_t)(pieces[T4-1].color.b*(br/100.0)));
				break;
			case T5:
				WS2812B_matrix_set(row, col_board, (uint8_t)(pieces[T5-1].color.r*(br/100.0)), (uint8_t)(pieces[T5-1].color.g*(br/100.0)), (uint8_t)(pieces[T5-1].color.b*(br/100.0)));
				break;
			case T6:
				WS2812B_matrix_set(row, col_board, (uint8_t)(pieces[T6-1].color.r*(br/100.0)), (uint8_t)(pieces[T6-1].color.g*(br/100.0)), (uint8_t)(pieces[T6-1].color.b*(br/100.0)));
				break;
			case T7:
				WS2812B_matrix_set(row, col_board, (uint8_t)(pieces[T7-1].color.r*(br/100.0)), (uint8_t)(pieces[T7-1].color.g*(br/100.0)), (uint8_t)(pieces[T7-1].color.b*(br/100.0)));
				break;
			default:
				break;
			}
		}
	}
}
void drawer_init(uint8_t nFieldWidth, uint8_t nFieldHeight, color_t* colors){
	board_w = nFieldWidth;
	board_h = nFieldHeight;
	pieces[0].piece = PIECE_I_H;
	pieces[0].color = colors[0];

	pieces[1].piece = PIECE_T_H;
	pieces[1].color = colors[1];

	pieces[2].piece = PIECE_O_H;
	pieces[2].color = colors[2];

	pieces[3].piece = PIECE_S_H;
	pieces[3].color = colors[3];

	pieces[4].piece = PIECE_Z_H;
	pieces[4].color = colors[4];

	pieces[5].piece = PIECE_L_H;
	pieces[5].color = colors[5];

	pieces[6].piece = PIECE_J_H;
	pieces[6].color = colors[6];

	timerInit();
	timerperid = timerGetId();
	timerStart(timerperid, TIMER_MS2TICKS(500), TIM_MODE_PERIODIC, tim_per_cb);
	timerStop(timerperid);

	WS2812B_init();

}
