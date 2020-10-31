/*
 * ftm_tb.c
 *
 *  Created on: Oct 19, 2020
 *      Author: Acer
 */

#include "drivers/headers/FTM.h"
#include "drivers/headers/button.h"

typedef struct {
	uint8_t PORT;
	uint8_t NUM;
}FTM_TB_PORTS;
enum { FTM_TB_PA, FTM_TB_PB, FTM_TB_PC, FTM_TB_PD, FTM_TB_PE };
static const FTM_TB_PORTS PORTS[30]={    {FTM_TB_PA,3},		//0 //probado
										 {FTM_TB_PC,1},		//1 //probado
										 {FTM_TB_PA,4},		//2
										 {FTM_TB_PC,2},		//3 //probado
										 {FTM_TB_PA,5},		//4
										 {FTM_TB_PC,3},		//5
										 {FTM_TB_PC,4},		//6
										 {FTM_TB_PD,4},		//7
										 {FTM_TB_PA,0},		//8
										 {FTM_TB_PD,5},		//9
										 {FTM_TB_PA,1},		//10 //probado
										 {FTM_TB_PD,6},		//11
										 {FTM_TB_PA,2},		//12 //probado
										 {FTM_TB_PD,7},		//13
										 {FTM_TB_PA,12},	//14
										 {FTM_TB_PB,0},		//15
										 {FTM_TB_PA,13},	//16
										 {FTM_TB_PB,1},		//17
										 {FTM_TB_PB,18},	//18 //probado
										 {FTM_TB_PB,19},	//19 //probado
										 {FTM_TB_PE,5},		//20
										 {FTM_TB_PD,0},		//21
										 {FTM_TB_PE,6},		//22
										 {FTM_TB_PD,1},		//23
										 {FTM_TB_PD,2},		//24
										 {FTM_TB_PD,3},		//25
										 {FTM_TB_PC,8},		//26 //probado
										 {FTM_TB_PC,9},		//27 //probado
										 {FTM_TB_PC,10},	//28
										 {FTM_TB_PC,11}};	//29
static int8_t ftmid;
static int8_t buttonid;
void FTM_TB_APP_INIT(void){
	FTM_DATA data;
	data.CNTIN = 0;
	data.CNV = 0xFFFF/2;
	data.EPWM_LOGIC = FTM_lAssertedHigh;
	data.MODE = FTM_mPulseWidthModulation;
	data.MODULO = 0xFFFF;
	data.PSC = FTM_PSC_x1;
	ftmid = FTMInit(PORTS[10].PORT, PORTS[10].NUM, data);

	buttonid = ButtonInit(PORTNUM2PIN(PA, 4), INPUT_PULLUP,LOW_WHEN_PRESSED);
}

void FTM_TB_APP_RUN(void){

	const ButtonEvent * evsw1 = ButtonGetEvent(buttonid);


	    while((*evsw1) != EOQ){
		switch (*evsw1){
			case LKP:

				break;
			case PRESS:

				break;

			case RELEASE:


				break;
			case SKP:
				FTMSetCnV(ftmid, 0x1000);
				FTMSetSWSYNC(ftmid);
				break;
			default:

				break;
			}
		evsw1++;
	    }
}
