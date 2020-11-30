#ifndef SOURCES_TEMPLATE_ADC_H_
#define SOURCES_TEMPLATE_ADC_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {
	ADC_b8, ADC_b12, ADC_b10, ADC_b16,
} ADCBits_t;

typedef enum {
	ADC_c24, ADC_c16, ADC_c10, ADC_c6, ADC_c4,
} ADCCycles_t;

typedef enum {
	ADC_t4, ADC_t8, ADC_t16, ADC_t32, ADC_t1,
} ADCTaps_t;

typedef enum {
	ADC_mA, ADC_mB,
} ADCMux_t;

typedef enum {
	ADC_0, ADC_1,
} ADCAlt_t;

typedef ADC_Type *ADC_t;
typedef uint8_t ADCChannel_t; /* Channel 0-23 */
typedef uint16_t ADCData_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief  Initializes ADC
 * @param adc : pointer to adc module
 * @param resolution: resolution of the adc
 * @param cycles: number of cycles in the SAR
 */
void ADC_Init(ADC_t adc, ADCBits_t reslution, ADCCycles_t cycles);

//Setters and getters
void ADC_SetInterruptMode(ADC_t, bool);
bool ADC_IsInterruptPending(ADC_t);
void ADC_ClearInterruptFlag(ADC_t);
void ADC_SetResolution(ADC_t, ADCBits_t);
ADCBits_t ADC_GetResolution(ADC_t);
void ADC_SetCycles(ADC_t, ADCCycles_t);
ADCCycles_t ADC_GetCycles(ADC_t);
void ADC_SetHardwareAverage(ADC_t, ADCTaps_t);
ADCTaps_t ADC_GetHardwareAverage(ADC_t);

//Calibration of the adc
bool ADC_Calibrate(ADC_t);

/**
 * @brief starts a conversion in the ADC
 * @param adc_t : pointer to adc module
 * @param adcchannel_t: channel used to start conversion
 * @param adcmux_t: mux option for the adc
 */
void ADC_Start(ADC_t, ADCChannel_t, ADCMux_t);

/**
 * @brief checks whether a conversion is done
 * @param adc_t : pointer to adc module
 */
bool ADC_IsReady(ADC_t);

/**
 * @brief get the result of a conversion
 * @param adc_t : pointer to adc module
 */
ADCData_t ADC_getData(ADC_t);
/*******************************************************************************
 ******************************************************************************/
#endif /* SOURCES_TEMPLATE_ADC_H_ */
