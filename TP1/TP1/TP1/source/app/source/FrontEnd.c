/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "../headers/FrontEnd.h"
#include "../headers/users.h"
#include "../../drivers/headers/PV.h"
#include "../../drivers/headers/FRDM.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH FILE SCOPE
 ******************************************************************************/
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE
 ******************************************************************************/
/*******************************************************************************
 * FUNCTION DEFINITION WITH GLOBAL SCOPE
 ******************************************************************************/

void drawFrontEnd(FEData data, state st) {
uint8_t i=0;
	switch (st) {
	case IDDLE:
		if(data.animation_en){
			for(i;i<4;i++)
				PVDisplaySendChar('-', 0);
		}
		else if((!data.good_id) && (!data.bad_id)){
			for(i;i<4;i++)
				PVdispSendChar(data.id_data[data.id_counter-3+i] + '0', i);
			}
		else if(data.bad_id){
			FRDMLedFlash(RED);//aca le digo que titile 3 veces rojo
		}
		else if(data.good_id){
			FRDMLedFlash(GREEN);//aca le digo que titile 3 veces verde
		}
		break;
	case ASK_PIN:
		if((!data.good_pin) && (!data.bad_pin)){
			for(i;i<4;i++)
				PVdispSendChar(data.pin_data[data.pin_counter-3+i] + '0', i);
			}
		else if(data.bad_id){
			FRDMLedFlash(RED);	//aca le digo que titile 3 veces rojo
		}
		else if(data.good_id){
			FRDMLedFlash(GREEN);	//aca le digo que titile 3 veces verde
		}
		uint8_t j=0;
		for(j=0; j < getStrikes(transformToNum(data.id_data, ID_LEN));j++){
			//Turn on led de rojo la placa verde
			////////////////////////////
			//////////MIRAAAAR////////////TODO
			////////////////////////////
		}
		break;
	case ACCESS:
		if(data.animation_en){
			uint8_t i=0;
			switch(data.animation_opt){
			case OPEN_ANIMATION:
				for(i=0;i<4;i++)
					PVdispSendChar(OPEN_MSG[i],i);
				break;
			case USERS_ANIMATION:
				for(i=0;i<4;i++)
					PVdispSendChar(USER_MSG[i],i);
				break;
			case BRIGHTNESS_ANIMATION:
				for(i=0;i<4;i++)
					PVdispSendChar(BRIGHTNESS_MSG[i],i);
				break;
			}
		}
		break;
	case OPEN:
		if(data.open){
			PVAnimation(OPEN_ANIMATION, true);
		}

		break;
	case USERS:
//TODO
		break;
	case BRIGHTNESS:
		uint8_t i=0;
		for(i=0;i<4;i++)
			PVdispSendChar(BRIGHTNESS_MSG[i],i);
		PVDisplaySetBright(data.brightness);
		break;
	default:
		break;
	}
}
/*******************************************************************************
 * FUNCTION DEFINITION WITH FILE SCOPE
 ******************************************************************************/

