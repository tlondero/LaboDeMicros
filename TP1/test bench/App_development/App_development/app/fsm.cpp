/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "fsm.h"
#include "../drivers/Timer.h"
#include "../drivers/placaVerde.h"
 /*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

 /*******************************************************************************
  * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
  ******************************************************************************/
  /*******************************************************************************
   * VARIABLE PROTOTYPES WITH FILE SCOPE
   ******************************************************************************/
static tim_id_t inactivity_timer_id;
static tim_id_t open_timer_id;

static encoder_id my_encoder_id;

static bool inactivity_triggered;
static bool open_triggered;

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
    return IDDLE;
}

state FSMRun(state actual_state) {
    
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
    state updated_state= IDDLE;
    return updated_state;
}
state askPinRoutine(void) {
    state updated_state = IDDLE;
    return updated_state;
}
state accessRoutine(void) {
    state updated_state = IDDLE;
    return updated_state;
}
state openRoutine(void) {
    state updated_state = OPEN;
    if (prev_state != OPEN) {
        timerReset(open_timer_id);
        timerResume(open_timer_id);
        PVAnimation(true);
    }
    if (open_triggered) {
        PVAnimation(false);
        updated_state = IDDLE;
    }
    return updated_state;
}
state usersRoutine(void) {
    state updated_state = IDDLE;
    return updated_state;
}
state brightnessRoutine(void){
    state updated_state = BRIGHTNESS;
    //Si entré aca , el estado actual es brightness
    //por lo que si el estado previo es distinto de brightenss 
    //es la primera vez que entro, por lo que debo iniciar el timer de timout



    //Aca debería mandar al display la palabra acorde a BRIGHTNESS para ver como cambia
    PVdispSendChar(BRIGHTNESS_MSG[3], 3);
    PVdispSendChar(BRIGHTNESS_MSG[2], 2);
    PVdispSendChar(BRIGHTNESS_MSG[1], 1);
    PVdispSendChar(BRIGHTNESS_MSG[0], 0);



    if (prev_state != BRIGHTNESS) {
        timerReset(inactivity_timer_id);
        timerResume(inactivity_timer_id);
    }
    if (PVencoder_event_avb(my_encoder_id)) {
        timerReset(inactivity_timer_id);
        timerResume(inactivity_timer_id);
        event_t ev = PVencoder_pop_event(my_encoder_id);
        if (ev == LEFT_TURN) {
            PVDecreaseBrightness();
        }
        else if (ev == RIGHT_TURN) {
            PVIncreaseBrightness();
        }
        else if (ev == BUTTON_PRESS) {
            updated_state = ACCESS;
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