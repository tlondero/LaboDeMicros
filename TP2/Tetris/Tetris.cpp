#include "source/tetris_game.h"
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 40;			// Console Screen Size Y (rows)
wstring tetromino[7];
int nFieldWidth = 10;
int nFieldHeight = 9;
unsigned char* pField = nullptr;

int Rotate(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
									//12 13 14 15

	case 1: // 90 degrees			//12  8  4  0
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
									//15 11  7  3

	case 2: // 180 degrees			//15 14 13 12
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
									// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}


bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	// All Field cells >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					// In Bounds so do collision check
					if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
						return false; // fail on first hit
				}
			}
		}

	return true;
}

int main()
{
	time_t t;
	srand((unsigned)time(&t));
	//Step 1- Init tetris
	tetris_init(nFieldWidth, nFieldHeight);
	//Step 2- Begin game
	tetris_begin_game(); //Sets game mode to RUNNING

	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) { 
		screen[i] = L' ';
	};
	board_ptr tetris_board = tetris_get_board(); //Allows the front end to take a peak at the game. Only watching is allowed
	tetris_print_board(tetris_board);


	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
		// Game Logic
	bool bKey[5];
	bool bForceDown = false;
	bool bRotateHold = true;
	vector<int> vLines;
	bool bGameOver = false;

	while (!bGameOver) // Main Loop
	{
		this_thread::sleep_for(100ms); // Small Step = 1 Game Tick

		// Input ========================
		for (int k = 0; k < 5; k++)								// R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28ZS"[k]))) != 0;

		// Handle player movement	
		if (bKey[0])
			tetris_move_right();
		if (bKey[1])
			tetris_move_left();
		if (bKey[2])
			tetris_move_down();
		if (bKey[4])
			bGameOver = true;

		// Rotate, but latch to stop wild spinning
		if (bKey[3])
		{
			bRotateHold = false;
			tetris_rotate_piece();
		}
		else
			bRotateHold = true;

		tetris_update_board();
		// Draw Field
		for (int y = 0; y < nFieldHeight; y++)
			for (int x = 0; x < nFieldWidth; x++)
				screen[y*nScreenWidth + x] = L" ABCDEFG=#"[tetris_board[y* nFieldWidth + x]];

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	// Oh Dear
	tetris_print_board(tetris_board);
	tetris_on_exit();
	CloseHandle(hConsole);
	system("pause");
	return 0;
}