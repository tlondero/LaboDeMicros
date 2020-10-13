/*
 * FTM.h
 *
 *  Created on: 10 oct. 2020
 *      Author: Usuario
 */

#ifndef FTM_H_
#define FTM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum channels {FTM_CH_0, FTM_CH_1, FTM_CH_2, FTM_CH_3, FTM_CH_4, FTM_CH_5, FTM_CH_6, FTM_CH_7, CANT_CHANNELS};

typedef enum
{
	FTM_mInputCapture,
	FTM_mOutputCompare,
	FTM_mPulseWidthModulation,
} FTMMode_t;

typedef enum
{
	FTM_eRising 		= 0x01,
	FTM_eFalling 		= 0x02,
	FTM_eEither 		= 0x03,
} FTMEdge_t;

typedef enum
{
	FTM_eToggle 		= 0x01,
	FTM_eClear 			= 0x02,
	FTM_eSet 			= 0x03,
} FTMEffect_t;

typedef enum
{
	FTM_lAssertedHigh	= 0x02,
	FTM_lAssertedLow 	= 0x03,
} FTMLogic_t;

typedef enum
{
	FTM_PSC_x1		= 0x00,
	FTM_PSC_x2		= 0x01,
	FTM_PSC_x4		= 0x02,
	FTM_PSC_x8		= 0x03,
	FTM_PSC_x16		= 0x04,
	FTM_PSC_x32		= 0x05,
	FTM_PSC_x64		= 0x06,
	FTM_PSC_x128	= 0x07,

} FTM_Prescal_t;

typedef struct{
	FTMMode_t MODE;

	FTM_Prescal_t PSC;
	FTMEdge_t IC_EDGE;
	FTMEffect_t OC_EFFECT;
	FTMLogic_t EPWM_LOGIC;

	uint16_t MODULO;
	uint16_t CNT;
	uint16_t CNTIN;
	uint16_t CNV;

	void (*CALLBACK)(void);

}FTM_DATA;
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
//Initializes a new FTM channel
//ftmdata is the data for the initialization of each ftm mode
uint8_t FTMInit(uint8_t pin, FTM_DATA data);

//Stops a FTM channel
void FTMStopClock(uint8_t module);

//Starts the FTM clock
void FTMStartClock(uint8_t module);


#endif /* FTM_H_ */
