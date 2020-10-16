/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "fsm.h"
#include "../drivers/Timer.h"
#include "../drivers/placaVerde.h"
#include "../drivers/Card.h"
#include "../drivers/FRDM.h"
#include "../app/users.h"
 /*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CLEARED_DIGIT 10
 /*******************************************************************************
  * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
  ******************************************************************************/
  /*******************************************************************************
   * VARIABLE PROTOTYPES WITH FILE SCOPE
   ******************************************************************************/
static tim_id_t inactivity_timer_id;
static tim_id_t open_timer_id;
static const uint8_t cancel_switch = BUTTON_SW2;
static const uint8_t back_switch = BUTTON_SW3;
static encoder_id my_encoder_id;
static uint8_t encoder_id_digits[ID_LEN];
static uint8_t encoder_pin_digits[PIN_LEN];

static bool inactivity_triggered;
static bool open_triggered;
static bool cancel_triggered;
static bool back_triggered;

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
	inactivity_timer_id = timerGetId();
	open_timer_id = timerGetId();
	my_encoder_id = PVencoder_register();
	timerStart(inactivity_timer_id, TIMER_MS2TICKS(INACTIVITY_TRIGGER_TIME), TIM_MODE_SINGLESHOT, inactivityCallback);
	timerStop(inactivity_timer_id);
	timerStart(open_timer_id, TIMER_MS2TICKS(OPEN_TRIGGER_TIME), TIM_MODE_SINGLESHOT, openCallback);
	timerStop(open_timer_id);
	//Tengo que activar las interrupciones de cancel y delete
	FRDMButtonIRQ(cancel_switch, GPIO_IRQ_MODE_FALLING_EDGE, cancelCallback);
	FRDMButtonIRQ(back_switch, GPIO_IRQ_MODE_FALLING_EDGE, backCallback);
	PVAnimation(IDDLE_ANIMATION, true);
	return IDDLE;
}

state FSMRun(state actual_state) {

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

/*******************************************************************************
 * FUNCTION DEFINITION WITH FILE SCOPE
 ******************************************************************************/
state IDDLERoutine(void) {
	static bool using_encoder = false;
	static uint8_t id_counter = 0;
	static uint8_t actual_encoder_number = 0;

	uint8_t* card_event = 0;
	if (!using_encoder) {
		card_event = cardGetPAN();
	}
	state updated_state = IDDLE;
	if (prev_state != IDDLE) {
		//PVAnimation(IDDLE_ANIMATION, true);
		id_counter = 0;
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		FRDMButtonIRQ(cancel_switch, GPIO_IRQ_MODE_FALLING_EDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, GPIO_IRQ_MODE_FALLING_EDGE, backCallback);
		//activar las interrupciones de cancel y del
	}
	if (PVencoder_event_avb(my_encoder_id)) {
		//PVAnimation(IDDLE_ANIMATION, false);
		using_encoder = true;
	}
	if (using_encoder) {
		if (cancel_triggered) {
			//uint8_t i = 0;
			//for (i = 0; i < id_counter; i++) {
			//	encoder_id_digits[i] = CLEARED_DIGIT; //sospechado al p2
			//}
			//PVAnimation(IDDLE_ANIMATION, true);
			id_counter = 0;
			using_encoder = false;
			cancel_triggered = false;
		}
		if (back_triggered) {
			if (id_counter > 0) {//osea lease borre el ultimo caracter
				//encoder_id_digits[id_counter] = CLEARED_DIGIT;
				//if (id_counter < 4)
				//	PVdispSendChar(' ', id_counter);
				//else {
				//	uint8_t i;
				//	for (i = 0; i < 4; i++) {
				//		PVdispSendChar(encoder_id_digits[id_counter - 1 - i] + '0', 3 - i);
				//	}
				//}
				//////////////////////////////////////////////////
				// ACA HAY QUE VER PARA QUE EN EL DISPLAY NO QUEDE CAGADO //creo que lo hice bien pero estaría bueno que me lo chequeens
				//////////////////////////////////////////////////////////
				id_counter--;
			}
			back_triggered = false;
		}
		//ahora hay que fijarse si llego un evento de encoder, si es derecha aumento el numero si es izquierda lo achico si es enter avanzo
		if (PVencoder_event_avb(my_encoder_id)) {
			event_t ev = PVencoder_pop_event(my_encoder_id);
			switch (ev) {
			case LEFT_TURN:
				if (actual_encoder_number == 0)
					actual_encoder_number = 9;
				else
					actual_encoder_number--;
				//if (id_counter < 4) {

				//	PVdispSendChar(actual_encoder_number + '0', id_counter);//Aca habría que ver bien el orden de los displays
				//}
				//else {
				//	PVdispSendChar(actual_encoder_number + '0', 3);//Aca habría que ver bien el orden de los displays
				//}
				break;
			case RIGHT_TURN:
				if (actual_encoder_number == 9)
					actual_encoder_number = 0;

				else
					actual_encoder_number++;
				//if (id_counter < 4) {

				//	PVdispSendChar(actual_encoder_number + '0', id_counter);//Aca habría que ver bien el orden de los displays
				//}
				//else {
				//	PVdispSendChar(actual_encoder_number + '0', 3);//Aca habría que ver bien el orden de los displays
				//}
				break;
			case BUTTON_PRESS:
				if (id_counter < ID_LEN) {
					encoder_id_digits[id_counter] = actual_encoder_number;
					//PVdispShift(LEFT);
					id_counter++;
				}
				else {
					if (checkExistance(transformToNum(&encoder_id_digits[0], ID_LEN))) {
						if (getBlockedStatus(transformToNum(&encoder_id_digits[0], ID_LEN))) {
							//PVAnimation(BLOCKED_ANIMATION, true);
							updated_state = IDDLE;
						}
						else {
							updated_state = ASK_PIN;
						}
					}
					//else {
						//PVAnimation(INVALID_ID_ANIMATION, true);
					//}
				}
				break;
			default:
				break;
			}
		}

	}
	else if (card_event != NULL) {
		PVAnimation(IDDLE_ANIMATION, false);
		if (checkExistance(transformToNum(card_event, ID_LEN))) {
			if (getBlockedStatus(transformToNum(card_event, ID_LEN))) {
				//PVAnimation(BLOCKED_ANIMATION, true);
				updated_state = IDDLE;
			}
			else {
				updated_state = ASK_PIN;
			}
		}
		else {
			card_event = NULL;
			//PVAnimation(INVALID_ID_ANIMATION, true);
		}

	}
	return updated_state;
}
state askPinRoutine(void) {
	state updated_state = ASK_PIN;
	static uint8_t pin_counter = 0;
	static uint8_t actual_encoder_number = 0;

	if (prev_state != ASK_PIN) {
		pin_counter = 0;
		PVAnimation(IDDLE_ANIMATION, false);//apago animaciones
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		FRDMButtonIRQ(cancel_switch, GPIO_IRQ_MODE_FALLING_EDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, GPIO_IRQ_MODE_FALLING_EDGE, backCallback);
		//activar las interrupciones de cancel y del
	}

	if (cancel_triggered) {
		uint8_t i = 0;
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		//for (i = 0; i < pin_counter; i++) {
		//	encoder_pin_digits[i] = CLEARED_DIGIT;
		//}
		pin_counter = 0;
		cancel_triggered = false;
	}
	if (back_triggered) {
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		if (pin_counter > 0) {//osea lease borre el ultimo caracter
			//encoder_pin_digits[pin_counter] = CLEARED_DIGIT;
			//if (pin_counter < 4)
			//	PVdispSendChar(' ', pin_counter);
			//else {
			//	uint8_t i;
			//	for (i = 0; i < 3; i++) {
			//		PVdispSendChar('-', i);
			//	}
			//	PVdispSendChar(encoder_pin_digits[pin_counter] + '0', 3);//escribo '-' en todos exepto el ultimo

			//}
			pin_counter--;
			back_triggered = false;
		}
	}
	//ahora hay que fijarse si llego un evento de encoder, si es derecha aumento el numero si es izquierda lo achico si es enter avanzo
	if (PVencoder_event_avb(my_encoder_id)) {
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);//Reinicio timer
		event_t ev = PVencoder_pop_event(my_encoder_id);
		switch (ev) {
		case LEFT_TURN:
			if (actual_encoder_number == 0)
				actual_encoder_number = 9;
			else
				actual_encoder_number--;
			//if (pin_counter < 4) {
			//	uint8_t k = 0;
			//	for (k = 0; k < pin_counter; k++) {
			//		PVdispSendChar('-', k);//LE pone simbolo '-' a todos expeto al ultimo que esta escribiendo
			//	}
			//	PVdispSendChar(actual_encoder_number + '0', pin_counter);//Aca habría que ver bien el orden de los displays
			//}
			//else {
			//	PVdispSendChar('-', 2);//a lo sumo el unico que no es '-' es el que antees era el menos sigificativo antes del shid
			//	PVdispSendChar(actual_encoder_number + '0', 3);//Aca habría que ver bien el orden de los displays
			//}
			break;
		case RIGHT_TURN:
			if (actual_encoder_number == 9)
				actual_encoder_number = 0;

			else
				actual_encoder_number++;
			//if (pin_counter < 4) {
			//	uint8_t k = 0;
			//	for (k = 0; k < pin_counter; k++) {
			//		PVdispSendChar('-', k);//LE pone simbolo '-' a todos expeto al ultimo que esta escribiendo
			//	}
			//	PVdispSendChar(actual_encoder_number + '0', pin_counter);//Aca habría que ver bien el orden de los displays
			//}
			//else {
			//	PVdispSendChar('-', 2);//a lo sumo el unico que no es '-' es el que antees era el menos sigificativo antes del shid
			//	PVdispSendChar(actual_encoder_number + '0', 3);//Aca habría que ver bien el orden de los displays
			//}
			break;
		case BUTTON_PRESS:
			if (pin_counter < PIN_LEN) {
				encoder_pin_digits[pin_counter] = actual_encoder_number;
				//if (pin_counter > 3)
					//PVdispShift(LEFT);
				pin_counter++;
			}
			else {
				if (checkPassword(transformToNum(&encoder_pin_digits[0], ID_LEN), transformToNum(&encoder_pin_digits[0], PIN_LEN)))
					updated_state = ACCESS;
				else {
					//PVAnimation(INVALID_PIN_ANIMATION, true);
					addStrike(transformToNum(&encoder_pin_digits[0], ID_LEN));
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
	if (prev_state != ACCESS) {
		//PVAnimation(IDDLE_ANIMATION, false);
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		//desactivar las interrupciones de cancel y del
		FRDMButtonIRQ(cancel_switch, GPIO_IRQ_MODE_DISABLE, cancelCallback);
		FRDMButtonIRQ(back_switch, GPIO_IRQ_MODE_DISABLE, backCallback);
	}

	return updated_state;
}
state openRoutine(void) {
	state updated_state = OPEN;
	if (prev_state != OPEN) {
		//PVAnimation(OPEN_ANIMATION, true);
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
		//PVAnimation(IDDLE_ANIMATION, false);
	}
	return updated_state;
}

state brightnessRoutine(void) {
	state updated_state = BRIGHTNESS;
	//Si entré aca , el estado actual es brightness
	//por lo que si el estado previo es distinto de brightenss 
	//es la primera vez que entro, por lo que debo iniciar el timer de timout


	if (prev_state != BRIGHTNESS) {
		//desactivar interrupciones de cancel y back
		//PVAnimation(BRIGHTNESS_ANIMATION, true);        //Aca debería mandar al display la palabra acorde a BRIGHTNESS para ver como cambia
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
	}
	if (PVencoder_event_avb(my_encoder_id)) {
		timerReset(inactivity_timer_id);
		timerResume(inactivity_timer_id);
		event_t ev = PVencoder_pop_event(my_encoder_id);
		if (ev == LEFT_TURN) {
			//PVDecreaseBrightness();
		}
		else if (ev == RIGHT_TURN) {
			//PVIncreaseBrightness();
		}
		else if (ev == BUTTON_PRESS) {
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
	uint8_t j = 0;;
	for (i = 0; i < count; i++) {
		aux = 0;
		for (j = 0; j < i; j++) {
			aux += data[count - 1 - i] * 10;
		}
		ret += aux;
	}
	return ret;
}