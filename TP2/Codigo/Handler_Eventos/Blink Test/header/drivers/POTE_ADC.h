/*
 * POTE_ADC.h
 *
 *  Created on: Nov 28, 2020
 *      Author: Acer
 */

#ifndef DRIVERS_POTE_ADC_H_
#define DRIVERS_POTE_ADC_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "header/event_handlers/paquetes.h"
#include "header/drivers/ADC.h"

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
//Inicializa el potenciometro y lo que necesitan los servicios
void PoteInit(void);

//Recibe un package y lo modifica si hay algun evento en el pote
void PoteGetEvent(package* pkg);

#endif /* DRIVERS_POTE_ADC_H_ */
