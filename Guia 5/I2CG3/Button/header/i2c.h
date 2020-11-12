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

typedef void (*callbackptr)(void);

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

bool i2cInit(uint8_t chan);

bool i2cTransaction(uint8_t slave_, uint8_t reg_, uint8_t* data_, uint8_t size_, i2c_mode_t mode_, callbackptr callback_);

void i2cIsrHandler();

#endif // _I2C_H_
