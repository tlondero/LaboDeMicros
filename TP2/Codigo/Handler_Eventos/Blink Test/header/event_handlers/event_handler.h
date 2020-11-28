/***************************************************************************/ /**
  @file     event_handler.h
  @brief
  @author   MAGT
 ******************************************************************************/

#ifndef EVENT_HANDLERS_EVENT_HANDLER_H_
#define EVENT_HANDLERS_EVENT_HANDLER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

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
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief calls to all the Peripheral devices and ask them for events then proceeds to execute the events received
*/
void EvHandGetEvents(void);


#endif /* EVENT_HANDLERS_EVENT_HANDLER_H_ */
