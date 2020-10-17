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
	uint8_t i = 0;
	switch (st) {
	case IDDLE:
		if (data.animation_en) {
			for (i = 0; i < 4; i++)
				PVDisplaySendChar('-', 0);
		} else if ((!data.good_id) && (!data.bad_id)) {
			for (i = 0; i < 4; i++) {
				if (data.id_counter > 3) {
					PVDisplaySendChar(
							data.id_data[data.id_counter - 3 + i] + '0', i);
				} else {
					PVDisplaySendChar(data.id_data[i] + '0', i);
				}
			}
		} else if (data.bad_id) {
			FRDMLedFlash(RED); //aca le digo que titile 3 veces rojo
		} else if (data.good_id) {
			FRDMLedFlash(GREEN); //aca le digo que titile 3 veces verde
		}
		break;
	case ASK_PIN:
		if ((!data.good_pin) && (!data.bad_pin)) {
			for (i = 0; i < 4; i++) {
				if (data.pin_counter > 3) {
					PVDisplaySendChar(
							data.pin_data[data.pin_counter - 3 + i] + '0', i);
				} else {
					PVDisplaySendChar(data.pin_data[i] + '0', i);
				}
			}
		} else if (data.bad_pin) {
			FRDMLedFlash(RED);	//aca le digo que titile 3 veces rojo
		} else if (data.good_pin) {
			FRDMLedFlash(GREEN);	//aca le digo que titile 3 veces verde
		}
		for (i = 0; i < getStrikes(transformToNum(data.pin_data, PIN_LEN));
				i++) {
			//Turn on led de rojo la placa verde
			////////////////////////////
			//////////MIRAAAAR////////////TODO
			////////////////////////////
		}
		break;
	case ACCESS:
		if (data.animation_en) {
			switch (data.animation_opt) {
			case OPEN_ANIMATION:
				for (i = 0; i < 4; i++)
					PVDisplaySendChar(OPEN_MSG[i], i);
				break;
			case USERS_ANIMATION:
				for (i = 0; i < 4; i++)
					PVDisplaySendChar(USER_MSG[i], i);
				break;
			case BRIGHTNESS_ANIMATION:
				for (i = 0; i < 4; i++)
					PVDisplaySendChar(BRIGHTNESS_MSG[i], i);
				break;
			}
		}
		break;
	case OPEN:
		if (data.open) {
			PVAnimation(OPEN_ANIMATION, true);
		}

		break;
	case USERS:
//TODO
		break;
	case BRIGHTNESS:
		for (i = 0; i < 4; i++)
			PVDisplaySendChar(BRIGHTNESS_MSG[i], i);
		PVDisplaySetBright(data.brightness);
		break;
	case USERS_CLAVE:
		for (i = 0; i < 4; i++) {
			if (data.pin_counter > 3) {
				PVDisplaySendChar(data.pin_data[data.pin_counter - 3 + i] + '0',
						i);
			} else {
				PVDisplaySendChar(data.pin_data[i] + '0', i);
			}
		}
		break;
	case USERS_ADD:
		if ((data.id_counter <= ID_LEN) && (!data.good_id)) {//Aca entra mientras ic_counter < 7 y no hayas dado al ok
			for (i = 0; i < 4; i++) {
				if (data.id_counter > 3) {
					PVDisplaySendChar(
							data.id_data[data.id_counter - 3 + i] + '0', i);
				} else {
					PVDisplaySendChar(data.id_data[i] + '0', i);
				}
			}
		} else if (data.good_id) {
			FRDMLedFlash(BLUE);
		} else if ((data.pin_counter <= PIN_LEN) && (!data.good_pin)) { // en el caso de que este metiendo el pin
			for (i = 0; i < 4; i++) {
				if (data.pin_counter > 3) {
					PVDisplaySendChar(
							data.pin_data[data.pin_counter - 3 + i] + '0', i);
				} else {
					PVDisplaySendChar(data.pin_data[i] + '0', i);
				}
			}
		}
		else if(data.good_pin){
			FRDMLedFlash(BLUE);
		}
		break;
	case USERS_DEL:
		PVMarquesina(((user_t *) (data.del_user_ptr))->id);
		if(data.del_user){
			FRDMLedFlash(BLUE);
		}
		break;
	default:
		break;
	}
}
/*******************************************************************************
 * FUNCTION DEFINITION WITH FILE SCOPE
 ******************************************************************************/
const char BRIGHTNESS_MSG[4];			// = { 'B','R','G','H' };
const char OPEN_MSG[4];			// = { 'O','P','E','N' };
const char USER_MSG[4];			// = { 'U','S','E','R' };

const char PASSWORD_MSG[4];			// = { 'P','A','S','S' };
const char ADD_USER_MSG[4];			// = { 'A','D','D','U' };
const char DEL_USER_MSG[4];			// = { 'D','E','L','U' };
