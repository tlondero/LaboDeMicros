
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