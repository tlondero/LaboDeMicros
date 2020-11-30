/*
 * i2c.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */

#include "../header/drivers/random.h"
#include "../header/drivers/gpio.h"



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
	uint32_t first_value;
	if (isInit == false) {

		isInit = true;
		valid = true;

		SIM_Type *sim = SIM;
		sim->SCGC3 |= SIM_SCGC3_RNGA_MASK;
		sim->SCGC6 |= SIM_SCGC6_RNGA_MASK;
		randBase =RNG ;

		randBase->CR |=  RNG_CR_INTM_MASK;

		randBase->CR |= RNG_CR_HA_MASK;
		randBase->CR |= RNG_CR_GO_MASK;
		while( !(randBase->SR & RNG_SR_OREG_LVL_MASK) ){}
		first_value=randBase->OR ;
	}

	return valid;
}


uint32_t RandGet(void){
	while( !(randBase->SR & RNG_SR_OREG_LVL_MASK) ){}
	return randBase->OR ;
}

