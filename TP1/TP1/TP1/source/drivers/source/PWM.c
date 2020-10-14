/*
 * pwm.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */

#include <drivers/headers/PWM.h>
#include "drivers/headers/FTM.h"

#define BAD_PERIOD 0x10

/*******************************************************************************
 * GLOBAL VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
typedef struct {
	uint32_t period;
	uint32_t dt;
	uint16_t cnv;
	uint8_t pwm_id;
	uint8_t ftm_id;
	uint8_t pin;
	uint8_t queried;
	uint8_t logic;
} pwm_t;
static uint8_t INITIALIZED_PWMS[MAX_PWMS];
static pwm_t PWMS[MAX_PWMS];
static uint8_t init;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/
int8_t pwm_get_id(void);
/*******************************************************************************
 * FUNCTION DEFINITIONS WITH GLOBAL SCOPE
 ******************************************************************************/

int8_t PWMInitSignal(pin_t pin, double freq, double dt, uint8_t initial_state) {
	int8_t id = pwm_get_id();
	double period = 1.0 / freq;
	double ftm_clk = (double) FTM_CLK;
	if (id != UNAVAILABLE_SPACE) {
		PWMS[id].pin = pin;
		PWMS[id].pwm_id = id;
		PWMS[id].logic = initial_state;
		PWMS[id].queried = 1;
		FTM_DATA data;
		data.CNT = 0;
		data.useCallback = 0;
		data.MODE = FTM_mPulseWidthModulation;
		data.PSC = getPSC(period);
		if(PSC == BAD_PERIOD){
			INITIALIZED_PWMS[id] = 0;
			id = -1;
		}

		/*
		 * Period = MOD - CNTIN + 1
		 * CNTIN = 0 -> Period = MOD + 1
		 */
		data.MODULO = (uint16_t) (period * ftm_clk - 1);
		data.CNTIN = 0;

		if (initial_state) {
			data.OC_EFFECT = FTM_lAssertedHigh;
			/*
			 * DT = CNV - CNTIN
			 * CNTIN = 0 -> DT = CNV
			 */
			data.CNV = (uint16_t) (round(period * dt) * ftm_clk);
		} else {
			data.OC_EFFECT = FTM_lAssertedLow;
			/*
			 * DT = CNV - CNTIN
			 * CNTIN = 0 -> DT = CNV
			 */
			data.CNV = (uint16_t) (round(period * (1 - dt)) * ftm_clk);
		}
		PWMS[pwm_id].cnv = data.CNV;
		PWMS[pwm_id].ftm_id = FTMInit(pin, data);
		if (PWMS[pwm_id].ftm_id == -1) {
			INITIALIZED_PWMS[id] = 0;
			id = -1;
		}
	}
	return id;
}

void PWMDestroySignal(uint8_t pwm_id, uint8_t final_state) {
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if (PWMS[pwm_id].queried) {
			PWMStopSignal(pwm_id, final_state);
		}
		INITIALIZED_PWMS[pwm_id] = 0;
	}
}

void PWMStartSignal(int8_t pwm_id) {
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if (dt >= 0 && dt <= 100) {
			if (INITIALIZED_PWMS[pwm_id] == 1) {
				PWMS[id].queried = 1;
				FTMSetCnV(ftm_id, PWMS[id].cnv);
			}
		}
	}
}

void PWMStopSignal(int8_t pwm_id, uint8_t final_state) {
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if (INITIALIZED_PWMS[pwm_id] == 1) {
			PWMS[id].queried = 0;
			if (PWMS[id].logic) {
				if (final_state) {
					FTMSetCnV(PWMS[id].ftm_id, 0xFFFF);
				} else {
					FTMSetCnV(PWMS[id].ftm_id, 0x0000);
				}
			} else {
				if (final_state) {
					FTMSetCnV(PWMS[id].ftm_id, 0x0000);
				} else {
					FTMSetCnV(PWMS[id].ftm_id, 0xFFFF);
				}
			}
		}
	}
}

uint8_t PWMSetFrequency(uint8_t pwm_id, double new_freq){
	double new_period = 1.0/new_freq;
	uint16_t new_PSC = getPSC(new_period);
	if(new_PSC != BAD_PERIOD){
		FTMSetPSC(PWMS[id].ftm_id, PSC);
		return 1;
	}
	else{
		return 0;
	}
}

uint8_t PWMSetDT(uint8_t pwm_id, double new_dt) {
	if (new_dt >= 0 && new_dt <= 1) {
		if (PWMS[pwm_id].logic) {
			FTMSetCnV(PWMS[id].ftm_id,
					(uint16_t) (round(period * dt) * ftm_clk));
		} else {
			FTMSetCnV(PWMS[id].ftm_id,
					(uint16_t) (round(period * (1 - dt)) * ftm_clk));
		}
		PWMS[pwm_id].cnv = data.CNV;
		return 1;
	}
	else{
		return 0;
	}
}

uint8_t getPSC(double period){
	uint8_t PSC;
	/*
	 * PS=x1
	 * Longest period -> MOD = 0xFFFF + 1 -> 1.31072ms
	 * Shortest period -> MOD = 0x1000 + 1 -> 0.08194ms
	 */
	if ((period > 0.08192e-3) && (period < 1.3107e-3)) {
		PSC = FTM_PSC_x1;
	}
	/*
	 * PS=x2
	 * Longest period -> MOD = 0xFFFF + 1 -> 2.62144ms
	 * Shortest period -> MOD = 0xFFFF/4 + 1 -> 0.65539ms
	 */
	else if ((period > 0.65535e-3) && (period < 2.6214e-3)) {
		PSC = FTM_PSC_x2;
	}
	/*
	 * PS=x4
	 * Longest period -> MOD = 0xFFFF + 1 -> 5.24288ms
	 * Shortest period -> MOD = 0xFFFF/4 + 1 -> 1.31078ms
	 */
	else if ((period > 5.2428e-3) && (period < 1.3107e-3)) {
		PSC = FTM_PSC_x4;
	}
	/*
	 * PS=x8
	 * Longest period -> MOD = 0xFFFF + 1 -> 10.48576ms
	 * Shortest period -> MOD = 0xFFFF/4 + 1 -> 2.62156ms
	 */
	else if ((period > 2.6214e-3) && (period < 10.4856e-3)) {
		PSC = FTM_PSC_x8;
	}
	/*
	 * PS=x16
	 * Longest period -> MOD = 0xFFFF + 1 -> 20.97152ms
	 * Shortest period -> MOD = 0xFFFF/4 + 1 -> 5.24312ms
	 */
	else if ((period > 5.2428e-3) && (period < 20.9712e-3)) {
		PSC = FTM_PSC_x16;
	}
	/*
	 * PS=x32
	 * Longest period -> MOD = 0xFFFF + 1 -> 41.94304s
	 * Shortest period -> MOD = 0xFFFF/4 + 1 -> 10.48624ms
	 */
	else if ((period > 10.4856e-3) && (period < 41.9424e-3)) {
		PSC = FTM_PSC_x32;
	}
	/*
	 * PS=x64
	 * Longest period -> MOD = 0xFFFF + 1 -> 83.88608ms
	 * Shortest period -> MOD = 0xFFFF/4 + 1 -> 20.97248ms
	 */
	else if ((period > 20.9712e-3) && (period < 83.8848e-3)) {
		PSC = FTM_PSC_x64;
	}
	/*
	 * PS=x128
	 * Longest period -> MOD = 0xFFFF + 1 -> 167.77216ms
	 * Shortest period -> MOD = 0xFFFF/4 + 1 -> 41.94496ms
	 */
	else if ((period > 41.9424e-3) && (period < 167.7696e-3)) {
		PSC = FTM_PSC_x128;
	}
	else{
		PSC = BAD_PERIOD;
	}
	return PSC;
}

int8_t pwm_get_id(void) {
	uint8_t i;
	uint8_t found_space = 0;
	int8_t id = -1;
	for (i = 0; i < MAX_PWMS; i++) {
		if (!(INITIALIZED_PWMS[i])) { //me fijo si hay espacio
			INITIALIZED_PWMS[i] = 1;
			found_space = 1;
			break;
		}
	}
	if (found_space) {
		id = i;
	} else {
		id = -1;
	}
	return id;
}

uint16_t round(double num) {
	aux = (uint16_t) (num * 10);
	if (aux % 10 <= 5) {
		return (uint16_t) (num);
	} else {
		return ((uint16_t) (num + 1));
	}

}
/*	//TODO: Si no anda la de arriba probar esta
 uint16_t redondea(float num){
 uint16_t a=((uint16_t) num) * 10;
 uint16_t b =  ((uint16_t) (num * 10));
 printf("%d vs %d , la diferencia es %d\n",a,b,a-b);
 if(((uint16_t) (num * 10))- ((((uint16_t) num) * 10) ) >= 5)
 return (uint16_t) num +1;
 else
 return (uint16_t) num;
 }
 */
