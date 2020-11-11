//
// Created by Msrt on 09/11/2020.
//

#ifndef TETRIS_TETRIS_H
#define TETRIS_TETRIS_H

#include <stdint.h>
#include <stdbool.h>

#define DEV_MODE 1
#if DEV_MODE
#include <iostream>
#define LEN(x) sizeof(x)/sizeof(x[0])

using namespace std;

void print_tetro(void);
#endif

/******************************
 * ENUMS and TYPEDEF
 *****************************/
typedef const uint8_t* board_ptr;


//AVAILABLE TOKENS TO DRAW THE BOARD
typedef enum {
	EMPTY=0, T1, T2, T3, T4, T5, T6, BORDER=9
}board_token;

/********************************
* PUBLIC TETRIS API DECLARATIONS
********************************/

/*******Game control********/
void tetris_move_right(void);
void tetris_move_left(void);
void tetris_move_down(void);
void tetris_rotate_piece(void);

/*********Context control******/
void tetris_init(uint8_t board_w, uint8_t board_h); //RUN THIS ONE FIRST
void tetris_begin_game(void); //RUN THIS ONE THEN
void tetris_pause_game(void);
void tetris_resume_game(void);
void tetris_restart_game(void);
void tetris_update_board(void); //RUN ALWAYS AT THE END OF THE GAME LOOP
void tetris_on_exit(void);

/*********Game settings*********/
void tetris_set_difficulty(uint8_t pieces2nextLevel); 

/******Game status******/
uint8_t tetris_get_score(void);

/***Graphics engine stuff***/
//Gives the front end access to the board but cannot change it
board_ptr tetris_get_board(void);

/****Debug functions****/
void tetris_print_board(board_ptr board);
#endif //TETRIS_TETRIS_H
