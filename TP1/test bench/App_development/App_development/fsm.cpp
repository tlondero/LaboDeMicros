/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "fsm.h"
 /*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

 /*******************************************************************************
  * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
  ******************************************************************************/
  /*******************************************************************************
   * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
   ******************************************************************************/
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
   /*******************************************************************************
	* FUNCTION DEFINITION WITH GLOBAL SCOPE
	******************************************************************************/

state FSMInitState(void) {
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
    state updated_state = IDDLE;
    return updated_state;
}
state usersRoutine(void) {
    state updated_state = IDDLE;
    return updated_state;
}
state brightnessRoutine(void) {
    state updated_state = IDDLE;
    return updated_state;
}