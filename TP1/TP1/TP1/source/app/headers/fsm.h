
/***************************************************************************//**
  @file     fsm.h
  @brief    fsm that controls the system
  @author   MAGT
 ******************************************************************************/
 /*******************************************************************************
  * INCLUDE HEADER FILES
  ******************************************************************************/
#ifndef _FSM_H_
#define _FSM_H_
#include <stdint.h>
#include "FEData.h"
  /*******************************************************************************
   * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
   ******************************************************************************/
#define INACTIVITY_TRIGGER_TIME (30000) //ms
#define OPEN_TRIGGER_TIME (5000) //ms
#define ID_LEN 8
#define PIN_LEN 5




/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/

typedef enum {IDDLE,ASK_PIN,ACCESS,OPEN,USERS,BRIGHTNESS ,USERS_CLAVE, USERS_ADD , USERS_DEL} state;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

 /*******************************************************************************
  * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
  ******************************************************************************/
  /**
   * @brief Starts the FSM with the initial state.
   *
   * @return state variable witht he inital state
   */
state FSMInitState(void);
/**
 * @brief RUns the FSM.
 * @param actual_state, the actual state of the FSM
 * @return state, the state updated after the fsm was executed.
 */
state FSMRun(state actual_state);

/**
 * @brief Returns the necessary data for the front end.
 * @return FEData, dedicated structure in FEData.h.
 */
FEData  * const FSMGetFEData(void);

#endif
