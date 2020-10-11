/***************************************************************************//**
 @file     FTM.h
 @brief
 @author   MAGT + Mariposa + Carpincho
 ******************************************************************************/
void ICInit(void);
void OCInit(void);
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "header/FTM.h"
#include "MK64F12.h"
#include "header/gpio.h"
#include <stdbool.h>
//FTM0, FTM0_CH0

PCRstr UserPCR;
static uint16_t OC1, OC2;

__ISR__ FTM0_IRQHandler(void) {
	OC_ISR();
}

void OC_ISR(void) {
	/// Limpio flag de interrupcion
	static bool variant = false;
	FTM0->CONTROLS[FTM_CH_0].CnSC &= ~FTM_CnSC_CHF_MASK;
	variant = !variant;
	if (variant)
		uint16_t increment = (FTM0->CONTROLS[FTM_CH_0].CnV & FTM_CnV_VAL_MASK)
				+ OC1;
	else
		uint16_t increment = (FTM0->CONTROLS[FTM_CH_0].CnV & FTM_CnV_VAL_MASK)
				+ OC2;
	FTM0->CONTROLS[FTM_CH_0].CnV = FTM_CnV_VAL(increment);
}

void IC_ISR(void) //FTM3 CH5 PTC9 as IC
{
	static uint16_t med1, med2, med;
	static uint8_t state = 0;

	FTM_ClearInterruptFlag(FTM3, FTM_CH_5);

	if (state == 0) {
		med1 = FTM_GetCounter(FTM3, FTM_CH_5); //
		state = 1;
	} else if (state == 1) {
		med2 = FTM_GetCounter(FTM3, FTM_CH_5);
		med = med2 - med1;
		state = 0;				// Set break point here and watch "med" value
	}
}

void FTMInit(FTM_modes_t mode) {
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;//Le doy clock-gating al flex timer que voy a usar
	NVIC_EnableIRQ(FTM0_IRQn); 			//Lo agrego al vector de interrupciones
	FTM0->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
	switch (mode) {
	case INPUT_CAPTURE:
		ICInit();
		break;
	case OUTPUT_COMPARE:
		OCInit();
		break;
	case PWM:
		break;
	default:
		break;
	}
}

void OCInit(void) {

	PCRstr UserPCR;
	UserPCR.PCR = false; // seteo todo en falso por default
	UserPCR.FIELD.DSE = true; //Drive Streght Enable
	UserPCR.FIELD.MUX = PORT_mAlt4; // pongo la alterniativa de mux de mi flex timer
	UserPCR.FIELD.IRQC = PORT_eDisabled; //desactivo las interrupciones

	PORT_Configure(PORTNUM2PIN(PORTC, 1), UserPCR);

	//Configuro el pin
	FTM0->MODE = (FTM0->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);
	//Activo las funcionalidades avanzadas.
	//BUSCLOCK Sysclk / 2 = 50MHz ->
	//Prescaler = divxXX => timer clock =XX x (1/BusClock) = XX / 50MHz =
	//vamos a hacer un ejemplo de período 1ms y DC 25%
	FTM0->SC = (FTM0->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(FTM_PSC_x2);//aca el preescaler está en 2
	//Preescaler en 2 -> cada tick es 0.04us
	//Status And Control == SC
	FTM0->CNTIN = CNTIN_INIT;
	FTM0->CNT = CNTIN_INIT;
	FTM0->MOD = MOD_INIT - 1; //PERÍODO MENOS UNO
	//Ahora vamos a configurar el MSA y MSB
	FTM0->CONTROLS[0].CnSC = (ftm->CONTROLS[0].CnSC
			& ~(FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK))
			| (FTM_CnSC_MSB((FTM_mOutputCompare >> 1) & 0X01)
					| FTM_CnSC_MSA((FTM_mOutputCompare >> 0) & 0X01));
	//Ahora tenemos que setear el ELSA y ELSB
	FTM0->CONTROLS[0].CnSC = (FTM0->CONTROLS[0].CnSC
			& ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK))
			| (FTM_CnSC_ELSB((FTM_eToggle >> 1) & 0X01)
					| FTM_CnSC_ELSA((FTM_eToggle >> 0) & 0X01));
	//CHIE: Channel interrupt enable -> Cuando CnV = free running counter se levanta el flag de CH-> CHIE te permite que CH interrumpa-

	//Aca lo que tenes que hacer es jugar con los valores de OC1 y OC2 tal que OC1+OC2 sea el período sobre la base de tiempo
	//	OC1 sea periodo por DC sobre la base de tiempo y oc2 1-DC por el periodo sobre la base de timepo
	OC1 = 6250;	// 25000 * 0.25 = 0.25ms
	OC2 = 18750	// 25000*0.75  = 0.75ms
				// período es 25000 ticks. 25000 ticks * 0.04us = 1ms
			FTM0->CONTROLS[0].CnSC = (FTM0->CONTROLS[0].CnSC
					& ~FTM_CnSC_CHIE_MASK) | FTM_CnSC_CHIE(1);
	FTM0->SC |= FTM_SC_CLKS(0x01);
}

void ICInit(void) {
	// PTC1 as OC (FTM0-CH0)
	PCRstr UserPCR;

	UserPCR.PCR = false;			// Default All false, Set only those needed

	UserPCR.FIELD.DSE = true;
	UserPCR.FIELD.MUX = PORT_mAlt4;
	UserPCR.FIELD.IRQC = PORT_eDisabled;

	PORT_Configure2(PORTC, 1, UserPCR);

	// PTC8 as GPIO
	UserPCR.PCR = false;			// Default All false, Set only those needed

	UserPCR.FIELD.DSE = true;
	UserPCR.FIELD.MUX = PORT_mGPIO;
	UserPCR.FIELD.IRQC = PORT_eDisabled;

	PORT_Configure2(PORTC, 8, UserPCR);

	GPIO_SetDirection(PTC, 8, GPIO__OUT);

	// PTC9 as IC (FTM3-CH5)
	UserPCR.PCR = false;			// Default All false, Set only those needed

	UserPCR.FIELD.DSE = true;
	UserPCR.FIELD.MUX = PORT_mAlt3;
	UserPCR.FIELD.IRQC = PORT_eDisabled;

	PORT_Configure2(PORTC, 9, UserPCR);

	//  Enable Timer advanced modes (FTMEN=1)

	FTM0->MODE = (FTM0->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);

	/// BusClock=sysclk/2= 50MHz
	/// Set prescaler = divx32 => Timer Clock = 32 x (1/BusClock)= 32x1/50MHz= 0.64 useg
	/// =>Timer Clock x OC_Delta= 0.64 x 100=64 useg

	/// ---- Generador

	FTM_SetPrescaler(FTM0, FTM_PSC_x32);               // Set Prescaler = divx32
	FTM0->CNTIN = 0x0000;				  		                 // Free running
	FTM0->MOD = 0xFFFF;
	FTM_SetWorkingMode(FTM0, FTM_CH_0, FTM_mOutputCompare);    // OC Function
	FTM_SetOutputCompareEffect(FTM0, FTM_CH_0, FTM_eToggle); // Toggle Output Pin
	FTM_SetInterruptMode(FTM0, FTM_CH_0, true); 		    // enable interrupts
	FTM_StartClock(FTM0);                                    // Select BusClk

	//--- medidor

	FTM_SetPrescaler(FTM3, FTM_PSC_x32);	 		// Set Prescaler = divx32
	FTM3->CNTIN = 0x0000;				  		  				// Free running
	FTM3->MOD = 0xFFFF;
	FTM_SetWorkingMode(FTM3, FTM_CH_5, FTM_mInputCapture); // Select IC Function
	FTM_SetInputCaptureEdge(FTM3, FTM_CH_5, FTM_eEither); // Capture on both edges
	FTM_SetInterruptMode(FTM3, FTM_CH_5, true);            // Enable interrupts
	FTM_StartClock(FTM3);
}
