
/***************************************************************************//**
 @file     AccelerometerEvents.c
 @brief    Application functions
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "header/AccelerometerEvents.h"
#include "header/FXOS8700CQ.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DECLARATION
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 *******************************************************************************/
movement accelerometerGetEvent(void){
	static bool go_back_to_center=false;
	uint8_t ret=NO_MOVE_m;
	SRAWDATA_f ac;

	if(FXOS8700CQ_getDataFlag()){
		ac = FXOS8700CQ_getAcc();
		if(((fabs(ac.x)> THRESHOLD) && !go_back_to_center )){
			if(((ac.x)< 0) ){
				ret=LEFT_m;
			}
			else{
				ret=RIGHT_m;
			}
			go_back_to_center=true;
		}
		else if(((fabs(ac.y)> THRESHOLD) && !go_back_to_center )){
			if(((ac.x)< 0) ){
				ret=ROTATE_m;
			}
			else{
				ret=DOWN_m;

			}
			go_back_to_center=true;
		}
		else if((fabs(ac.x) <= THRESHOLD) && (fabs(ac.y) <= THRESHOLD)){
			ret=NO_MOVE_m;
			go_back_to_center=false;
		}
	}
	return ret;
}
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DEFINITION
 *******************************************************************************/

