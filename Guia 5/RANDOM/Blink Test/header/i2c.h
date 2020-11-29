#ifndef _I2C_H_
#define _I2C_H_

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

bool i2cInit(ic2_channel_t chan);

bool i2cTransaction(uint8_t slave_, uint8_t reg_, uint8_t *data_, uint8_t size_,
		i2c_mode_t mode_, callbackptr callback_);

I2C_FAULT i2cStatus(void);

#endif // _I2C_H_
