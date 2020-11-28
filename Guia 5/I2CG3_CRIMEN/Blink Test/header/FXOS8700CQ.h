/*
 * FXOS8700CQ.h
 *
 *  Created on: Nov 12, 2020
 *      Author: Guido
 */

#ifndef FXOS8700CQ_H_
#define FXOS8700CQ_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
} SRAWDATA;
typedef struct
{
	float x;
	float y;
	float z;
} SRAWDATA_f;

typedef void (* callbackptr) (void);

typedef enum { I2C_ERROR,  I2C_OK} I2C_FAIL;

typedef struct{
	SRAWDATA * pMagnData;
	SRAWDATA * pAccelData;
	callbackptr callback;
	I2C_FAIL error;
}read_data;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief configures the FXOS8700CQ for 200-Hz hybrid mode meaning that both
 * accelerometer data are provided at the 200-Hz rate.
 */
void FXOS8700CQInit(void);


/**
 * @brief Updates the accelerometer and magnetometer.
 *
 */
void ReadAccelMagnData(void);

/**
 * @brief returns if theres new information
 *
 */
bool getDataReady(void);

/**
 * @brief returns the acceleration vector.
 *
 */

SRAWDATA_f getAcc(void);

/**
 * @brief Returns the magnetization vector
 */

SRAWDATA_f getMag(void);

#endif /* FXOS8700CQ_H_ */
