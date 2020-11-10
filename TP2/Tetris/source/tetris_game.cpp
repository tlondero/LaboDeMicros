//
// Created by Msrt on 09/11/2020.
//

#include "tetris_game.h"

/******************************
 * ENUMS and TYPEDEF
 *****************************/

typedef enum {
	IDLE, RUNNING, PAUSED, GAME_OVER
} game_status;

typedef enum {
	R0, R90, R180, R270
} tetro_rotation;

typedef struct tetris_t{
	//Board
	uint8_t board_w;
	uint8_t board_h;
	unsigned char* board;

	//Game settings
	game_difficulty difficulty;

	//Game info
	uint8_t current_x_pos;	//CURRENT TOP LEFT CORNER OF THE TETROMINO PIECE
	uint8_t current_y_pos;
	uint8_t current_rotation; //R0 R90 R180 R270
	uint8_t current_piece;   //0...6
	uint8_t game_status;	//RUNNING PAUSED GAME_OVER IDLE

	//User input flags
	bool bForceDown = false;
	bool bRotateHold = true;

	//Score
	uint8_t score;
}Tetris;

//PRIVATE TETRIS API DECLARATIONS
void _tetris_clean_board(void);
uint8_t _tetris_rotate(uint8_t px, uint8_t py, tetro_rotation r);


/************** TETRIS GAME ********************/
static Tetris tetris_game;
/************** TETRIS GAME ********************/

/*Tetrominos*/
static const char* tetromino[] = { "..X...X...X...X.",
								   "..X..XX...X.....",
								   ".....XX..XX.....",
								   "..X..XX..X......",
								   ".X...XX...X.....",
								   ".X...X...XX.....",
								   "..X...X..XX....." };

#if DEV_MODE

static int nScreenWidth = 80;			// Console Screen Size X (columns)
static int nScreenHeight = 30;			// Console Screen Size Y (rows)

void print_tetro(void) {
	int i = 0;
	for (i = 0; i < LEN(tetromino); i++)
		cout << tetromino[i] << endl;
}
void fill_board(void) {
	int i, j;
	for (i = 0; i < tetris_game.board_w; i++)
		for (j = 0; j < tetris_game.board_h; j++)
			tetris_game.board[j + tetris_game.board_h * i] = 'A';

}


#endif

//PUBLIC TETRIS API
/*******Game control********/
//void tetris_move_right(void) {
//
//}
//void tetris_move_left(void);
//void tetris_move_down(void);
//void tetris_rotate_piece(void);

/*********Context control******/
void tetris_init(uint8_t board_w, uint8_t board_h) {
	tetris_game.board_h = board_h;
	tetris_game.board_w = board_w;
	tetris_game.board = (unsigned char*)calloc(board_h * board_w, sizeof(unsigned char));
	tetris_game.score = 0;
	tetris_game.game_status = IDLE;
	tetris_set_difficulty(EASY);
}

void tetris_begin_game(void) {
	tetris_game.game_status = RUNNING;
}

void tetris_pause_game(void) {
	tetris_game.game_status = PAUSED;
}

void tetris_resume_game(void) {
	tetris_game.game_status = RUNNING;
}

void tetris_restart_game(void) {
	_tetris_clean_board();
	tetris_set_difficulty(EASY);
	tetris_game.score = 0;
	tetris_game.game_status = RUNNING;
}

/*********Game settings*********/
void tetris_set_difficulty(game_difficulty difficulty) {
	tetris_game.difficulty = difficulty;
}

/******Game status******/
uint8_t tetris_get_score(void) {
	return tetris_game.score;
}


/***Graphics engine stuff***/
const unsigned char* tetris_get_board(void)
{
	return (const unsigned char*)tetris_game.board;
}

//PRIVATE TETRIS API DEFINITIONS
void _tetris_clean_board(void) {
	// TODO:implementar
	return;
}

uint8_t _tetris_rotate(uint8_t px, uint8_t py, tetro_rotation r) {

	int pi = 0;
	switch (r)
	{
	case R0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
									//12 13 14 15

	case R90: // 90 degrees			//12  8  4  0
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
									//15 11  7  3

	case R180: // 180 degrees			//15 14 13 12
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
									// 3  2  1  0

	case R270: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}

