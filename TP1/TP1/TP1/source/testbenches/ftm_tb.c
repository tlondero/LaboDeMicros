/*
 * ftm_tb.c
 *
 *  Created on: Oct 19, 2020
 *      Author: Acer
 */

#include "drivers/headers/FTM.h"

typedef struct {
	uint8_t PORT;
	uint8_t NUM;
}FTM_TB_PORTS;
enum { PA, PB, PC, PD, PE };
static const FTM_TB_PORTS PORTS[30]={             {PA,3},		//0
														 {PC,1},		//1
														 {PA,4},		//2
														 {PC,2},		//3
														 {PA,5},		//4
														 {PC,3},		//5
														 {PC,4},		//6
														 {PD,4},		//7
														 {PA,0},		//8
														 {PD,5},		//9
														 {PA,1},		//10
														 {PD,6},		//11
														 {PA,2},		//12
														 {PD,7},		//13
														 {PA,12},		//14
														 {PB,0},		//15
														 {PA,13},		//16
														 {PB,1},		//17
														 {PB,18},		//18
														 {PB,19},		//19
														 {PE,5},		//20
														 {PD,0},		//21
														 {PE,6},		//22
														 {PD,1},		//23
														 {PD,2},		//24
														 {PD,3},		//25
														 {PC,8},		//26
														 {PC,9},		//27 //probado
														 {PC,10},		//28
														 {PC,11}};		//29

void FTM_TB_APP_INIT(void){
	FTM_DATA data;
	data.CNTIN = 0;
	data.CNV = 0xFFFF/2;
	data.EPWM_LOGIC = FTM_lAssertedHigh;
	data.MODE = FTM_mPulseWidthModulation;
	data.MODULO = 0xFFFF;
	data.PSC = FTM_PSC_x1;
	FTMInit(PORTS[27], PORTS[27], data);
}

void FTM_TB_APP_RUN(void){

}
