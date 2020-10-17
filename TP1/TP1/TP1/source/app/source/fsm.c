/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "../headers/fsm.h"
#include "../../drivers/headers/timer.h"
#include "../../drivers/headers/PV.h"
#include "../../drivers/headers/Card.h"
#include "../../drivers/headers/FRDM.h"
#include "../../app/headers/users.h"
 /*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CLEARED_DIGIT 10
 /*******************************************************************************
  * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
  ******************************************************************************/
enum{OPEN_SEL,BRIGHT_SEL,USER_SEL};
  /*******************************************************************************
   * VARIABLE PROTOTYPES WITH FILE SCOPE
   ******************************************************************************/





//Pin buffer
static uint8_t encoder_pin_digits[PIN_LEN];

//Id Buffer
static uint8_t encoder_id_digits[ID_LEN];
//Frond-end data
static FEData fe_data;
static FEData* const fe_data_ptr = &fe_data;

//Inactivity
static tim_id_t inactivity_timer_id;
static bool inactivity_triggered;

//Open
static tim_id_t open_timer_id;
static bool open_triggered;

//Cancel
static const uint8_t cancel_switch = BUTTON_SW2;
static bool cancel_triggered;

//Back
static const uint8_t back_switch = BUTTON_SW3;
static bool back_triggered;

//Fsm
static state prev_state = IDDLE;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE
 ******************************************************************************/
 /**
  * @brief IDDLERoutine RELLENAR.
  *
  * @return state variable with the updated state
  */
state IDDLERoutine(void);
//transforms de array to a number 
uint32_t transformToNum(uint8_t* data, uint8_t count);
uint32_t pwr(uint32_t num, uint32_t exp);

/**
 * @brief AskPinRoutine RELLENAR.
 *
 * @return state variable with the updated state
 */
state askPinRoutine(void);
/**
 * @brief AccessRoutine RELLENAR.
 *
 * @return state variable with the updated state
 */
state accessRoutine(void);
/**
 * @brief OpenRoutine RELLENAR.
 *
 * @return state variable with the updated state
 */
state openRoutine(void);
/**
 * @brief UsersRoutine RELLENAR.
 *
 * @return state variable with the updated state
 */
state usersRoutine(void);
/**
 * @brief BrightnessRoutine RELLENAR.
 *
 * @return state variable with the updated state
 */
state brightnessRoutine(void);
///////////////////////////
//     Callbacks        //
//////////////////////////
void inactivityCallback(void);
void cancelCallback(void);
void backCallback(void);
void openCallback(void);
/*******************************************************************************
 * FUNCTION DEFINITION WITH GLOBAL SCOPE
 ******************************************************************************/

state FSMInitState(void) {

	//Pido ids de todos los servicios
	inactivity_timer_id = timerGetId();
	open_timer_id = timerGetId();

	//Comienzo timers
	timerStart(inactivity_timer_id, TIMER_MS2TICKS(INACTIVITY_TRIGGER_TIME), TIM_MODE_SINGLESHOT, inactivityCallback);
	timerStop(inactivity_timer_id);
	timerStart(open_timer_id, TIMER_MS2TICKS(OPEN_TRIGGER_TIME), TIM_MODE_SINGLESHOT, openCallback);
	timerStop(open_timer_id);

	//Tengo que activar las interrupciones de cancel y delete
	FRDMButtonIRQ(cancel_switch, GPIO_IRQ_MODE_FALLING_EDGE, cancelCallback);
	FRDMButtonIRQ(back_switch, GPIO_IRQ_MODE_FALLING_EDGE, backCallback);
	FRDMLedSetFlash(3);

	initUsers();
	//Front-end related stuff
	fe_data.animation_en = true;
	fe_data.animation_opt = IDDLE_ANIMATION;
	fe_data.brightness = 100;
	fe_data.id_data = &encoder_id_digits[0];
	fe_data.pin_data = &encoder_pin_digits[0];
	//me suscribo a los eventos de encoder
	PVSuscribeEvent(BTN_PRESS, true);
	PVSuscribeEvent(ENC_LEFT, true);
	PVSuscribeEvent(ENC_RIGHT, true);
	return IDDLE;
}

state FSMRun(state actual_state) {

	//Cada vez que entro de nuevo a FSMRun desactivo
	//los bools que le informaban al front end de las cosas que sucedieron
	fe_data.blocked_user = false;
	fe_data.bad_id = false;
	fe_data.good_id = false;
	fe_data.bad_pin = false;
	fe_data.good_pin = false;
	fe_data.open = false;

	state updated_state;
	switch (actual_state) {
	case IDDLE:
		updated_state = IDDLERoutine();//DONE
		break;
	case ASK_PIN:
		updated_state = askPinRoutine();//DONE
		break;
	case ACCESS:
		updated_state = accessRoutine();//TODO
		break;
	case OPEN:
		updated_state = openRoutine();//DONE
		break;
	case USERS:
		updated_state = usersRoutine();//TODO
		break;
	case BRIGHTNESS:
		updated_state = brightnessRoutine();//DONE
		break;
	default:
		break;
	}
	prev_state = actual_state;
	return updated_state;
}

FEData  * const FSMGetFEData(void) {
	return fe_data_ptr;
}

/*******************************************************************************
 * FUNCTION DEFINITION WITH FILE SCOPE
 ******************************************************************************/
state IDDLERoutine(void) {
	static bool using_encoder = false;
	static uint8_t actual_encoder_number = 0;
	uint8_t* card_event = 0;

	state updated_state = IDDLE;

	/*
	* STATE INIT
	*/
	if (prev_state != IDDLE) {
		fe_data.animation_en = true;
		fe_data.animation_opt = IDDLE_ANIMATION;
		fe_data.id_counter = 0;
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		FRDMButtonIRQ(cancel_switch, GPIO_IRQ_MODE_FALLING_EDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, GPIO_IRQ_MODE_FALLING_EDGE, backCallback);
		//activar las interrupciones de cancel y del
	}

	/*
	* STATE RUN
	*/
	if (!using_encoder) {
		card_event = cardGetPAN();
		if (card_event != NULL) {
			if (checkExistance(transformToNum(card_event, ID_LEN))) {
				if (getBlockedStatus(transformToNum(card_event, ID_LEN))) {
					fe_data.blocked_user = true;
					updated_state = IDDLE;
				}
				else {
					updated_state = ASK_PIN;
					fe_data.good_id = true;
					fe_data.animation_en = false;
				}
			}
			else {
				card_event = NULL;
				fe_data.bad_id = true;
			}
		}
	}

	if (PVCheckEvent()) {
		fe_data.animation_en = false;
		using_encoder = true;
	}
	if (using_encoder && updated_state == IDDLE) {
		if (cancel_triggered) {
			fe_data.animation_en = true;
			fe_data.id_counter = 0;
			using_encoder = false;
			cancel_triggered = false;
		}
		else if (back_triggered) {
			if (fe_data.id_counter > 0) {
				//encoder_id_digits[fe_data.id_counter] = CLEARED_DIGIT;
				//if (fe_data.id_counter < 4)
				//	PVdispSendChar(' ', fe_data.id_counter);
				//else {
				//	uint8_t i;
				//	for (i = 0; i < 4; i++) {
				//		PVdispSendChar(encoder_id_digits[fe_data.id_counter - 1 - i] + '0', 3 - i);
				//	}
				//}
				//////////////////////////////////////////////////
				// ACA HAY QUE VER PARA QUE EN EL DISPLAY NO QUEDE CAGADO //creo que lo hice bien pero estar�a bueno que me lo chequeens
				//////////////////////////////////////////////////////////
				fe_data.id_counter--;
			}
			back_triggered = false;
		}
		//ahora hay que fijarse si llego un evento de encoder, si es derecha aumento el numero si es izquierda lo achico si es enter avanzo
		if (PVCheckEvent()) {
			event_t ev = PVGetEv();
			switch (ev) {
			case ENC_LEFT:
				if (actual_encoder_number == 0)
					actual_encoder_number = 9;
				else
					actual_encoder_number--;

				encoder_id_digits[fe_data.id_counter] = actual_encoder_number;
//				if (fe_data.id_counter < 4) {
//
//					PVdispSendChar(actual_encoder_number + '0', fe_data.id_counter);//Aca habr�a que ver bien el orden de los displays
//				}
//				else {
//					PVdispSendChar(actual_encoder_number + '0', 3);//Aca habr�a que ver bien el orden de los displays
//				}
				break;
			case ENC_RIGHT:
				if (actual_encoder_number == 9)
					actual_encoder_number = 0;

				else
					actual_encoder_number++;

				encoder_id_digits[fe_data.id_counter] = actual_encoder_number;
				//if (fe_data.id_counter < 4) {
				//	PVdispSendChar(actual_encoder_number + '0', fe_data.id_counter);//Aca habr�a que ver bien el orden de los displays
				//}
				//else {
				//	PVdispSendChar(actual_encoder_number + '0', 3);//Aca habr�a que ver bien el orden de los displays
				//}
				break;
			case BTN_PRESS:
				if (fe_data.id_counter < ID_LEN-1) {
					fe_data.id_counter++;
				}
				else {
					if (checkExistance(transformToNum(&encoder_id_digits[0], ID_LEN))) {
						if (getBlockedStatus(transformToNum(&encoder_id_digits[0], ID_LEN))) {
							//fe_data.blocked_user = true;
							fe_data.animation_en = true;
							fe_data.animation_opt = IDDLE_ANIMATION;
							fe_data.id_counter = 0;
							updated_state = IDDLE;
						}
						else {
							updated_state = ASK_PIN;
							fe_data.good_id = true;
							fe_data.animation_en = false;
						}
					}
					else {
						fe_data.bad_id = true;
						fe_data.id_counter = 0;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	return updated_state;
}
state askPinRoutine(void) {
	state updated_state = ASK_PIN;
	static uint8_t actual_encoder_number = 0;

	if (prev_state != ASK_PIN) {
		fe_data.pin_counter = 0;
		fe_data.animation_en = false;
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		FRDMButtonIRQ(cancel_switch, GPIO_IRQ_MODE_FALLING_EDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, GPIO_IRQ_MODE_FALLING_EDGE, backCallback);
		//activar las interrupciones de cancel y del
	}

	if (cancel_triggered) {
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		fe_data.pin_counter = 0;
		cancel_triggered = false;
	}
	if (back_triggered) {
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		if (fe_data.pin_counter > 0) {//osea lease borre el ultimo caracter

			//encoder_pin_digits[fe_data.pin_counter] = CLEARED_DIGIT;
			//if (fe_data.pin_counter < 4)
			//	PVdispSendChar(' ', fe_data.pin_counter);
			//else {
			//	uint8_t i;
			//	for (i = 0; i < 3; i++) {
			//		PVdispSendChar('-', i);
			//	}
			//	PVdispSendChar(encoder_pin_digits[fe_data.pin_counter] + '0', 3);//escribo '-' en todos exepto el ultimo

			//}
			fe_data.pin_counter--;
			back_triggered = false;
		}
	}
	//ahora hay que fijarse si llego un evento de encoder, si es derecha aumento el numero si es izquierda lo achico si es enter avanzo
	if (PVCheckEvent()) {
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);//Reinicio timer
		event_t ev = PVGetEv();
		switch (ev) {
		case ENC_LEFT:
			if (actual_encoder_number == 0)
				actual_encoder_number = 9;
			else
				actual_encoder_number--;

			encoder_pin_digits[fe_data.pin_counter] = actual_encoder_number;
			//if (fe_data.pin_counter < 4) {
			//	uint8_t k = 0;
			//	for (k = 0; k < fe_data.pin_counter; k++) {
			//		PVdispSendChar('-', k);//LE pone simbolo '-' a todos expeto al ultimo que esta escribiendo
			//	}
			//	PVdispSendChar(actual_encoder_number + '0', fe_data.pin_counter);//Aca habr�a que ver bien el orden de los displays
			//}
			//else {
			//	PVdispSendChar('-', 2);//a lo sumo el unico que no es '-' es el que antees era el menos sigificativo antes del shid
			//	PVdispSendChar(actual_encoder_number + '0', 3);//Aca habr�a que ver bien el orden de los displays
			//}
			break;
		case ENC_RIGHT:
			if (actual_encoder_number == 9)
				actual_encoder_number = 0;

			else
				actual_encoder_number++;

			encoder_pin_digits[fe_data.pin_counter] = actual_encoder_number;
			//if (fe_data.pin_counter < 4) {
			//	uint8_t k = 0;
			//	for (k = 0; k < fe_data.pin_counter; k++) {
			//		PVdispSendChar('-', k);//LE pone simbolo '-' a todos expeto al ultimo que esta escribiendo
			//	}
			//	PVdispSendChar(actual_encoder_number + '0', fe_data.pin_counter);//Aca habr�a que ver bien el orden de los displays
			//}
			//else {
			//	PVdispSendChar('-', 2);//a lo sumo el unico que no es '-' es el que antees era el menos sigificativo antes del shid
			//	PVdispSendChar(actual_encoder_number + '0', 3);//Aca habr�a que ver bien el orden de los displays
			//}
			break;
		case BTN_PRESS:
			if (fe_data.pin_counter < PIN_LEN) {
				fe_data.pin_counter++;
			}
			else {
				if (checkPassword(transformToNum(&encoder_id_digits[0], ID_LEN), transformToNum(&encoder_pin_digits[0], PIN_LEN))) {
					updated_state = ACCESS;
					fe_data.good_pin = true;
				}
				else {
					addStrike(transformToNum(&encoder_id_digits[0], ID_LEN));
					fe_data.bad_pin = true;
					fe_data.pin_counter = 0;
					if (getBlockedStatus(transformToNum(&encoder_id_digits[0], ID_LEN))) {
						fe_data.blocked_user = true;
						updated_state = IDDLE;
					}
				}
			}
			break;
		default:
			break;
		}
	}

	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
	}
	return updated_state;

}

state accessRoutine(void) {
	state updated_state = ACCESS;
	static uint8_t selection=OPEN_SEL;

	if (prev_state != ACCESS) {
		fe_data.animation_en = false;
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		//desactivar las interrupciones de cancel y del
		FRDMButtonIRQ(cancel_switch, GPIO_IRQ_MODE_DISABLE, cancelCallback);
		FRDMButtonIRQ(back_switch, GPIO_IRQ_MODE_DISABLE, backCallback);
	}
	if (PVCheckEvent()) {
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);//Reinicio timer
		event_t ev = PVGetEv();
		switch (ev) {
		case ENC_LEFT:
			if (selection == OPEN_SEL)
				selection = USER_SEL;
			else 
				selection--;
			fe_data.animation_en = true;
			switch (selection) {
			case USER_SEL :
				fe_data.animation_opt = USER_SELECTED_ANIMATION;
				break;
			case OPEN_SEL:
				fe_data.animation_opt = OPEN_SELECTED_ANIMATION;
				break;
			case BRIGHT_SEL:
				fe_data.animation_opt = BRIGHTNESS_SELECTED_ANIMATION;
				break;
			default: break;
			}
			break;
		case ENC_RIGHT:
			if (selection == USER_SEL)
				selection = OPEN_SEL;
			else
				selection++;
			fe_data.animation_en = true;
			switch (selection) {
			case USER_SEL:
				fe_data.animation_opt = USER_SELECTED_ANIMATION;
				break;
			case OPEN_SEL:
				fe_data.animation_opt = OPEN_SELECTED_ANIMATION;
				break;
			case BRIGHT_SEL:
				fe_data.animation_opt = BRIGHTNESS_SELECTED_ANIMATION;
				break;
			default: break;
			}
			break;
		case BTN_PRESS :
			switch (selection) {
			case USER_SEL:
				updated_state = USERS;
				break;
			case OPEN_SEL:
				updated_state= OPEN;
				break;
			case BRIGHT_SEL:
				updated_state = BRIGHTNESS;
				break;
			default: break;
			}
			break;
		default: break;
		}
	}
	return updated_state;
}

state openRoutine(void) {
	state updated_state = OPEN;
	if (prev_state != OPEN) {
		fe_data.animation_en = true;
		fe_data.open = true;
		timerReset(open_timer_id);
		timerResume(open_timer_id);
	}
	if (open_triggered) {
		updated_state = IDDLE;
		open_triggered = false;
	}
	return updated_state;
}

state usersRoutine(void) {
	state updated_state = IDDLE;
	if (prev_state != USERS) {
		fe_data.animation_en = false;
	}
	return updated_state;
}

state brightnessRoutine(void) {
	state updated_state = BRIGHTNESS;
	//Si entr� aca , el estado actual es brightness
	//por lo que si el estado previo es distinto de brightenss 
	//es la primera vez que entro, por lo que debo iniciar el timer de timout


	if (prev_state != BRIGHTNESS) {
		//desactivar interrupciones de cancel y back
		fe_data.animation_en = false;
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
	}
	if (PVCheckEvent()) {
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		event_t ev = PVGetEv();
		if ((ev == ENC_LEFT) && (fe_data.brightness > 0)) {
			fe_data.brightness--;
		}
		else if ((ev == ENC_RIGHT) && (fe_data.brightness < 100)) {
			fe_data.brightness++;
		}
		else if (ev == BTN_PRESS) {
			updated_state = ACCESS;
			timerStop(inactivity_timer_id);
		}
	}

	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
	}
	return updated_state;
}

/*******************************************************************************
 * CALLBACKS DEFINITION
 ******************************************************************************/
void inactivityCallback(void) {
	inactivity_triggered = true;
}

void openCallback(void) {
	open_triggered = true;
}


void cancelCallback(void) {
	cancel_triggered = true;
}
void backCallback(void) {
	back_triggered = true;
}

/////////////////utility
uint32_t transformToNum(uint8_t* data, uint8_t count) {
	uint32_t ret = 0;
	uint32_t aux = 0;
	uint8_t i = 0;
	for (i = 0; i < count; i++) {
		aux = 0;
		aux = data[count - 1 - i] * pwr(10, i);
		ret += aux;
	}
	return ret;
}

uint32_t pwr(uint32_t num, uint32_t exp) {
	uint32_t ret = 1;
	uint8_t i = 0;
	for (i = 0; i < exp; i++) {
		ret *= num;
	}
	return ret;
}
