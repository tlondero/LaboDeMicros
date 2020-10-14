/***************************************************************************//**
 @file     FTM.h
 @brief
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "drivers/headers/FTM.h"
#include "MK64F12.h"
#include "drivers/headers/PORT.h"
#include <stdbool.h>
//FTM0, FTM0_CH0

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SC_CLKS_OFF 0x00
#define SC_CLKS_SYSCLK 0x01
#define FTM_PIN_CANT 30

typedef struct {
	uint8_t MODULE;
	uint8_t CHANNEL;
	uint8_t ALT;
	uint8_t MUX;
	uint8_t PIN;
	uint8_t ID;
	uint8_t PORT;
	uint8_t NUM;
}FTM_Pinout_t;

// Ports
enum { PA, PB, PC, PD, PE };

static const FTM_Pinout_t FTM_PINOUT[FTM_PIN_CANT]={{0,0,0,3,37,0,PA,3},
							 {0,0,1,4,71,1,PC,1},
							 {0,1,0,3,38,2,PA,4},
							 {0,1,1,4,72,3,PC,2},
							 {0,2,0,3,39,4,PA,5},
							 {0,2,1,4,73,5,PC,3},
							 {0,3,0,4,76,6,PC,4},
							 {0,4,0,4,97,7,PD,4},
							 {0,5,0,4,34,8,PA,0},
							 {0,5,1,4,98,9,PD,5},
							 {0,6,0,3,35,10,PA,1},
							 {0,6,1,4,99,11,PD,6},
							 {0,7,0,3,36,12,PA,2},
							 {0,7,1,4,100,13,PD,7},
							 {1,0,0,3,42,14,PA,12},
							 {1,0,1,3,53,15,PB,0},
							 {1,1,0,3,43,16,PA,13},
							 {1,1,1,3,54,17,PB,1},
							 {2,0,0,3,64,18,PB,18},
							 {2,1,0,3,65,19,PB,19},
							 {3,0,0,6,6,20,PE,5},
							 {3,0,1,4,93,21,PD,0},
							 {3,1,0,6,7,22,PE,6},
							 {3,1,1,4,94,23,PD,1},
							 {3,2,0,4,95,24,PD,2},
							 {3,3,0,4,96,25,PD,3},
							 {3,4,0,3,80,26,PC,8},
							 {3,5,0,3,81,27,PC,9},
							 {3,6,0,3,82,28,PC,10},
							 {3,7,0,3,83,29,PC,11}};
static const uint8_t MODULE0_ID_START = 0;
static const uint8_t MODULE0_ID_END = 13;
static const uint8_t MODULE1_ID_START = 14;
static const uint8_t MODULE1_ID_END = 17;
static const uint8_t MODULE2_ID_START = 18;
static const uint8_t MODULE2_ID_END = 19;
static const uint8_t MODULE3_ID_START = 20;
static const uint8_t MODULE3_ID_END = 29;
PCRstr UserPCR;
static uint8_t FTMX_INIT[4];
static void (*CALLBACK[FTM_PIN_CANT])(void);
static CALLBACK_EN[FTM_PIN_CANT];
static const uint32_t STATUS_CHF[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
		0x40, 0x80 };
static FTM_Type * const FTM_POINTERS[4] = { FTM0, FTM1, FTM2, FTM3 };
static PORT_Type * const PORT_SELECTORS[] = {PORTA, PORTB, PORTC, PORTD, PORTE};

__ISR__ FTM0_IRQHandler(void) {
	uint32_t status;
	uint8_t i;
	uint8_t j;

	status = FTM0->STATUS; //Capturo flags de interrupcion de todos los canales
	FTM0->STATUS = 0;	//Limpio todos los flags

	for (i = 0; i < CANT_CHANNELS - 1; i++) {
		if (status == STATUS_CHF[i]) {	//Me fijo que canal fue
			for(j = MODULE0_ID_START; j < MODULE0_ID_END+1; j++){
				if((CALLBACK_EN[FTM_PINOUT[j].ID]) && (FTM_PINOUT[j].MODULE == 0) && (FTM_PINOUT[j].CHANNEL == i)){
					CALLBACK[FTM_PINOUT[j].ID]();	//Llamo su callback guardado
				}
			}
		}
	}

}
__ISR__ FTM1_IRQHandler(void) {
	uint32_t status;
	uint8_t i;
	uint8_t j;

	status = FTM1->STATUS; //Capturo flags de interrupcion de todos los canales
	FTM1->STATUS = 0;	//Limpio todos los flags

	for (i = 0; i < CANT_CHANNELS - 1; i++) {
			if (status == STATUS_CHF[i]) {	//Me fijo que canal fue
				for(j = MODULE1_ID_START; j < MODULE1_ID_END+1; j++){
					if((CALLBACK_EN[FTM_PINOUT[j].ID]) && (FTM_PINOUT[j].MODULE == 1) && (FTM_PINOUT[j].CHANNEL == i)){
						CALLBACK[FTM_PINOUT[j].ID]();	//Llamo su callback guardado
					}
				}
			}
		}
}
__ISR__ FTM2_IRQHandler(void) {
	uint32_t status;
	uint8_t i;
	uint8_t j;

	status = FTM2->STATUS; //Capturo flags de interrupcion de todos los canales
	FTM2->STATUS = 0;	//Limpio todos los flags

	for (i = 0; i < CANT_CHANNELS - 1; i++) {
			if (status == STATUS_CHF[i]) {	//Me fijo que canal fue
				for(j = MODULE2_ID_START; j < MODULE2_ID_END+1; j++){
					if((CALLBACK_EN[FTM_PINOUT[j].ID]) && (FTM_PINOUT[j].MODULE == 2) && (FTM_PINOUT[j].CHANNEL == i)){
						CALLBACK[FTM_PINOUT[j].ID]();	//Llamo su callback guardado
					}
				}
			}
		}
}
__ISR__ FTM3_IRQHandler(void) {
	uint32_t status;
	uint8_t i;
	uint8_t j;

	status = FTM3->STATUS; //Capturo flags de interrupcion de todos los canales
	FTM3->STATUS = 0;	//Limpio todos los flags

	for (i = 0; i < CANT_CHANNELS - 1; i++) {
			if (status == STATUS_CHF[i]) {	//Me fijo que canal fue
				for(j = MODULE3_ID_START; j < MODULE3_ID_END+1; j++){
					if((CALLBACK_EN[FTM_PINOUT[j].ID]) && (FTM_PINOUT[j].MODULE == 3) && (FTM_PINOUT[j].CHANNEL == i)){
						CALLBACK[FTM_PINOUT[j].ID]();	//Llamo su callback guardado
					}
				}
			}
		}
}

void FTMStartClock(uint8_t module) {
	FTM_POINTERS[module]->SC |= FTM_SC_CLKS(SC_CLKS_SYSCLK);
}

void FTMStopClock(uint8_t module) {
	FTM_POINTERS[module]->SC |= FTM_SC_CLKS(SC_CLKS_OFF);
}

uint8_t FTMInit(uint8_t pin, FTM_DATA data) {
	uint8_t i;
	uint8_t module;
	uint8_t channel;
	uint8_t mux;
	uint8_t id;
	PCRstr UserPCR;
	PORT_Type * port_ptr;
	uint32_t num;
	/*
	 * IDENTIFICACION DE MODULO/CANAL
	 */

	for (i = 0; i < FTM_PIN_CANT - 1; i++) {
		if (FTM_PINOUT[i].PIN == pin) {
			pin = FTM_PINOUT[i].PIN;
			mux = FTM_PINOUT[i].MUX;
			channel = FTM_PINOUT[i].CHANNEL;
			module = FTM_PINOUT[i].MODULE;
			id = FTM_PINOUT[i].ID;
			port_ptr = PORT_SELECTORS[FTM_PINOUT[i].PORT];
			num = FTM_PINOUT[i].NUM;
			break;
		}
	}

	/*
	 * INICIALIZACION POR MODULO
	 */
	if (FTMX_INIT[module] == 0) {

		switch (module) {
		case 0:
			SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;	//clock-gating
			NVIC_EnableIRQ(FTM0_IRQn); 			//nvic-enable
			break;
		case 1:
			SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;	//clock-gating
			NVIC_EnableIRQ(FTM1_IRQn); 			//nvic-enable
			break;
		case 2:
			SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;	//clock-gating
			SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;
			NVIC_EnableIRQ(FTM2_IRQn); 			//nvic-enable
			break;
		case 3:
			SIM->SCGC6 |= SIM_SCGC3_FTM3_MASK;	//clock-gating
			NVIC_EnableIRQ(FTM3_IRQn); 			//nvic-enable
			break;
		default:
			break;
		}

		FTM_POINTERS[module]->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;//LDOK enable all CH
		UserPCR.PCR = false; // seteo en falso por default
		UserPCR.FIELD.DSE = true; //Drive Streght Enable
		UserPCR.FIELD.MUX = mux; // pongo la alterniativa de mux de mi flex timer
		UserPCR.FIELD.IRQC = PORT_eDisabled; //desactivo las interrupciones
		PORT_Configure2(port_ptr, num, UserPCR);
		FTM_POINTERS[module]->MODE = (FTM_POINTERS[module]->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);
		FTM_POINTERS[module]->SC = (FTM_POINTERS[module]->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(data.PSC);
		FTM_POINTERS[module]->CNTIN = data.CNTIN;						//CNTIN
		FTM_POINTERS[module]->CNT = data.CNT;							//CNT
		FTM_POINTERS[module]->MOD = data.MODULO;						//MOD
		FTMX_INIT[module] = 1;
	}

	/*
	 * INICIALIZACION POR CANAL
	 */



	//SET MSB:A EN OUTPUT COMPARE
	FTM_POINTERS[module]->CONTROLS[channel].CnSC = (FTM_POINTERS[module]->CONTROLS[channel].CnSC
			& ~(FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK))
			| (FTM_CnSC_MSB((data.MODE >> 1) & 0X01)
					| FTM_CnSC_MSA((data.MODE >> 0) & 0X01));

	//SET ELSB:A
	switch(data.MODE){
	case FTM_mInputCapture:
		FTM_POINTERS[module]->CONTROLS[channel].CnSC =
				(FTM_POINTERS[module]->CONTROLS[channel].CnSC
						& ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK))
						| (FTM_CnSC_ELSB((data.IC_EDGE >> 1) & 0X01)
								| FTM_CnSC_ELSA((data.IC_EDGE >> 0) & 0X01));
		break;
	case FTM_mOutputCompare:
		FTM_POINTERS[module]->CONTROLS[channel].CnSC =
						(FTM_POINTERS[module]->CONTROLS[channel].CnSC
								& ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK))
								| (FTM_CnSC_ELSB((data.OC_EFFECT >> 1) & 0X01)
										| FTM_CnSC_ELSA((data.OC_EFFECT >> 0) & 0X01));
		FTM_POINTERS[module]->CONTROLS[channel].CnV = FTM_CnV_VAL(data.CNV);
		break;
	case FTM_mPulseWidthModulation:
		FTM_POINTERS[module]->CONTROLS[channel].CnSC =
						(FTM_POINTERS[module]->CONTROLS[channel].CnSC
								& ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK))
								| (FTM_CnSC_ELSB((data.EPWM_LOGIC >> 1) & 0X01)
										| FTM_CnSC_ELSA((data.EPWM_LOGIC >> 0) & 0X01));
		FTM_POINTERS[module]->CONTROLS[channel].CnV = FTM_CnV_VAL(data.CNV);
		break;
	default:
		break;
	}

	FTM_POINTERS[module]->CONTROLS[channel].CnSC = (FTM_POINTERS[module]->CONTROLS[channel].CnSC
			& ~FTM_CnSC_CHIE_MASK) | FTM_CnSC_CHIE(1);

	/*
	 * GUARDO EL CALLBACK PROVISTO
	 */

	CALLBACK_EN[id] = data.useCallback;
	CALLBACK[id] = data.CALLBACK;

	return id;
}

void FTMSetCnV(uint8_t id, uint16_t cnv){
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->CONTROLS[FTM_PINOUT[id].CHANNEL].CnV = FTM_CnV_VAL(cnv);
}

void FTMSetPSC(uint8_t id, uint8_t PSC){
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->SC = (FTM_POINTERS[FTM_PINOUT[id].MODULE]->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(PSC);
}
