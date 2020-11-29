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
void drawer_change_brightness(uint8_t br);
void drawer_change_piece(char p, uint8_t r, uint8_t g, uint8_t b);
void drawer_draw_gameover(uint8_t score);
void drawer_update_board(unsigned char* board);

#endif /* DRAWER_H_ */
