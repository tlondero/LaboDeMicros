/*
 * POTE_ADC.c
 *
 *  Created on: Nov 28, 2020
 *      Author: Acer
 */

#include <../header/drivers/POTE_ADC.h>
#include <../header/drivers/timer.h>

#define TIMER_MS 500
#define MAX_ADC 4096
#define THRESHOLD 310
#define ADC_0_DP0_CHANNEL 0x00

static int8_t timerid;
static ADCData_t data;
static ADCData_t prev_data;
void timcb(void){
	ADC_Start(ADC0, ADC_0_DP0_CHANNEL, ADC_mA);
}

void PoteInit(void){
	ADC_Init(ADC0, ADC_b12, ADC_c4);
	prev_data = 0;
	data = 0;
	timerInit();
	timerStart(timerid, TIMER_MS, TIM_MODE_PERIODIC, timcb);
}

void PoteGetEvent(package* pkg){
	if (ADC_IsReady(ADC0)) {
		data = ADC_getData(ADC0);
		if(data != prev_data){

			if(data < (prev_data-THRESHOLD)  && (data >= 0)){
				pkg->action.left = true;
				prev_data = data;
			}
			else if(data >= (prev_data+THRESHOLD) && (data <= MAX_ADC)){
				pkg->action.right = true;
				prev_data = data;

			}
		}
	}

}
