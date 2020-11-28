/***************************************************************************/ /**
  @file     uartPackHand.h
  @brief
  @author   MAGT
 ******************************************************************************/

#ifndef UARTPACKHAND_H_
#define UARTPACKHAND_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "paquetes.h"
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PACKAGE_SIZE (4)


#define DEV_MODE 1


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
 * @brief interprets the packages specified by the "paquetes.h" file and fills the corresponding data structure acordingly
 * @param
 * @data * is a pointer tpo the package data structure to be filled
 * @id UART's number
 * @return A new byte has being received
*/
void uartPackProcess(package * data, uint8_t id );

#endif /* UARTPACKHAND_H_ */
