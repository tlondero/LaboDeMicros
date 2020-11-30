/*
 * Drawer.h
 *
 *  Created on: Nov 29, 2020
 *      Author: Acer
 */

#ifndef DRAWER_H_
#define DRAWER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "header/tetris_game.h"
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
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/*
 * Brief: Changes brightness of the whole matrix
 * @param br: brightness level 0-100
 */
void drawer_change_brightness(uint8_t br);

/*
 * Brief: Changes the color of a piece
 * @param p: which piece
 * @param r: red 0-255
 * @param g: green 0-255
 * @param b: blue 0-255
 */
void drawer_change_piece(char p, uint8_t r, uint8_t g, uint8_t b);

/*
 * Brief: Draws the gameover screen
 * @param score: score of the game when losing
 */
void drawer_draw_gameover(uint8_t score);

/*
 * Brief: Updates the buffer contents to be transfered via DMA to the FTM module given a board
 * @param board: pointer to the board
 */
void drawer_update_board(board_ptr board);

/*
 * Brief: Initializes all the drivers needed to write on the matrix
 * @param nFieldWidth: width of the matrix
 * @param nFieldHeight: height of the matrix
 * @param colors: default colors of all pieces
 */
void drawer_init(uint8_t nFieldWidth, uint8_t nFieldHeight, color_t* colors);

#endif /* DRAWER_H_ */
