/*
 * timer_tb.h
 *
 *  Created on: 19 sep. 2020
 *      Author: Msrt
 */

#ifndef HEADERS_TIMER_TB_H_
#define HEADERS_TIMER_TB_H_

/**********************************
 * INIT AND CALLBACKS
 */
#include "headers/timer.h"
#include "headers/gpio.h"
#include "headers/board.h"

#include <stdbool.h>

//Timer startup and gpio startup
void test_init(void);

//BLOCKING DELAY
void test_block(pin_t pin, ttick_t delay_, bool led); // test

//SINGLE SHOT TEST
void test_singleshot(pin_t pin, ttick_t delay); //test
void test_callback_single(void);                //helper callback

//PERIODIC TIMER TEST
void test_periodic(pin_t pin, ttick_t period);

void test_pausa(pin_t pin, ttick_t delay);

void test_resume(ttick_t delay1, ttick_t delay2);

// void test_reset(void);

#endif /* HEADERS_TIMER_TB_H_ */
