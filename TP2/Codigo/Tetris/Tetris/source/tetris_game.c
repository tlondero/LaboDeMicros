#include "../header/tetris_game.h"
#include "../header/drivers/random.h"

/******************************
 * ENUMS TYPEDEF
 *****************************/

typedef enum {
	R0, R90, R180, R270
} _tetro_rotation;

typedef enum {
	BACK, FRONT
}which_board;

typedef uint8_t* _board_ptr;

typedef struct tetris_t {
	//Board
	uint8_t board_w;
	uint8_t board_h;
	uint16_t board_size;
	_board_ptr board_back; //This board contains all the fixed tetrominoes
	_board_ptr board_front; //This board will be an exact mirror of the previous one plus the current piece which is moving all around

	//Animations
	void* tetris_animation_callback_1;
	void* tetris_animation_callback_2;
	void* tetris_animation_callback_3;

	//Game info
	int8_t current_x;			//CURRENT TOP LEFT CORNER OF THE TETROMINO PIECE
	int8_t current_y;
	uint8_t current_rotation;	//R0 R90 R180 R270
	uint8_t current_piece;		//1...6
	game_status game_status;	//RUNNING PAUSED GAME_OVER IDLE
	bool lines_avb; //TRUE if there are any lines to delete set
	//Game mechanics related stuff
	uint8_t ticks_passed;
	uint8_t ticks2down;
	uint8_t pieces2nextLevel;

	//User input flags
	bool bForceDown;
	bool bRotateHold;

	//Score
	uint8_t score;

}Tetris;

/********************************
* PRIVATE API DECLARATIONS
********************************/

/*********BOARD METHODS*********/
static _board_ptr _create_board(uint8_t board_w, uint8_t board_h);
static void _tetris_clean_board(void);
static void _tetris_set_board_borders(void);
static void _tetris_lock_piece_down(void);
static void _tetris_draw_current_piece(which_board board);
static void _tetris_check_for_lines(void);
static void _tetris_update_score(uint8_t lines_completed);
/*********TETROMINOS METHODS*********/
static uint8_t _tetris_rotate(uint8_t px, uint8_t py, uint8_t r);
static bool _tetris_piece_fits(int8_t nPosX, int8_t nPosY, uint8_t r);
//DEBUG METHODS

/*********Game Mechanics*********/
static void _tetris_sort_new_piece(void);
static void _tetris_force_piece_down(void);
static void _tetris_update_ticks_count(void);
static void _tetris_remove_and_lower(void);
static void _tetris_delete_lines(void);
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

void print_tetro(void) {
	int i = 0;
	for (i = 0; i < LEN(tetromino); i++)
		cout << tetromino[i] << endl;
}
void fill_board(void) {
	int i, j;
	for (i = 0; i < tetris_game.board_w; i++)
		for (j = 0; j < tetris_game.board_h; j++)
			tetris_game.board_back[j + tetris_game.board_h * i] = 'A';

}

#endif

/*****************************
* PUBLIC API DEFINITIONS
******************************/

/*********Context control******/
void tetris_init(uint8_t board_w, uint8_t board_h) {
   /* Intializes random number generator */
	RandInit();
	srand(RandGet());
	//Get boad ready
	tetris_game.bForceDown = false;
	tetris_game.bRotateHold = true;
	tetris_game.board_h = board_h;
	tetris_game.board_w = board_w;
	tetris_game.board_size = board_h * board_w;
	tetris_game.board_back = _create_board(board_w, board_h);
	tetris_game.board_front = _create_board(board_w, board_h);
	_tetris_set_board_borders(); //Sets the board boundaries and the rest of the elements are set to empty

	//Get player ready to play
	tetris_game.current_x = tetris_game.board_w / 2; //Begin in the middle of the board
	tetris_game.current_y = 0; //Begin at the top
	tetris_game.score = 0;
	tetris_game.game_status = TETRIS_IDLE_ST;

	//Set game mechanics
	tetris_game.lines_avb = false;
	tetris_set_difficulty(EASY);
}

void tetris_begin_game(void) {
	tetris_game.game_status = TETRIS_RUNNING_ST;
	//tetris_game.current_piece = rand() % 7;
	_tetris_sort_new_piece();
	tetris_game.game_status = _tetris_piece_fits(tetris_game.current_x,
												tetris_game.current_y,
												tetris_game.current_rotation)? TETRIS_RUNNING_ST:TETRIS_GAME_OVER_ST;

}

void tetris_pause_game(void) {
	tetris_game.game_status = TETRIS_PAUSED_ST;
}

void tetris_resume_game(void) {
	tetris_game.game_status = TETRIS_RUNNING_ST;
}

void tetris_restart_game(void) {
	_tetris_clean_board();
	_tetris_set_board_borders();
	tetris_game.current_x = tetris_game.board_w / 2; //Begin in the middle of the board
	tetris_game.current_y = 0; //Begin at the top
	tetris_game.score = 0;
	_tetris_sort_new_piece();
	tetris_set_difficulty(EASY);
	tetris_game.score = 0;
	tetris_game.game_status = TETRIS_RUNNING_ST;

}

void tetris_end_game(void) {
	tetris_game.game_status = TETRIS_GAME_OVER_ST;
}

void tetris_update_board(void)
{
	if (tetris_get_game_status() == TETRIS_RUNNING_ST)
	{
		//Delete lines and move pieces
		if (tetris_game.lines_avb == true) {
			_tetris_delete_lines();
			tetris_game.lines_avb = false;
		}

		//Update ticks counts
		_tetris_update_ticks_count();

		//Copy back to front
		memcpy(tetris_game.board_front, (board_ptr)tetris_game.board_back, sizeof(tetris_game.board_back[0]) * tetris_game.board_size);

		//if it's time force the pieces down
		_tetris_force_piece_down();

		//Draw current piece
		_tetris_draw_current_piece(FRONT);

	}
}

void tetris_on_exit(void) {
	free(tetris_game.board_back);
	free(tetris_game.board_front);
}

/*********Game settings*********/
void tetris_set_difficulty(uint8_t difficulty) {
	tetris_game.ticks2down = difficulty;
}

/*******Game control********/
void tetris_move_right(void) {
	tetris_game.current_x += _tetris_piece_fits(tetris_game.current_x + 1, tetris_game.current_y, tetris_game.current_rotation);
}

void tetris_move_left(void) {
	tetris_game.current_x -= _tetris_piece_fits(tetris_game.current_x - 1, tetris_game.current_y, tetris_game.current_rotation);
}

void tetris_move_down(void) {
	if (_tetris_piece_fits(tetris_game.current_x, tetris_game.current_y + 1, tetris_game.current_rotation))
		tetris_game.current_y += 1;
	else
	{
		//Lock piece in position
		_tetris_lock_piece_down();
	}
}

void tetris_rotate_piece(void) {
	tetris_game.current_rotation += _tetris_piece_fits(tetris_game.current_x, tetris_game.current_y + 1, tetris_game.current_rotation + 1);
}

/*********Game status*********/
uint8_t tetris_get_score(void) {
	return tetris_game.score;
}

uint8_t tetris_get_current_piece(void) {
	return tetris_game.current_piece;
}

uint8_t tetris_get_current_rotation(void) {
	return tetris_game.current_rotation;
}

game_status tetris_get_game_status(void) {
	return tetris_game.game_status;
}

/***Graphics engine stuff***/
const unsigned char* tetris_get_board(void)
{
	return (board_ptr)tetris_game.board_front;
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

/*********BOARD METHODS*********/
static _board_ptr _create_board(uint8_t board_w, uint8_t board_h) {
	return (_board_ptr)calloc(board_h * board_w, sizeof(unsigned char));
}

static void _tetris_clean_board(void) {
	int i = 0;
	int j = 0;
	for (i = 0; i < tetris_game.board_h; i++) {
		for (j = 0; j < tetris_game.board_w; j++)
		{
			tetris_game.board_back[i * tetris_game.board_w + j] = EMPTY;
			tetris_game.board_front[i * tetris_game.board_w + j] = EMPTY;
		}
	}
}

static void _tetris_set_board_borders(void) {
	int x = 0;
	int y = 0;
	for (y = 0; y < tetris_game.board_h; y++) {
		for (x = 0; x < tetris_game.board_w; x++) {
			//Going down columns by column
			tetris_game.board_back[y * tetris_game.board_w + x] = (x == 0 || x == tetris_game.board_w - 1 || y == tetris_game.board_h - 1) ? BORDER : EMPTY;
		}
	}
	memcpy(tetris_game.board_front, (board_ptr)tetris_game.board_back, sizeof(tetris_game.board_back[0]) * tetris_game.board_size);

}

static void _tetris_lock_piece_down(void) {
	_tetris_draw_current_piece(BACK);
	_tetris_check_for_lines();
	_tetris_sort_new_piece();
	return;
}

static void _tetris_draw_current_piece(which_board board) {
	uint8_t px, py;

	for (py = 0; py < 4; py++)
		for (px = 0; px < 4; px++)
			if (tetromino[tetris_game.current_piece][_tetris_rotate(px, py, tetris_game.current_rotation)] != L'.')
			{
				if (board == BACK)
					tetris_game.board_back[(tetris_game.current_y + py) * tetris_game.board_w + (tetris_game.current_x + px)] = tetris_game.current_piece + 1;
				else if (board == FRONT)
				{
					tetris_game.board_front[(tetris_game.current_y + py) * tetris_game.board_w + (tetris_game.current_x + px)] = tetris_game.current_piece + 1;
				}
			}
}

static void _tetris_check_for_lines(void) {
	uint8_t px, py, lines_completed;

	lines_completed = 0;
	for (py = 0; py < 4; py++)
		if (tetris_game.current_y + py < tetris_game.board_h - 1)
		{
			bool bLine = true;
			for (px = 1; px < tetris_game.board_w - 1; px++)
				bLine &= (tetris_game.board_back[(tetris_game.current_y + py) * tetris_game.board_w + px]) != EMPTY;

			if (bLine)
			{
				// Remove Line, set to =
				for (px = 1; px < tetris_game.board_w - 1; px++)
					tetris_game.board_back[(tetris_game.current_y + py) * tetris_game.board_w + px] = LINE;
				lines_completed++;
				tetris_game.lines_avb = true;
			}

		}
	_tetris_update_score(lines_completed);
}

static void _tetris_update_score(uint8_t lines_completed) {
	//TODO
	return;
}

/*********TETROMINOS METHODS*********/
static uint8_t _tetris_rotate(uint8_t px, uint8_t py, uint8_t r) {

	uint8_t pi = 0;
	switch (r % 4)
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

static bool _tetris_piece_fits(int8_t nPosX, int8_t nPosY, uint8_t r) {
	// All Field cells >0 are occupied

	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = _tetris_rotate(px, py, r);

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
					if ((tetromino[tetris_game.current_piece][pi] != '.') && (tetris_game.board_back[fi] != EMPTY))
						return false; // fail on first hit
				}
			}
		}

	return true;
}

/*********Game Mechanics*********/
static void _tetris_sort_new_piece(void) {
	tetris_game.current_piece = rand() % 7;
	tetris_game.current_x = tetris_game.board_w / 2; //Begin in the middle of the board
	tetris_game.current_y = 0; //Begin at the top
	tetris_game.game_status = _tetris_piece_fits(tetris_game.current_x,
												 tetris_game.current_y,
												 tetris_game.current_rotation) ? TETRIS_RUNNING_ST : TETRIS_GAME_OVER_ST;
}
static void _tetris_force_piece_down(void) {
	if(tetris_game.ticks_passed % tetris_game.ticks2down == 0)
		tetris_move_down();
}

static void _tetris_update_ticks_count(void) {
	tetris_game.ticks_passed++;
}

static void _tetris_remove_and_lower(void) {

	return;
}

static void _tetris_delete_lines(void) {
	uint8_t py;
	for (py = 0; py < tetris_game.board_h; py++) {
		if (tetris_game.board_back[py * tetris_game.board_w + 1] == LINE) {

			//tetris_game.board_back[py * tetris_game.board_w + 1] = BORDER;
			uint8_t x, y;
			for (x = 1; x < tetris_game.board_w - 1; x++) {
				for (y = py; y > 0; y--) {
					tetris_game.board_back[y * tetris_game.board_w + x] = tetris_game.board_back[(y-1) * tetris_game.board_w + x];
				}
			}
		}
	}
	tetris_game.lines_avb = false;
}

//DEBUG METHODS
#if DEV_MODE
static void _tetris_print_board() {
	int i = 0;
	int j = 0;
	for (i = 0; i < tetris_game.board_h; i++) {
		for (j = 0; j < tetris_game.board_w; j++)
		{
			printf("%d", tetris_game.board_back[i * tetris_game.board_w + j]);
		}
		printf("\n");
	}
}
#endif
