#ifndef _RAND_H_
#define _RAND_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum {
	I2C_NO_FAULT, I2C_BUS_BUSY, I2C_TIMEOUT, I2C_SLAVE_ERROR
} I2C_FAULT;

typedef enum {
	I2C_0, I2C_1, I2C_2, I2C_COUNT
} ic2_channel_t;

typedef enum {
	I2C_WRITE, I2C_READ
} i2c_mode_t;

typedef void (*callbackptr)(void);

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

bool RandInit(void);

uint32_t RandGet(void);


#endif // _RAND_H_
