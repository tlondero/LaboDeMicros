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

/************************************************************************
 * TABLA DE PINES CON MODULOS FTM PARA LA K64F DE 100 PINES
 ************************************************************************/

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

#if NXP_K64F_LQFP100 == 1
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

#endif
/************************************************************************
 ************************************************************************
 ************************************************************************/

PCRstr UserPCR;
static uint8_t FTMX_INIT[4];
static void (*CALLBACK[FTM_PIN_CANT])(void);
static uint8_t CALLBACK_EN[FTM_PIN_CANT];
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

#if CALLBACK_MODE == 1
	for (i = 0; i < CANT_CHANNELS - 1; i++) {
		if (status == STATUS_CHF[i]) {	//Me fijo que canal fue
			for(j = MODULE0_ID_START; j < MODULE0_ID_END+1; j++){
				if((CALLBACK_EN[FTM_PINOUT[j].ID]) && (FTM_PINOUT[j].MODULE == 0) && (FTM_PINOUT[j].CHANNEL == i)){
					CALLBACK[FTM_PINOUT[j].ID]();	//Llamo su callback guardado
				}
			}
		}
	}
#endif
}
__ISR__ FTM1_IRQHandler(void) {
	uint32_t status;
	uint8_t i;
	uint8_t j;

	status = FTM1->STATUS; //Capturo flags de interrupcion de todos los canales
	FTM1->STATUS = 0;	//Limpio todos los flags

#if CALLBACK_MODE == 1
	for (i = 0; i < CANT_CHANNELS - 1; i++) {
			if (status == STATUS_CHF[i]) {	//Me fijo que canal fue
				for(j = MODULE1_ID_START; j < MODULE1_ID_END+1; j++){
					if((CALLBACK_EN[FTM_PINOUT[j].ID]) && (FTM_PINOUT[j].MODULE == 1) && (FTM_PINOUT[j].CHANNEL == i)){
						CALLBACK[FTM_PINOUT[j].ID]();	//Llamo su callback guardado
					}
				}
			}
		}
#endif
}
__ISR__ FTM2_IRQHandler(void) {
	uint32_t status;
	uint8_t i;
	uint8_t j;

	status = FTM2->STATUS; //Capturo flags de interrupcion de todos los canales
	FTM2->STATUS = 0;	//Limpio todos los flags

#if CALLBACK_MODE == 1
	for (i = 0; i < CANT_CHANNELS - 1; i++) {
			if (status == STATUS_CHF[i]) {	//Me fijo que canal fue
				for(j = MODULE2_ID_START; j < MODULE2_ID_END+1; j++){
					if((CALLBACK_EN[FTM_PINOUT[j].ID]) && (FTM_PINOUT[j].MODULE == 2) && (FTM_PINOUT[j].CHANNEL == i)){
						CALLBACK[FTM_PINOUT[j].ID]();	//Llamo su callback guardado
					}
				}
			}
		}
#endif
}
__ISR__ FTM3_IRQHandler(void) {
	uint32_t status;
	uint8_t i;
	uint8_t j;

	status = FTM3->STATUS; //Capturo flags de interrupcion de todos los canales
	FTM3->STATUS = 0;	//Limpio todos los flags

#if CALLBACK_MODE == 1

	for (i = 0; i < CANT_CHANNELS - 1; i++) {
			if (status == STATUS_CHF[i]) {	//Me fijo que canal fue
				for(j = MODULE3_ID_START; j < MODULE3_ID_END+1; j++){
					if((CALLBACK_EN[FTM_PINOUT[j].ID]) && (FTM_PINOUT[j].MODULE == 3) && (FTM_PINOUT[j].CHANNEL == i)){
						CALLBACK[FTM_PINOUT[j].ID]();	//Llamo su callback guardado
					}
				}
			}
		}
#endif
}

/* @brief: Continua el clock de FTM para un modulo entero.
 * @param  module: a que modulo se le quiere parar el clock.
 */
void FTMStartClock(uint8_t module) {
	FTM_POINTERS[module]->SC |= FTM_SC_CLKS(SC_CLKS_SYSCLK);
}

/* @brief: Para el clock de FTM para un modulo entero.
 * @param  module: que modulo se le quiere parar el clock.
 */
void FTMStopClock(uint8_t module) {
	FTM_POINTERS[module]->SC |= FTM_SC_CLKS(SC_CLKS_OFF);
}

/* @brief: Inicializa un pin con las especificaciones pedidas en data.
 * @param  port: el puerto del pin que se quiere inicializar
 * @param  num: el numero del puerto  del pin que se quiere inicializar
 * @param  data: las especificaciones para la inicializacion, ver la estructura FTM_DATA.
 * @returns: id de la FTM. Si hubo error se retorna el define FTM_NO_SPACE.
 */
uint8_t FTMInit(uint8_t port, uint8_t num, FTM_DATA data) {
	int8_t id=0;
	uint8_t i;
	uint8_t module;
	uint8_t channel;
	uint8_t mux;
	PCRstr UserPCR;
	PORT_Type * port_ptr;

	/*
	 * IDENTIFICACION DE MODULO/CANAL
	 */
	i = 0;
	while((FTM_PINOUT[i].PORT != port) || (FTM_PINOUT[i].NUM != num)){
		if(++i ==  FTM_PIN_CANT){
			id = FTM_NO_SPACE;
			break;
		}
	}

	if(id != FTM_NO_SPACE){

		mux = FTM_PINOUT[i].MUX;
		channel = FTM_PINOUT[i].CHANNEL;
		module = FTM_PINOUT[i].MODULE;
		id = FTM_PINOUT[i].ID;
		port_ptr = PORT_SELECTORS[port];

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
				SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;	//clock-gating
				NVIC_EnableIRQ(FTM3_IRQn); 			//nvic-enable
				break;
			default:
				break;
			}

			FTM_POINTERS[module]->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | (1<<channel);//LDOK enable CH para escribir en los buffers de MOD, CNTIN y CNV
			UserPCR.PCR = false; // seteo en falso por default
			UserPCR.FIELD.DSE = true; //Drive Streght Enable
			UserPCR.FIELD.MUX = mux; // pongo la alterniativa de mux de mi flex timer (guardado en la tabla predefinida)
			UserPCR.FIELD.IRQC = PORT_eDisabled; //desactivo las interrupciones
			PORT_Configure2(port_ptr, num, UserPCR);
		}

		/*
		 * INICIALIZACION POR CANAL
		 */
		FTM_POINTERS[module]->MODE = (FTM_POINTERS[module]->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);
		FTM_POINTERS[module]->SC = (FTM_POINTERS[module]->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(data.PSC);
		FTM_POINTERS[module]->CNTIN = data.CNTIN;						//CNTIN
		FTM_POINTERS[module]->CNT = data.CNT;							//CNT
		FTM_POINTERS[module]->MOD = data.MODULO;						//MOD

		//SET MSB:A EN EL MODO PEDIDO
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
			FTMSetCnV(id, data.CNV);
			break;
		default:
			break;
		}

		FTMSetInterruptMode(id, 1);

#if CALLBACK_MODE == 1
		/*
		 * GUARDO EL CALLBACK PROVISTO
		 */
		CALLBACK_EN[id] = data.useCallback;
		CALLBACK[id] = data.CALLBACK;
#endif

		/*
		 * ACTIVO LA SINCRONIZACION POR SOFTWARE DE MOD, CNTIN Y CNV
		 */
		FTMSetSoftwareSync(id); //TODO: Cuando se implementen las interrupciones por hardware, hacer que se pueda pedir una u otra.

		if (FTMX_INIT[module] == 0) {
			FTMStartClock(module);	//Comienzo clock del FTM.
			FTMX_INIT[module] = 1;
		}
	}
	return id;
}

/* @brief: Set del CnV
 * @param  id: id de la FTM
 * @param cnv
 */
void FTMSetCnV(uint8_t id, uint16_t cnv){
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->CONTROLS[FTM_PINOUT[id].CHANNEL].CnV = FTM_CnV_VAL(cnv);
}

/* @brief: Set del prescaler o divisor de frecuencias
 * @param  id: id de la FTM
 * @param PSC
 */
void FTMSetPSC(uint8_t id, uint16_t PSC){
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->SC = (FTM_POINTERS[FTM_PINOUT[id].MODULE]->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(PSC);
}

/* @brief: Set del MODULO
 * @param  id: id de la FTM
 * @param MOD
 */
void FTMSetMOD(uint8_t id, uint16_t MOD){
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->MOD = MOD;

}

/* @brief: Habilita (1) o deshabilita (0) las interrupciones
 * @param  id: id de la FTM
 * @param mode: 1 para habilitar, 0 para deshabilitar
 */
void FTMSetInterruptMode(uint8_t id, uint8_t mode){
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->CONTROLS[FTM_PINOUT[id].CHANNEL].CnSC = (FTM_POINTERS[FTM_PINOUT[id].MODULE]->CONTROLS[FTM_PINOUT[id].CHANNEL].CnSC
		& ~FTM_CnSC_CHIE_MASK) | FTM_CnSC_CHIE(mode);
}

/* @brief: configura los registros de SYNC y SYNCONF para poder
 * realizar la sincronización por software de la escritura en los registros de MOD, CNV y CNTIN
 * @param  id: id de la FTM
 */
void FTMSetSoftwareSync(uint8_t id){
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->SYNCONF |= ((FTM_SYNCONF_SYNCMODE(1) | FTM_SYNCONF_SWWRBUF(1) | FTM_SYNCONF_SWRSTCNT(1)));
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->SYNC |= FTM_SYNC_CNTMAX(1);

	switch(FTM_PINOUT[id].CHANNEL){
	case 0:
	case 1:
		FTM_POINTERS[FTM_PINOUT[id].MODULE]->COMBINE |= FTM_COMBINE_SYNCEN0(1);
		break;
	case 2:
	case 3:
		FTM_POINTERS[FTM_PINOUT[id].MODULE]->COMBINE |= FTM_COMBINE_SYNCEN1(1);
		break;
	case 4:
	case 5:
		FTM_POINTERS[FTM_PINOUT[id].MODULE]->COMBINE |= FTM_COMBINE_SYNCEN2(1);
		break;
	case 6:
	case 7:
		FTM_POINTERS[FTM_PINOUT[id].MODULE]->COMBINE |= FTM_COMBINE_SYNCEN3(1);
		break;

	default: break;
	}


}

/* @brief: Set del bit que comienza la sincronización para la escritura
 * en los registros de MOD, CNV y CNTIN
 * @param  id: id de la FTM
 * @param cnv
 */
void FTMSetSWSYNC(uint8_t id){
	FTM_POINTERS[FTM_PINOUT[id].MODULE]->SYNC |= FTM_SYNC_SWSYNC(1);
}
