/*
 * display_tb.c
 *
 *  Created on: 28 sep. 2020
 *      Author: Msrt
 */
#include "display_tb.h"

static int counter = 0;

static int counter2 = 0;
static void helper_test_display029_2(void){
	counter++;
	if (counter > 9){
		counter = 0;
		counter2 +=1;
		if (counter2 > 9){
			counter2 = 0;
		}
	}
}

static void helper_test_display029(void){
	//static int counter = 0;
	static int a = 1;
//	dispSendChar(counter+'0', 1);
	if (a) //(counter%2)
		{
			dispSendChar(counter2 +'0', 0);
			a=0;
		}

	else
		{
			dispSendChar(counter+'0', 1);
			a=1;
		}

//	dispSendChar('0', 0);
//dispSendChar('9', 1);
//dispSendChar('2', 2);
//dispSendChar('3', 3);
/*
	if (counter >= 9)
		counter = 0;
	else{
		counter++;
	}*/
}
static tim_id_t id_029;
static tim_id_t id_counter;

void test_display029(void){
	dispInit();
	//dispBrightness(100);
	id_029 = timerGetId();
	id_counter = timerGetId();
	timerStart(id_029, 10, TIM_MODE_PERIODIC, helper_test_display029);
	timerStart(id_counter, 1000, TIM_MODE_PERIODIC, helper_test_display029_2);
}

void test_multiplexing(void){
	dispInit();
	dispSendChar('0', 0);
	dispSendChar('1', 1);
	dispSendChar('2', 2);
	dispSendChar('3', 3);
	dispClearAll();
	dispSendChar('4', 0);
	dispSendChar('5', 1);
	dispSendChar('6', 2);
	dispSendChar('7', 3);

}




