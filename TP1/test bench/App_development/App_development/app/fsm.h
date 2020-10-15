
/***************************************************************************//**
  @file     fsm.h
  @brief    fsm that controls the system
  @author   MAGT
 ******************************************************************************/
#pragma once
 /*******************************************************************************
  * INCLUDE HEADER FILES
  ******************************************************************************/
#include <cstdint>
  /*******************************************************************************
   * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
   ******************************************************************************/
#define INACTIVITY_TRIGGER_TIME (5000) //ms
#define OPEN_TRIGGER_TIME (5000) //ms
#define ID_LEN 8
#define PIN_LEN 5
const char BRIGHTNESS_MSG[4] = { 'B','R','G','H' };
const char OPEN_MSG[4] = { 'O','P','E','N' };
const char USER_MSG[4] = { 'U','S','E','R' };

const char PASSWORD_MSG[4] = { 'P','A','S','S' };
const char ADD_USER_MSG[4] = { 'A','D','D','U' };
const char DEL_USER_MSG[4] = { 'D','E','L','U' };

   /*******************************************************************************
    * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
    ******************************************************************************/

typedef enum {IDDLE,ASK_PIN,ACCESS,OPEN,USERS,BRIGHTNESS} state;
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