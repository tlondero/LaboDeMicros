/*
 * main.c
 *
 *  Created on: May 1, 2015
 *      Author: Juan Pablo VEGA - Laboratorio de Microprocesadores
 */

#include "hardware.h"
#include "PORT.h"
#include "GPIO.h"
#include "FTM.h"
#include "UART.h"
#include "ADC.h"
#include "SysTick.h"

#include <stdint.h>
#include <stdbool.h>

#include "MK64F12.h"

#define __FOREVER__ 	for(;;)

void App_Run(void);

bool startConv;

int main(void) {
	hw_Init();
	PORT_Init();
	GPIO_Init();
	UART_Init();
	SysTick_Init();

	ADC_Init(ADC0, ADC_b12, ADC_c4);

	startConv = false;

	/*//Dani hace esto para ver que la app arrancó
	 UART_Send_Data('A');
	 UART_Send_Data('B');
	 UART_Send_Data('C');
	 */

	// 		hw_DisableInterrupts();
	// Enable interrupts
	hw_EnableInterrupts();
	__FOREVER__
		App_Run();

}

void App_Run(void) {

	uint8_t data;

	if (!startConv) {
		//ADC0_DP0 SE
		ADC_Start(ADC0, 0x00, ADC_mA);
		startConv = true;
	}

	if (ADC_IsReady(ADC0)) {
		startConv = false;
		data = ADC_getData(ADC0);
		UART_Send_Data(data);
	}
}
