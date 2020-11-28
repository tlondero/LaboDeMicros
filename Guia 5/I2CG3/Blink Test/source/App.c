/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "header/Button.h"
#include "header/i2c.h"
#include "header/gpio.h"
#include "MK64F12.h"
#include "header/board.h"
#include "header/FXOS8700CQ.h"
#include "timer.h"
#include <math.h>
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define THRESHOLD (0.5)
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 * INTERRUPCIONES
 ******************************************************************************/

/*******************************************************************************
 * BALIZA
 ******************************************************************************/


void I2CInit_tb(void) {
	FXOS8700CQInit();
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);
	gpioWrite(PIN_LED_GREEN, HIGH);
	gpioWrite(PIN_LED_BLUE, HIGH);
	timerInit();
	tim_id_t tid = timerGetId();
	timerStart(tid, TIMER_MS2TICKS(200),TIM_MODE_PERIODIC,FXOS8700CQ_ReadAccelMagnData);

}


/* Función que se llama constantemente en un ciclo infinito */
SRAWDATA_f ac;
void I2CRun_tb(void) {
	FXOS8700CQ_ReadAccelMagnData();

	if(FXOS8700CQ_getDataFlag()){
		ac = FXOS8700CQ_getAcc();
	}
}
void I2CRunDeltas_tb(void) {
	static bool first=true;
	static bool go_back_to_center=false;
	if(FXOS8700CQ_getDataFlag()  && !first ){
		ac = FXOS8700CQ_getAcc();
		if(((fabs(ac.x)> THRESHOLD) && !go_back_to_center )){// || (fabs(ac.y-old_ac.y)> THRESHOLD) || (fabs(ac.z-old_ac.z)> THRESHOLD)) ){
			if(((ac.x)< 0) ){//|| ((ac.y-old_ac.y)< 0) || ((ac.z-old_ac.z)< 0)){
				gpioWrite(PIN_LED_GREEN, LOW);
			}
			else{
				gpioWrite(PIN_LED_RED, LOW);
			}
			go_back_to_center=true;
		}
		else if(((fabs(ac.y)> THRESHOLD) && !go_back_to_center )){
			if(((ac.x)< 0) ){
				gpioWrite(PIN_LED_GREEN, LOW);
				gpioWrite(PIN_LED_RED, LOW);
			}
			else{
				gpioWrite(PIN_LED_RED, LOW);
				gpioWrite(PIN_LED_BLUE, LOW);

			}
			go_back_to_center=true;

		}
		else if((fabs(ac.x) <= THRESHOLD) && (fabs(ac.y) <= THRESHOLD)){
			gpioWrite(PIN_LED_RED, HIGH);
			gpioWrite(PIN_LED_BLUE, HIGH);
			gpioWrite(PIN_LED_GREEN, HIGH);
			go_back_to_center=false;
		}
	}
	if(first)
		first=false;
}
