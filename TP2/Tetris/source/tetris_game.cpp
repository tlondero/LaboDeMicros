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
} _tetro_rotation;

typedef uint8_t* _board_ptr;

typedef struct tetris_t {
	//Board
	uint8_t board_w;
	uint8_t board_h;
	_board_ptr board;

	//Game settings
	game_difficulty difficulty;

	//Game info
	uint8_t current_x_pos;	//CURRENT TOP LEFT CORNER OF THE TETROMINO PIECE
	uint8_t current_y_pos;
	_tetro_rotation current_rotation; //R0 R90 R180 R270
	uint8_t current_piece;   //0...6
	uint8_t game_status;	//RUNNING PAUSED GAME_OVER IDLE

	//User input flags
	bool bForceDown = false;
	bool bRotateHold = true;

	//Score
	uint8_t score;
}Tetris;

/********************************
* PRIVATE API DECLARATIONS
********************************/

//BOARD METHODS
void _tetris_clean_board(void);
void _tetris_set_board_borders(void);

//TETROMINOS METHODS
uint8_t _tetris_rotate(uint8_t px, uint8_t py, _tetro_rotation r);
static uint8_t _tetris_rotate(uint8_t px, uint8_t py, _tetro_rotation r);

//DEBUG METHODS
#if DEV_MODE
void _tetris_print_board(void); //Prints the current board to the console
#endif

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

/*****************************
* PUBLIC API DEFINITIONS
******************************/

/*********Context control******/
void tetris_init(uint8_t board_w, uint8_t board_h) {
	//Get boad ready
	tetris_game.board_h = board_h;
	tetris_game.board_w = board_w;
	tetris_game.board = (unsigned char*)calloc(board_h * board_w, sizeof(unsigned char));
	_tetris_set_board_borders(); //Sets the board boundaries and the rest of the elements are set to empty

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

/*******Game control********/
void tetris_move_right(void) {
	tetris_game.current_x_pos += _tetris_piece_fits(tetris_game.current_x_pos + 1, tetris_game.current_y_pos);
}
void tetris_move_left(void) {
	tetris_game.current_x_pos += _tetris_piece_fits(tetris_game.current_x_pos -1, tetris_game.current_y_pos);
}
void tetris_move_down(void) {
	tetris_game.current_x_pos += _tetris_piece_fits(tetris_game.current_x_pos, tetris_game.current_y_pos + 1);
}
void tetris_rotate_piece(void) {

}
/*********Game status*********/
uint8_t tetris_get_score(void) {
	return tetris_game.score;
}

/***Graphics engine stuff***/
const unsigned char* tetris_get_board(void)
{
	return (board_ptr)tetris_game.board;
}
/****Debug functions****/
void tetris_print_board(board_ptr board) {
	int i = 0;
	int j = 0;
	for (i = 0; i < tetris_game.board_h; i++) {
		for (j = 0; j < tetris_game.board_w; j++)
		{
			printf("%d", board[i * tetris_game.board_w + j]);
		}
		printf("\n");
	}
}


/*****************************
* PRIVATE API DEFINITIONS
******************************/

//BOARD METHODS
static void _tetris_clean_board(void) {
	int i = 0;
	int j = 0;
	for (i = 0; i < tetris_game.board_h; i++) {
		for (j = 0; j < tetris_game.board_w; j++)
		{
			tetris_game.board[i * tetris_game.board_w + j] = EMPTY;
		}
	}
}

void _tetris_set_board_borders(void) {
	int x = 0;
	int y = 0;
	for (x = 0; x < tetris_game.board_w; x++) {
		for (y = 0; y < tetris_game.board_h; y++) {
		//Going down columns by column
			tetris_game.board[y * tetris_game.board_w + x] = (x == 0 || x == tetris_game.board_w - 1 || y == tetris_game.board_w - 1) ? BORDER : T1;
		}
	}
}

//TETROMINOS METHODS
static uint8_t _tetris_rotate(uint8_t px, uint8_t py, _tetro_rotation r) {

	int pi = 0;
	switch (r%4)
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

bool _tetris_piece_fits(uint8_t nPosX, uint8_t nPosY, _tetro_rotation r) {
	// All Field cells >0 are occupied

	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = _tetris_rotate(tetris_game.current_x_pos, tetris_game.current_y_pos, tetris_game.current_rotation);

			// Get index into field
			int fi = (nPosY + py) * tetris_game.board_w + (nPosX + px);

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (nPosX + px >= 0 && nPosX + px < tetris_game.board_w)
			{
				if (nPosY + py >= 0 && nPosY + py < tetris_game.board_h)
				{
					// In Bounds so do collision check
					if ( (tetromino[tetris_game.current_piece][pi] != '.') && (tetris_game.board[fi] != 0) )
						return false; // fail on first hit
				}
			}
		}

	return true;
}

//DEBUG METHODS
#if DEV_MODE
static void _tetris_print_board() {
	int i = 0;
	int j = 0;
	for (i = 0; i < tetris_game.board_h; i++) {
		for (j = 0; j < tetris_game.board_w; j++)
		{
			printf("%d", tetris_game.board[i * tetris_game.board_w + j]);
		}
		printf("\n");
	}
}
#endif