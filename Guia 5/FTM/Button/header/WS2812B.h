/*
 * WS2812B.h
 *
 *  Created on: Nov 14, 2020
 *      Author: Acer
 */

#ifndef WS2812B_H_
#define WS2812B_H_

#include <stdint.h>
#include <stdbool.h>

void WS2812B_init(void);
void WS2812B_matrix_set(uint8_t color, uint8_t brightness, uint8_t row, uint8_t col);

#endif /* WS2812B_H_ */
