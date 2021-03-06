

 /***************************************************************************/ /**
   @file   * AccelerometerEvents.h
   @brief
   @author   MAGT
  ******************************************************************************/

#ifndef ACCELEROMETEREVENTS_H_
#define ACCELEROMETEREVENTS_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "paquetes.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define THRESHOLD (0.5)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void accelerometerGetEvent(package * data);
#endif /* ACCELEROMETEREVENTS_H_ */
