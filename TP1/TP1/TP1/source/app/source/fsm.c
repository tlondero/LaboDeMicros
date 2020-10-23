/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "../headers/fsm.h"
#include "../../drivers/headers/timer.h"
#include "../../drivers/headers/PV.h"
#include "../../drivers/headers/Card.h"
#include "../../drivers/headers/FRDM.h"
#include "../headers/utility.h"
#include "../../app/headers/users.h"
 /*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_BRGHT 3
#define MIN_BRGHT 0

 /*******************************************************************************
  * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
  ******************************************************************************/
enum{OPEN_SEL,BRIGHT_SEL,USER_SEL};
enum{USERS_CLAVE_SEL, USERS_ADD_SEL, USERS_DEL_SEL};
  /*******************************************************************************
   * VARIABLE PROTOTYPES WITH FILE SCOPE
   ******************************************************************************/



//Pin buffer
static uint8_t encoder_pin_digits[PIN_LEN];
static uint8_t encoder_pin_digits_mirror[PIN_LEN];

//Id Buffer
static uint8_t encoder_id_digits[ID_LEN];
static uint8_t encoder_id_digits_mirror[ID_LEN];

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

//del_user
static void* aux_ptr;
static uint8_t aux_i;

//Fsm
static state prev_state = IDDLE;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE
 ******************************************************************************/
 /**
  * @brief Initial state of the app. In this state, the program is waiting for an ID given either via
  * a magnetic card or the rotary encoder.
  *
  * @return state variable with the updated state
  */
state IDDLERoutine(void);

uint32_t pwr(uint32_t num, uint32_t exp);

/**
 * @brief Once a registered ID is given, the app transitions to the askPin state where
 * the program waits for the valid PIN for the registered user. Inactivity timers and
 * cancel/back buttons are functional in this state.
 *
 * @return state variable with the updated state
 */
state askPinRoutine(void);
/**
 * @brief In this state, one has access to either open the door or configure certain
 * parameters, such as brightness or users, via the users menu.
 *
 * @return state variable with the updated state
 */
state accessRoutine(void);
/**
 * @brief The door remains open during this state.
 *
 * @return state variable with the updated state
 */
state openRoutine(void);
/**
 * @brief In this state, a menu is prompted where the user can modify his own pass
 * or, if the user is an admin, can add or delete users.
 *
 * @return state variable with the updated state
 */
state usersRoutine(void);
/**
 * @brief Using the rotary encoder one can change the brightness.
 *
 * @return state variable with the updated state
 */
state brightnessRoutine(void);
/***
 * @brief In this state one can modify his own password.
 *
 * @return state variable with the updated state
 */
state claveRoutine(void);
/**
 * @brief In this state, all deletable users are shown and can
 * be selected to be deleted.
 *
 * @return state variable with the updated state
 */
state delRoutine(void);
/**
 * @brief Here the program asks for an ID and PIN to
 * create a new non-admin user.
 *
 * @return state variable with the updated state
 */
state addRoutine(void);

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
//Saves the state of pin and id buffers
static void FSMPushState(void){
	int i = 0;
	for(i = 0; i<PIN_LEN; i++){
		encoder_pin_digits_mirror[i] = encoder_pin_digits[i];
		encoder_pin_digits[i] = 0;
	}
	for(i = 0; i<ID_LEN; i++){
			encoder_id_digits_mirror[i] = encoder_id_digits[i];
			encoder_id_digits[i] = 0;
	}
}
//Pulls the state of pin and id bufferss
static void FSMPullState(void){
	int i = 0;
	for(i = 0; i<PIN_LEN; i++)
		encoder_pin_digits[i] = encoder_pin_digits_mirror[i];

	for(i = 0; i<ID_LEN; i++)
		encoder_id_digits[i] =  encoder_id_digits_mirror[i];

}

//Initial state of the FSM.
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
	FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
	FRDMButtonIRQ(back_switch, BT_FEDGE, backCallback);
	FRDMLedSetFlash(3);

	initUsers();
	//Front-end related stuff
	fe_data.animation_en = true;
	fe_data.animation_opt = IDDLE_ANIMATION;
	fe_data.brightness = 2;
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
	fe_data.br = false;

	state updated_state;
	switch (actual_state) {
	case IDDLE:
		updated_state = IDDLERoutine();
		break;
	case ASK_PIN:
		updated_state = askPinRoutine();
		break;
	case ACCESS:
		updated_state = accessRoutine();
		break;
	case OPEN:
		updated_state = openRoutine();
		break;
	case USERS:
		updated_state = usersRoutine();
		break;
	case BRIGHTNESS:
		updated_state = brightnessRoutine();
		break;
	case USERS_CLAVE:
		updated_state = claveRoutine();
		break;
	case USERS_DEL:
		updated_state = delRoutine();
		break;
	case USERS_ADD:
		updated_state = addRoutine();
		break;
	default:
		break;
	}
	prev_state = actual_state;
	return updated_state;
}

//Getter de la data para el front end
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
		fe_data.pin_counter = 0;

		timerReset(inactivity_timer_id);

		FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, BT_FEDGE, backCallback);

		int i = 0;
		for(i = 0; i<ID_LEN; i++)
			encoder_id_digits[i] = 0;
		for(i = 0; i<PIN_LEN; i++)
			encoder_pin_digits[i] = 0;

		card_event = NULL;
	}

	/*
	* STATE RUN
	*/

	//Si no estoy usando el encoder, me fijo si el driver de tarjeta tiene nueva info
	if (!using_encoder) {
		card_event = cardGetPAN();
		if (card_event != NULL) {
			//Me fijo que exista el usuario y no este bloqueado
			if (checkExistance(transformToNum(card_event, ID_LEN))) {
				fe_data.animation_en = false;
				if (getBlockedStatus(transformToNum(card_event, ID_LEN))) {
					fe_data.blocked_user = true;
					fe_data.bad_id = true;
					fe_data.animation_en = true;
					updated_state = IDDLE;
				}
				else {
					//Paso a pedir el pin
					updated_state = ASK_PIN;
					fe_data.good_id = true;
					int i = 0;
					for( i = 0; i<ID_LEN; i++)
						encoder_id_digits[i] = card_event[i];
				}
			}
			else {
				card_event = NULL;
				fe_data.bad_id = true;
			}
		}
	}

	//Si llega un evento del encoder
	if (PVCheckEvent()) {
		fe_data.animation_en = false;
		using_encoder = true;
	}

	//Si estaba usando el encoder y aun no me tengo que ir del estado
	if (using_encoder && (updated_state == IDDLE)) {
		//Boton de cancel
		if (cancel_triggered) {
			fe_data.animation_en = true;
			fe_data.id_counter = 0;
			using_encoder = false;
			cancel_triggered = false;
		}
		//Boton de back
		else if (back_triggered) {
			if (fe_data.id_counter > 0) {
				fe_data.id_counter--;
			}
			back_triggered = false;
		}
		//Ahora hay que fijarse si llego un evento de encoder, si es derecha aumento el numero si es izquierda lo achico si es enter avanzo
		if (PVCheckEvent()) {
			event_t ev = PVGetEv();
			timerReset(inactivity_timer_id);
			switch (ev) {
			case ENC_LEFT:
				if (actual_encoder_number == 0)
					actual_encoder_number = 9;
				else
					actual_encoder_number--;

				encoder_id_digits[fe_data.id_counter] = actual_encoder_number;
				break;
			case ENC_RIGHT:
				if (actual_encoder_number == 9)
					actual_encoder_number = 0;

				else
					actual_encoder_number++;

				encoder_id_digits[fe_data.id_counter] = actual_encoder_number;
				break;
			case BTN_PRESS:
				if (fe_data.id_counter < ID_LEN-1) {
					fe_data.id_counter++;
					actual_encoder_number = 0;
				}
				else {
					//Una vez que toodo el id haya sido ingresado me fijo si el usuario existe y no esta bloqueado
					if (checkExistance(transformToNum(&encoder_id_digits[0], ID_LEN))) {
						if (getBlockedStatus(transformToNum(&encoder_id_digits[0], ID_LEN))) {
							//fe_data.blocked_user = true;
							fe_data.animation_en = true;
							fe_data.animation_opt = IDDLE_ANIMATION;
							fe_data.id_counter = 0;
							fe_data.bad_id = true;
							updated_state = IDDLE;
						}
						else {
							//Paso a pedir el pin.
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

	//Si el timer de inactividad expiró
	if (inactivity_triggered) {
			updated_state = IDDLE;
			inactivity_triggered = false;
			using_encoder = false;
		}
	return updated_state;
}
state askPinRoutine(void) {
	state updated_state = ASK_PIN;
	static uint8_t actual_encoder_number = 0;

	/*
	* STATE INIT
	*/
	if (prev_state != ASK_PIN) {
		fe_data.pin_counter = 0;
		fe_data.animation_en = false;

		timerReset(inactivity_timer_id);

		FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, BT_FEDGE, backCallback);
	}

	/*
	* STATE RUN
	*/
	//Si presionaron el boton de ir para atras se borra el ultimo caracter
	if (back_triggered) {
		timerReset(inactivity_timer_id);
		if (fe_data.pin_counter > 0) {//osea lease borre el ultimo caracter
			fe_data.pin_counter--;
			back_triggered = false;
		}
	}

	//Ahora hay que fijarse si llego un evento de encoder, si es derecha aumento el numero si es izquierda lo achico si es enter avanzo
	if (PVCheckEvent()) {
		timerReset(inactivity_timer_id);
		event_t ev = PVGetEv();
		switch (ev) {
		case ENC_LEFT:
			if (actual_encoder_number == 0)
				actual_encoder_number = 9;
			else
				actual_encoder_number--;

			encoder_pin_digits[fe_data.pin_counter] = actual_encoder_number;

			break;
		case ENC_RIGHT:
			if (actual_encoder_number == 9)
				actual_encoder_number = 0;

			else
				actual_encoder_number++;

			encoder_pin_digits[fe_data.pin_counter] = actual_encoder_number;

			break;
		case BTN_PRESS:
			if (fe_data.pin_counter < PIN_LEN-1) {
				fe_data.pin_counter++;
				actual_encoder_number = 0;
			}
			else {
				//Me fijo que la password coincida con el ID
				if (checkPassword(transformToNum(&encoder_id_digits[0], ID_LEN), transformToNum(&encoder_pin_digits[0], PIN_LEN))) {
					updated_state = ACCESS;
					fe_data.good_pin = true;
				}
				//Si no coincide, añado un strike, si ya fueron tres, lo bloqueo al usuario.
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

	//Si presionan cancelar, volvemos a IDDLE.
	if (cancel_triggered) {
		timerReset(inactivity_timer_id);
		fe_data.pin_counter = 0;
		updated_state= IDDLE;
		cancel_triggered = false;
	}

	//Si hubo inactividad, volvemos a IDDLE.
	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
	}
	return updated_state;

}

state accessRoutine(void) {
	state updated_state = ACCESS;
	static uint8_t selection=OPEN_SEL;

	/*
	* STATE INIT
	*/
	if (prev_state != ACCESS) {

		fe_data.animation_en = true;
		fe_data.animation_opt = OPEN_SELECTED_ANIMATION;

		timerReset(inactivity_timer_id);

		FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, BT_DISABLE, backCallback);
	}

	/*
	* STATE RUN
	*/
	//Me fijo si hubo un evento del encoder
	if (PVCheckEvent()) {
		timerReset(inactivity_timer_id);
		event_t ev = PVGetEv();
		//Si es izquierda, me muevo circularmente entre las opciones de menues a la izquierda
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
			//Si es derecha, me muevo circularmente entre las opciones de menues a la derecha
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
			//Si se presiona el encoder, vamos al estado elegido.
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

	//Si se presiono cancelar, se vuelve a iddle
	if (cancel_triggered) {
			timerReset(inactivity_timer_id);
			timerStop(inactivity_timer_id);
			cancel_triggered = false;
			updated_state = IDDLE;
		}

	//Si hubo inactividad, se vuelve a iddle
	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
	}
	return updated_state;
}

state openRoutine(void) {
	state updated_state = OPEN;

	/*
	* STATE INIT
	*/
	if (prev_state != OPEN) {

		fe_data.animation_en = true;
		fe_data.open = true;

		timerStop(inactivity_timer_id);
		timerReset(open_timer_id);

	}

	/*
	* STATE RUN
	*/
	//Espero a que expire el timer para cerrar la puerta y volver a iddle.
	if (open_triggered) {
		updated_state = IDDLE;
		open_triggered = false;
	}
	return updated_state;
}

state usersRoutine(void) {
	state updated_state = USERS;
	static uint8_t selection=USERS_CLAVE_SEL;

	/*
	* STATE INIT
	*/
	if (prev_state != USERS) {

		fe_data.animation_en = true;
		fe_data.animation_opt = CLAVE_SELECTED_ANIMATION;

		timerReset(inactivity_timer_id);

		FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, BT_FEDGE, backCallback);
	}

	/*
	* STATE RUN
	*/

	//Me fijo si hubo un evento del encoder.
	if (PVCheckEvent()) {
		timerReset(inactivity_timer_id);
		event_t ev = PVGetEv();
		switch (ev) {
		//Si es izquierda, me muevo circularmente entre las opciones de menues a la izquierda
		case ENC_LEFT:
			if(getAdminStatus(transformToNum(&encoder_id_digits[0], ID_LEN))){
				if (selection == USERS_CLAVE_SEL)
					selection = USERS_DEL_SEL;
				else
					selection--;
				switch (selection) {
				case USERS_CLAVE_SEL :
					fe_data.animation_opt = CLAVE_SELECTED_ANIMATION;
					break;
				case USERS_ADD_SEL:
					fe_data.animation_opt = ADD_SELECTED_ANIMATION;
					break;
				case USERS_DEL_SEL:
					fe_data.animation_opt = DEL_SELECTED_ANIMATION;
					break;
				default: break;
				}
			}
			break;
			//Si es derecha, me muevo circularmente entre las opciones de menues a la derecha
		case ENC_RIGHT:
			if(getAdminStatus(transformToNum(&encoder_id_digits[0], ID_LEN))){
				if (selection == USERS_DEL_SEL)
					selection = USERS_CLAVE_SEL;
				else
					selection++;
				switch (selection) {
				case USERS_CLAVE_SEL:
					fe_data.animation_opt = CLAVE_SELECTED_ANIMATION;
					break;
				case USERS_ADD_SEL:
					fe_data.animation_opt = ADD_SELECTED_ANIMATION;
					break;
				case USERS_DEL_SEL:
					fe_data.animation_opt = DEL_SELECTED_ANIMATION;
					break;
				default: break;
				}
			}
			break;
			//Si presionan el encoder, voy a ese estado.
		case BTN_PRESS:
			switch (selection) {
			case USERS_CLAVE_SEL:
				updated_state = USERS_CLAVE;
				break;
			case USERS_ADD_SEL:
				updated_state= USERS_ADD;
				break;
			case USERS_DEL_SEL:
				updated_state = USERS_DEL;
				break;
			default: break;
			}
			break;
		default: break;
		}
	}

	//Si hubo inactividad, vuelvo a iddle
	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
	}

	//Si presionaron atras, vuelvo a access.
	if (back_triggered) {
			back_triggered = false;
			updated_state = ACCESS;
			timerStop(inactivity_timer_id);
	}

	//Si presionaron cancelar, vuelvo a iddle.
	if (cancel_triggered) {
		cancel_triggered = false;
		updated_state = IDDLE;
		timerStop(inactivity_timer_id);
	}
	return updated_state;
}

state brightnessRoutine(void) {
	state updated_state = BRIGHTNESS;

	/*
	* STATE INIT
	*/
	if (prev_state != BRIGHTNESS) {

		fe_data.animation_en = false;
		timerReset(inactivity_timer_id);

		FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, BT_FEDGE, backCallback);
	}

	/*
	* STATE RUN
	*/
	//Si hubo un evento del encoder
	if (PVCheckEvent()) {
		timerReset(inactivity_timer_id);
		event_t ev = PVGetEv();
		//Si es a la izquierda disminuyo el brillo
		if ((ev == ENC_LEFT) && (fe_data.brightness > MIN_BRGHT)) {
			fe_data.brightness-=1;
			fe_data.br = true;
		}
		//Si es a la derecha aumento el brillo
		else if ((ev == ENC_RIGHT) && (fe_data.brightness < MAX_BRGHT)) {
			fe_data.brightness+=1;
			fe_data.br = true;
		}
		//Si apretan el encoder vuelvo al menu de user.
		else if (ev == BTN_PRESS) {
			updated_state = ACCESS;
			timerStop(inactivity_timer_id);
		}
	}

	//Si apretan atras vuelvo a access
	if (back_triggered) {
		back_triggered = false;
		updated_state = ACCESS;
		timerStop(inactivity_timer_id);
	}

	//Si apretan cancel vuelvo a iddle
	if (cancel_triggered) {
		cancel_triggered = false;
		updated_state = IDDLE;
		timerStop(inactivity_timer_id);
	}

	//Si hubo inactividad vuelvo a iddle
	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
		timerStop(inactivity_timer_id);
	}
	return updated_state;
}
state claveRoutine(void) {
	state updated_state = USERS_CLAVE;
	static uint8_t actual_encoder_number = 0;

	/*
	* STATE INIT
	*/
	if (prev_state != USERS_CLAVE) {
		fe_data.pin_counter = 0;
		fe_data.animation_opt = CLAVE_SELECTED_ANIMATION;
		fe_data.animation_en = true;

		timerReset(inactivity_timer_id);

		FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, BT_FEDGE, backCallback);
	}

	/*
	* STATE RUN
	*/

	//Si apretaron atras, vuelvo a users
	if (back_triggered) {
		back_triggered = false;
		updated_state = USERS;
		timerStop(inactivity_timer_id);
	}

	//Si apretaron cancel, vuelvo a iddle
	if (cancel_triggered) {
		cancel_triggered = false;
		updated_state = IDDLE;
		timerStop(inactivity_timer_id);
	}

	//Si aun tengo que permanecer en este estado y hubo evento del encoder
	//Si es hacia la izquierda decremento el numero
	//Si es hacia la derecha incremento el numero
	//Si apretan el encoder voy al siguiente numero del PIN
	if ((updated_state == USERS_CLAVE) && (PVCheckEvent())) {
		timerReset(inactivity_timer_id);
		event_t ev = PVGetEv();
		switch (ev) {
		case ENC_LEFT:
			if (actual_encoder_number == 0)
				actual_encoder_number = 9;
			else
				actual_encoder_number--;

			encoder_pin_digits[fe_data.pin_counter] = actual_encoder_number;
			break;
		case ENC_RIGHT:
			if (actual_encoder_number == 9)
				actual_encoder_number = 0;

			else
				actual_encoder_number++;

			encoder_pin_digits[fe_data.pin_counter] = actual_encoder_number;
			break;
		case BTN_PRESS:
			if (fe_data.pin_counter < PIN_LEN-1) {
				fe_data.pin_counter++;
			}
			else {
				//Si ya se ingresaron todos los numeros del PIN, cambio la contraseña
				setPassword(transformToNum(&encoder_id_digits[0], ID_LEN), transformToNum(&encoder_pin_digits[0], PIN_LEN));
				fe_data.good_pin = 1;
				updated_state = USERS;
			}
			break;
		default:
			break;
		}
	}

	//Si hubo inactividad, vuelvo a iddle
	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
	}
	return updated_state;
}

state addRoutine(void) {
	state updated_state = USERS_ADD;
	static uint8_t actual_encoder_number = 0;
	static bool using_encoder = false;
	static bool asking_pin = false;
	uint8_t* card_event = 0;

	/*
	* STATE INIT
	*/
	if (prev_state != USERS_ADD) {
		FSMPushState();

		fe_data.animation_en = true;
		fe_data.animation_opt = ADD_SELECTED_ANIMATION;
		fe_data.id_counter = 0;
		fe_data.pin_counter = 0;

		timerReset(inactivity_timer_id);

		FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, BT_FEDGE, backCallback);

		//Limpio todos los registros a utilizar para cargar las nuevas credenciales
		using_encoder = false;
		asking_pin = false;
		int i = 0;
		for(i = 0; i<ID_LEN; i++)
			encoder_id_digits[i] = 0;
		for(i = 0; i<PIN_LEN; i++)
			encoder_pin_digits[i] = 0;

		card_event = NULL;

	}

	/*
	* STATE RUN
	*/
	//Si tocaron atras, vuelvo a users
	if (back_triggered) {
		back_triggered = false;
		updated_state = USERS;
		timerStop(inactivity_timer_id);
	}

	//Si tocaron cancel vuelvo a iddle
	if (cancel_triggered) {
		cancel_triggered = false;
		updated_state = IDDLE;
		timerStop(inactivity_timer_id);
	}

	//Si hubo inactividad, vuelvo a iddle
	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
	}

	//Carga del ID mediante tarjeta
	if (!using_encoder) {
		card_event = cardGetPAN();
		if (card_event != NULL) {
			asking_pin = true;
			fe_data.good_id = true;
			fe_data.animation_en = false;
			fe_data.id_counter = ID_LEN+1;
			int i = 0;
			for( i = 0; i<ID_LEN; i++)
				encoder_id_digits[i] = card_event[i];
		}
	}

	//Carga del ID mediante encoder.
	if (PVCheckEvent()) {
		fe_data.animation_en = false;
		using_encoder = true;
	}
	if (using_encoder && (updated_state == USERS_ADD) && !asking_pin) {
		if (PVCheckEvent()) {
			event_t ev = PVGetEv();
			timerReset(inactivity_timer_id);
			switch (ev) {
			//Si es izquierda decremento el numero circularmente
			case ENC_LEFT:
				if (actual_encoder_number == 0)
					actual_encoder_number = 9;
				else
					actual_encoder_number--;

				encoder_id_digits[fe_data.id_counter] = actual_encoder_number;
				break;
			//Si es derecha incremento el numero circularmente
			case ENC_RIGHT:
				if (actual_encoder_number == 9)
					actual_encoder_number = 0;

				else
					actual_encoder_number++;

				encoder_id_digits[fe_data.id_counter] = actual_encoder_number;
				break;
			//Si apretaron el encoder paso al siguiente digito del ID
			case BTN_PRESS:
				if (fe_data.id_counter < ID_LEN) {
					fe_data.id_counter++;
					actual_encoder_number = 0;
					//Si ya se ingresaron todos los digitos, pide el pin poniendo asking_pin en true.
					if (fe_data.id_counter == ID_LEN){
						fe_data.good_id = true;
						fe_data.animation_en = false;
						asking_pin = true;
					}
				}

				break;
			default:
				break;
			}
		}
	}

	//Carga del pin solamente mediante el encoder rotatorio
	if (asking_pin && (updated_state == USERS_ADD) && (PVCheckEvent())) {
		timerReset(inactivity_timer_id);
		event_t ev = PVGetEv();
		switch (ev) {
		case ENC_LEFT:
			if (actual_encoder_number == 0){
				actual_encoder_number = 9;
			}
			else{
				actual_encoder_number--;
			}
			encoder_pin_digits[fe_data.pin_counter] = actual_encoder_number;

			break;
		case ENC_RIGHT:
			if (actual_encoder_number == 9){
				actual_encoder_number = 0;
			}
			else{
				actual_encoder_number++;
			}
			encoder_pin_digits[fe_data.pin_counter] = actual_encoder_number;

			break;
		case BTN_PRESS:
			if (fe_data.pin_counter < PIN_LEN-1) {
				fe_data.pin_counter++;
				actual_encoder_number = 0;
			}
			else {
				user_t new_user;
				new_user.admin = false;
				new_user.available = true;
				new_user.blocked = false;
				new_user.id = transformToNum(&encoder_id_digits[0], ID_LEN);
				new_user.password = transformToNum(&encoder_pin_digits[0], PIN_LEN);
				new_user.strikes = 0;
				updated_state = USERS;
				fe_data.good_pin = true;
				//Añado el nuevo usuario a la base de datos
				addUser(new_user);
			}
			break;
		default:
			break;
		}
	}

	if(updated_state != USERS_ADD)
		FSMPullState();

	return updated_state;
}
state delRoutine(void) {
	int8_t counter = 0;
	state updated_state = USERS_DEL;

	/*
	* STATE INIT
	*/
	if (prev_state != USERS_DEL) {
		fe_data.animation_opt = DEL_SELECTED_ANIMATION;
		fe_data.pin_counter = 0;
		fe_data.animation_en = true;

		timerReset(inactivity_timer_id);

		FRDMButtonIRQ(cancel_switch, BT_FEDGE, cancelCallback);
		FRDMButtonIRQ(back_switch, BT_FEDGE, backCallback);

		//Busco el primero usuario deleteable de la base de datos.
		//Deleteable es que no sea admin y que sea un usuario activado
		aux_ptr = (void*) getUsersList();
		aux_i = 0;
		counter = 0;
		while(true){
			if(counter == MAX_USERS){
				fe_data.del_user_ptr = 0;
				fe_data.del_i = MAX_USERS;
				break;
			}
			else if (!(((user_t*) aux_ptr)[aux_i%MAX_USERS]).admin && (((user_t*) aux_ptr)[aux_i%MAX_USERS]).available) {
				fe_data.del_user_ptr = &(((user_t*) aux_ptr)[aux_i%MAX_USERS]);
				fe_data.del_i = aux_i%MAX_USERS;
				break;
			}
			aux_i++;
			counter++;
		}
	}

	/*
	* STATE RUN
	*/
	//Si apretaron atras, vuelvo a users
	if (back_triggered) {
		back_triggered = false;
		updated_state = USERS;
		timerStop(inactivity_timer_id);
	}
	//Si apretaron cancelar, vuelvo a iddle
	if (cancel_triggered) {
		cancel_triggered = false;
		updated_state = IDDLE;
		timerStop(inactivity_timer_id);
	}
	//Si hubo inactividad, vuelvo a iddle
	if (inactivity_triggered) {
		updated_state = IDDLE;
		inactivity_triggered = false;
	}
	//Si hubo un evento del encoder hacia izquierda o derecha, paso al siguiente usuario deletable de la base de datos
	if (PVCheckEvent()) {
		fe_data.animation_en = false;
		timerReset(inactivity_timer_id);
		event_t ev = PVGetEv();
		switch (ev) {
		case ENC_LEFT:
			//Busco el siguiente usario hacia la izquierda
			counter = 0;
			while(true){
				aux_i--;
				if(counter == MAX_USERS){
					fe_data.del_user_ptr = 0;
					fe_data.del_i = MAX_USERS;
					break;
				}
				else if (!(((user_t*) aux_ptr)[aux_i%MAX_USERS]).admin && (((user_t*) aux_ptr)[aux_i%MAX_USERS]).available) {
					fe_data.del_user_ptr = &(((user_t*) aux_ptr)[aux_i%MAX_USERS]);
					fe_data.del_i = aux_i%MAX_USERS;
					break;
				}
				counter++;
			}
			break;
		case ENC_RIGHT:
			//Busco el siguiente usario hacia la derecha
			counter = 0;
			while(true){
				aux_i++;
				if(counter == MAX_USERS){
					fe_data.del_user_ptr = 0;
					fe_data.del_i = MAX_USERS;
					break;
				}
				else if (!(((user_t*) aux_ptr)[aux_i%MAX_USERS]).admin && (((user_t*) aux_ptr)[aux_i%MAX_USERS]).available) {
					fe_data.del_user_ptr = &(((user_t*) aux_ptr)[aux_i%MAX_USERS]);
					fe_data.del_i = aux_i%MAX_USERS;
					break;
				}
				counter++;
			}
			break;
			//Si presionan, borro al usuario
		case BTN_PRESS:
			deleteUser(((user_t*)(fe_data.del_user_ptr))->id);
			fe_data.del_user = true;
			updated_state = USERS;
			break;
		default: break;
		}
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


