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

enum {
	I2C_0, I2C_1, I2C_2, I2C_COUNT
};

typedef enum {
	I2C_WRITE, I2C_READ
} i2c_mode_t;

typedef void (*callbackPtr)(void);

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

bool i2cInit(uint8_t chan);

bool i2cTransaction(uint8_t slave, uint8_t reg, uint8_t* data, uint8_t dSize, i2c_mode_t mode_, callbackPtr callback_);

void i2cISR_HANDLER();

#endif // _I2C_H_
