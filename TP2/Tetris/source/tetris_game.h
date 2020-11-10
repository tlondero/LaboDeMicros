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

typedef enum {
	EASY, MEDIUM, HELL
} game_difficulty;

//AVAILABLE TOKENS TO DRAW THE BOARD
typedef enum {
	EMPTY, T1, T2, T3, T4, T5, T6, BORDER
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
void tetris_init(uint8_t board_w, uint8_t board_h);
void tetris_begin_game(void);
void tetris_pause_game(void);
void tetris_resume_game(void);
void tetris_restart_game(void);

/*********Game settings*********/
void tetris_set_difficulty(game_difficulty difficulty); //EASY-MEDIUM-HELL

/******Game status******/
uint8_t tetris_get_score(void);

/***Graphics engine stuff***/
//Gives the front end access to the board but cannot change it
board_ptr tetris_get_board(void);

/****Debug functions****/
void tetris_print_board(board_ptr board);
#endif //TETRIS_TETRIS_H
