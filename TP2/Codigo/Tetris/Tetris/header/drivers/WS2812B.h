/*
 * WS2812B.h
 *
 *  Created on: Nov 14, 2020
 *      Author: Acer
 */

#ifndef WS2812B_H_
#define WS2812B_H_
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

/*
 * Brief: Initializes all the drivers responsible for the matrix.
 */
void WS2812B_init(void);

/*
 * Brief: Setter of a single led
 * @param row: row of the led
 * @param col: column of the led
 * @param r: red value 0-255
 * @param g: green value 0-255
 * @param b: blue value 0-255
 */
void WS2812B_matrix_set(uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b);

#endif /* WS2812B_H_ */
