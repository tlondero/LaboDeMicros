/*
 * display_tb.c
 *
 *  Created on: 28 sep. 2020
 *      Author: Msrt
 */
#include "display_tb.h"


static void helper_test_display029 (void){
	static int counter = 0;

//	dispSendChar(counter+'0', counter%3);

	dispSendChar('0', 0);
	dispSendChar('9', 1);
	dispSendChar('2', 2);
	dispSendChar('3', 3);

	/*if (counter > 9)
		counter = 0;
	else{
		counter++;
	}*/
}
static tim_id_t id_029;
void test_display029(void){
	dispInit();
	//dispBrightness(100);
	id_029 = timerGetId();
	timerStart(id_029, 15, TIM_MODE_PERIODIC, helper_test_display029);
}

