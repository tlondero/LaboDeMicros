/*
 * i2c.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */

#include "header/random.h"
#include "header/gpio.h"


//#include <stddef.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * GLOBAL VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/

static bool isInit = false;
static RNG_Type *randBase;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION DEFINITIONS WITH LOCAL SCOPE
 ******************************************************************************/

bool RandInit(void) {

	bool valid = false;

	if (isInit == false) {

		isInit = true;
		valid = true;

		SIM_Type *sim = SIM;
		sim->SCGC3 |= SIM_SCGC3_RNGA_MASK;
		sim->SCGC3 &= ~SIM_SCGC3_RNGA_MASK;
		sim->SCGC3 |= SIM_SCGC3_RNGA_MASK;

		randBase = RNG;

		//Config
		NVIC_EnableIRQ(RNG_IRQn);

		randBase->CR = 0;

		randBase->CR |= RNG_CR_GO_MASK;

		randBase->CR |= RNG_CR_INTM_MASK;
		randBase->CR |= RNG_CR_HA_MASK;

		randBase->CR &= ~RNG_CR_SLP_MASK;			//Normal mode
	}

	return valid;
}

bool RandCheck(void){
	return !(randBase->SR & RNG_SR_OREG_LVL_MASK);
}

uint32_t RandGet(void){
	return randBase->OR;
}

/*
bool RandInit() {
    RNG->CR |= RNG_CR_SLP_MASK;
    RNG->CR |= RNG_CR_GO_MASK;
    RNG->CR |= RNG_CR_HA_MASK;
    RNG->CR |= RNG_CR_INTM_MASK;
    return true;
}

uint32_t RandGet() {
    while ((RNG->SR & RNG_SR_OREG_LVL_MASK) == 0);
    return RNG->OR;
}*/
