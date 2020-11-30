/*
 * spi_botones.h
 *
 *  Created on: 28 nov. 2020
 *      Author: Msrt
 */

#ifndef SPI_BOTONES_H_
#define SPI_BOTONES_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "./SPI.h"
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct
{
	bool left;
	bool right;
	bool down;
	bool rotate;
	bool pause_resume;
	bool reset;
} button_status;

typedef uint8_t action_589;
typedef button_status* _status_pointer;
typedef const button_status* status_pointer;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes the SPI module for use with a 589 shift register
 */
void SPI_589_init(void);

/**
 * @brief Updates the state of the buttons. To be used before the getter.
 */
void SPI_589_update_buttons(void);

/**
 * @brief Getter of the state of the buttons.
 * @returns the state of the buttons.
 */
status_pointer SPI_589_get_buttons_status(void);
#endif /* SPI_BOTONES_H_ */
