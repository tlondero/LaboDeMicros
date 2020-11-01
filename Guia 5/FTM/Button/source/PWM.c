/*
 * pwm.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */

#include "header/PWM.h"
#include "header/FTM.h"

#define BAD_PERIOD 0x10
#define MAX_PWMS 16
#define PSC2DIV(psc) (1 << psc)

/*******************************************************************************
 * GLOBAL VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
typedef struct {
	double period;
	double dt;
	uint16_t cnv;
	int8_t pwm_id;
	int8_t ftm_id;
	uint8_t port;
	uint8_t num;
	uint8_t queried;
	uint8_t logic;
	uint8_t PSC;
} pwm_t;
static uint8_t INITIALIZED_PWMS[MAX_PWMS];
static pwm_t PWMS[MAX_PWMS];

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/
int8_t pwm_get_id(void);
uint16_t round(double num);
uint8_t getPSC(double period);
/*******************************************************************************
 * FUNCTION DEFINITIONS WITH GLOBAL SCOPE
 ******************************************************************************/

//MAX FREQ: 194552.53Hz -> 194.5KHz
//MIN FREQ: 5.96Hz
//WARNING: FREQUENCIES CLOSE TO MAX MAY DELIVER SLIGHT ERROR ON DUTY CYCLE
int8_t PWMInitSignal(uint8_t port, uint8_t num, double freq, double dt, uint8_t initial_state) {
	int8_t id = pwm_get_id();
	double period = 1.0 / freq;
	double ftm_clk = (double) FTM_CLK;
	FTM_DATA data;
	if (id != PWM_NO_SPACE) {
		data.PSC = getPSC(period);
		if (data.PSC == BAD_PERIOD) {
			INITIALIZED_PWMS[id] = 0;
			id = -1;
		} else {
			INITIALIZED_PWMS[id] = 1;
			PWMS[id].port = port;
			PWMS[id].num = num;
			PWMS[id].pwm_id = id;
			PWMS[id].logic = initial_state;
			PWMS[id].queried = 1;
			PWMS[id].PSC = data.PSC;
			PWMS[id].period = period;
			PWMS[id].dt = dt;
			data.CNT = 0;
			data.useCallback = 0;
			data.MODE = FTM_mPulseWidthModulation;
			/*
			 * Period = MOD - CNTIN + 1
			 * CNTIN = 0 -> Period = MOD + 1
			 */
			data.MODULO = (uint16_t) ((PWMS[id].period / PSC2DIV(data.PSC))
					* ftm_clk - 1);
			data.CNTIN = 0;
			if (initial_state) {
				data.EPWM_LOGIC = FTM_lAssertedHigh;
				/*
				 * DT = CNV - CNTIN
				 * CNTIN = 0 -> DT = CNV
				 */
				data.CNV = (uint16_t) (round(
						(PWMS[id].period / PSC2DIV(data.PSC)) * dt * ftm_clk));
			} else {
				data.EPWM_LOGIC = FTM_lAssertedLow;
				/*
				 * DT = CNV - CNTIN
				 * CNTIN = 0 -> DT = CNV
				 */
				data.CNV = (uint16_t) (round(
						(PWMS[id].period / PSC2DIV(data.PSC)) * (1.0 - dt)
						* ftm_clk));
			}
			PWMS[id].cnv = data.CNV;
			PWMS[id].ftm_id = FTMInit(port, num, data);
			if (PWMS[id].ftm_id == -1) {
				INITIALIZED_PWMS[id] = 0;
				id = PWM_NO_SPACE;
			}
		}
	}
	return id;
}

void PWMDestroySignal(uint8_t pwm_id, uint8_t final_state) {
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if (INITIALIZED_PWMS[pwm_id] == 1) {
			if (PWMS[pwm_id].queried) {
				PWMStopSignal(pwm_id, final_state);
			}
			INITIALIZED_PWMS[pwm_id] = 0;
		}
	}
}

void PWMStartSignal(int8_t pwm_id) {
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if (INITIALIZED_PWMS[pwm_id] == 1) {
			PWMS[pwm_id].queried = 1;
			FTMSetCnV(PWMS[pwm_id].ftm_id, PWMS[pwm_id].cnv);
			FTMSetSWSYNC(PWMS[pwm_id].ftm_id);
		}
	}
}

void PWMStopSignal(int8_t pwm_id, uint8_t final_state) {
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if (INITIALIZED_PWMS[pwm_id] == 1) {
			PWMS[pwm_id].queried = 0;
			if (PWMS[pwm_id].logic) {
				if (final_state) {
					FTMSetCnV(PWMS[pwm_id].ftm_id, 0xFFFF);;
				} else {
					FTMSetCnV(PWMS[pwm_id].ftm_id, 0x0000);
				}
			} else {
				if (final_state) {
					FTMSetCnV(PWMS[pwm_id].ftm_id, 0x0000);
				} else {
					FTMSetCnV(PWMS[pwm_id].ftm_id, 0xFFFF);
				}
			}
			FTMSetSWSYNC(PWMS[pwm_id].ftm_id);
		}
	}
}

//WARNING: HIGH FREQUENCIES MAY DELIVER HIGH ERROR ON DUTY CYCLE
uint8_t PWMSetFrequency(uint8_t pwm_id, double new_freq) {
	uint8_t ok = 0;
	double ftm_clk = (double) FTM_CLK;
	double new_period = 1.0 / new_freq;
	uint16_t new_PSC = getPSC(new_period);
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if ((INITIALIZED_PWMS[pwm_id] == 1)) {
			if (new_PSC != BAD_PERIOD) {
				PWMS[pwm_id].PSC = new_PSC;
				PWMS[pwm_id].period = new_period;
				if (PWMS[pwm_id].logic) {
					PWMS[pwm_id].cnv = (uint16_t) (round(
							(PWMS[pwm_id].period / PSC2DIV(PWMS[pwm_id].PSC))
									* PWMS[pwm_id].dt * ftm_clk));
				} else {
					PWMS[pwm_id].cnv = (uint16_t) (round(
							(PWMS[pwm_id].period / PSC2DIV(PWMS[pwm_id].PSC))
									* (1.0 - PWMS[pwm_id].dt) * ftm_clk));
				}
				FTMSetCnV(PWMS[pwm_id].ftm_id, PWMS[pwm_id].cnv);
				FTMSetMOD(PWMS[pwm_id].ftm_id,
						(uint16_t) ((PWMS[pwm_id].period
								/ PSC2DIV(PWMS[pwm_id].PSC)) * ftm_clk - 1));
				FTMSetPSC(PWMS[pwm_id].ftm_id, new_PSC);
				FTMSetSWSYNC(PWMS[pwm_id].ftm_id);
				ok = 1;
			}
		}
	}
	return ok;
}

uint8_t PWMSetDT(uint8_t pwm_id, double new_dt) {
	double ftm_clk = (double) FTM_CLK;
	uint8_t ok = 0;
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if ((INITIALIZED_PWMS[pwm_id] == 1)) {
			if (new_dt >= 0 && new_dt <= 1) {
				if (PWMS[pwm_id].logic) {
					PWMS[pwm_id].cnv = (uint16_t) (round(
							(PWMS[pwm_id].period / PSC2DIV(PWMS[pwm_id].PSC))
									* new_dt * ftm_clk));
				} else {
					PWMS[pwm_id].cnv = (uint16_t) (round(
							(PWMS[pwm_id].period * (1.0 - new_dt)
									/ PSC2DIV(PWMS[pwm_id].PSC)) * ftm_clk));
				}
				FTMSetCnV(PWMS[pwm_id].ftm_id, PWMS[pwm_id].cnv);
				FTMSetSWSYNC(PWMS[pwm_id].ftm_id);
				ok = 1;
			}
		}
	}
	return ok;
}

uint8_t PWMIncrementDT(uint8_t pwm_id, double deltaDT) {
	double ftm_clk = (double) FTM_CLK;
	uint8_t ok = 0;
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if ((INITIALIZED_PWMS[pwm_id] == 1)) {
			if (PWMS[pwm_id].dt + deltaDT <= 1) {
				PWMS[pwm_id].dt += deltaDT;
				if (PWMS[pwm_id].logic) {
					PWMS[pwm_id].cnv = (uint16_t) (round(
							(PWMS[pwm_id].period / PSC2DIV(PWMS[pwm_id].PSC))
									* PWMS[pwm_id].dt * ftm_clk));

				} else {
					PWMS[pwm_id].cnv = (uint16_t) (round(
							(PWMS[pwm_id].period / PSC2DIV(PWMS[pwm_id].PSC))
									* (1.0 - PWMS[pwm_id].dt) * ftm_clk));
				}
			} else {
				PWMS[pwm_id].dt = 1.0;
				if (PWMS[pwm_id].logic) {
					PWMS[pwm_id].cnv = 0xFFFF;
				} else {
					PWMS[pwm_id].cnv = 0x0000;
				}
			}
			FTMSetCnV(PWMS[pwm_id].ftm_id, PWMS[pwm_id].cnv);
			FTMSetSWSYNC(PWMS[pwm_id].ftm_id);
			ok = 1;
		}
	}
	return ok;
}

uint8_t PWMDecrementDT(uint8_t pwm_id, double deltaDT) {
	double ftm_clk = (double) FTM_CLK;
	uint8_t ok = 0;
	if (pwm_id >= 0 && pwm_id < MAX_PWMS) {
		if ((INITIALIZED_PWMS[pwm_id] == 1)) {
			if (PWMS[pwm_id].dt - deltaDT >= 0) {
				PWMS[pwm_id].dt -= deltaDT;
				if (PWMS[pwm_id].logic) {
					PWMS[pwm_id].cnv = (uint16_t) (round(
							(PWMS[pwm_id].period / PSC2DIV(PWMS[pwm_id].PSC))
									* PWMS[pwm_id].dt * ftm_clk));
				} else {
					PWMS[pwm_id].cnv = (uint16_t) (round(
							(PWMS[pwm_id].period / PSC2DIV(PWMS[pwm_id].PSC))
									* (1.0 - PWMS[pwm_id].dt) * ftm_clk));
				}
			} else {
				PWMS[pwm_id].dt = 0.0;
				if (PWMS[pwm_id].logic) {
					PWMS[pwm_id].cnv = 0x0000;
				} else {
					PWMS[pwm_id].cnv = 0xFFFF;
				}
			}
			FTMSetCnV(PWMS[pwm_id].ftm_id, PWMS[pwm_id].cnv);
			FTMSetSWSYNC(PWMS[pwm_id].ftm_id);
			ok = 1;
		}
	}
	return ok;
}

uint8_t getPSC(double period) {
	uint8_t PSC;
	/*
	 * PS=x1
	 * Longest period -> MOD = 0xFFFF + 1 -> 1.31072ms
	 * Shortest period -> MOD = 0x0100 + 1 -> 0.00514ms
	 */
	if ((period > 0.00514e-3) && (period < 1.3107e-3)) {
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
	} else {
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
	uint16_t aux = (uint16_t) (num * 10);
	if (aux % 10 <= 5) {
		return (uint16_t) (num);
	} else {
		return ((uint16_t) (num + 1));
	}

}
