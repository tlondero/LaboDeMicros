/*
 * utility
 *
 *  Created on: Oct 18, 2020
 *      Author: Guido
 */
#define ASCII_NUM_BASE 48
#define DIGIT_MAX_LEN 8
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

char* num2str(uint32_t num){
	int8_t i;
	int8_t j;
	uint32_t id[DIGIT_MAX_LEN];
	char chars[DIGIT_MAX_LEN+1];

	for (i = DIGIT_MAX_LEN; i > 0 ; i--) {
		uint32_t aux = 0;
		if (i == DIGIT_MAX_LEN) {
			id[DIGIT_MAX_LEN - i] = (num / pwr(10, i - 1));
		}
		else {
			for (j = i; j < DIGIT_MAX_LEN; j++) {
				aux += id[j-i] * pwr(10, DIGIT_MAX_LEN - j);
			}
			id[DIGIT_MAX_LEN - i] = (num / pwr(10, i - 1)) - aux;
		}
		chars[DIGIT_MAX_LEN - i] = id[DIGIT_MAX_LEN - i] + ASCII_NUM_BASE;
	}

	chars[8] = '\0';

	return &chars;
}
