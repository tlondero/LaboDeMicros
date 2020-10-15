/*
 * display_tb.h
 *
 *  Created on: 28 sep. 2020
 *      Author: Msrt
 */

#ifndef TESTS_DISPLAY_TB_DISPLAY_TB_H_
#define TESTS_DISPLAY_TB_DISPLAY_TB_H_

#include "../source/drivers/headers/timer.h"
#include "../source/drivers/headers/Display.h"


void test_init_display(void);
//numbers from 0 to 1 in a single display
void test_display029(void);
void test_multiplexing(void);
#endif /* TESTS_DISPLAY_TB_DISPLAY_TB_H_ */
