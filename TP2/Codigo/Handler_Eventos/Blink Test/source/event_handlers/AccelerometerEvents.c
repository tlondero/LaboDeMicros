
/***************************************************************************//**
 @file     AccelerometerEvents.c
 @brief    Application functions
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <header/drivers/FXOS8700CQ.h>
#include <header/event_handlers/AccelerometerEvents.h>
#include <math.h>

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
void accelerometerGetEvent(package * data){
	static bool go_back_to_center=false;
	SRAWDATA_f ac;

	if(FXOS8700CQ_getDataFlag()){
		ac = FXOS8700CQ_getAcc();
		if(((fabs(ac.x)> THRESHOLD) && !go_back_to_center )){
			if(((ac.x)< 0) ){
				data->action.left=true;
			}
			else{
				data->action.right=true;
			}
			go_back_to_center=true;
		}
		else if(((fabs(ac.y)> THRESHOLD) && !go_back_to_center )){
			if(((ac.x)< 0) ){
				data->action.rotate=true;
			}
			else{
				data->action.down=true;

			}
			go_back_to_center=true;
		}
		else if((fabs(ac.x) <= THRESHOLD) && (fabs(ac.y) <= THRESHOLD)){
			go_back_to_center=false;

		}
	}
}
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DEFINITION
 *******************************************************************************/

