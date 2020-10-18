/*
 * utility
 *
 *  Created on: Oct 18, 2020
 *      Author: Guido
 */
#include "../headers/utility.h"

/////////////////utility
uint32_t transformToNum(uint8_t* data, uint8_t count) {
	uint32_t ret = 0;
	uint32_t aux = 0;
	uint8_t i = 0;
	for (i = 0; i < count; i++) {
		aux = 0;
		aux = data[count - 1 - i] * pwr(10, i);
		ret += aux;
	}
	return ret;
}

uint32_t pwr(uint32_t num, uint32_t exp) {
	uint32_t ret = 1;
	uint8_t i = 0;
	for (i = 0; i < exp; i++) {
		ret *= num;
	}
	return ret;
}
