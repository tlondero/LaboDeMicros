/*
 * utility.h
 *
 *  Created on: Oct 18, 2020
 *      Author: Guido
 */

#ifndef APP_HEADERS_UTILITY_H_
#define APP_HEADERS_UTILITY_H_
#include <stdint.h>
//transforms de array to a number
uint32_t transformToNum(uint8_t* data, uint8_t count);
char* num2str(num);
uint32_t pwr(uint32_t num, uint32_t exp);
#endif /* APP_HEADERS_UTILITY_H_ */
