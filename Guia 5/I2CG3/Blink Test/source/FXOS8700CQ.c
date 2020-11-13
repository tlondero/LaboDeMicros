/*
 * FXOS8700CQ.c
 *
 *  Created on: Nov 12, 2020
 *      Author: Guido
 */

#include "header/FXOS8700CQ.h"
#include "header/i2c.h"

////////////////////////////////////////Que cuando termina la transmision llamas a el callback que te doy//////////////////////////////////////////////////////////////////////////
// FXOS8700CQ I2C address
#define FXOS8700CQ_SLAVE_ADDR 0x1D // with pins SA0=0, SA1=0 //the default for the FRDM K64 board.

// FXOS8700CQ internal register addresses
#define FXOS8700CQ_STATUS 0x00
#define FXOS8700CQ_WHOAMI 0x0D
#define FXOS8700CQ_XYZ_DATA_CFG 0x0E
#define FXOS8700CQ_CTRL_REG1 0x2A
#define FXOS8700CQ_M_CTRL_REG1 0x5B
#define FXOS8700CQ_M_CTRL_REG2 0x5C
#define FXOS8700CQ_WHOAMI_VAL 0xC7
// number of bytes to be read from the FXOS8700CQ
#define FXOS8700CQ_READ_LEN 13 // status plus 6 channels = 13 bytes

/*******************************************************************************
 CALLBACK DECLARATIONS
 ******************************************************************************/
void callback_init(void);
void callback_read(void);
/*******************************************************************************
 GLOBAL VARIABLE DECLARATIONS
 ******************************************************************************/
static bool i2c_finished;
static read_data *r_data;
uint8_t Buffer[FXOS8700CQ_READ_LEN];
/*******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/
I2C_FAIL _mqx_ints_FXOS8700CQ_start(void) {
	i2cInit(I2C_0);
	uint8_t databyte;
	bool trans_ok = i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_WHOAMI,
			&databyte, 1, I2C_READ, callback_init);

	if (trans_ok) {
	}
	while (i2c_finished == false) {
		/*
		 if (i2cStatus() != I2C_NO_FAULT) {
		 return (I2C_ERROR);
		 }*/
	}

	if (databyte != FXOS8700CQ_WHOAMI_VAL) {
		return (I2C_ERROR);
	}
	// write 0000 0000 = 0x00 to accelerometer control register 1 to place FXOS8700CQ into standby
	// [7-1] = 0000 000
	// [0]: active=0
	databyte = 0x00;
	i2c_finished = false;
	i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_CTRL_REG1, &databyte, 1,
			I2C_WRITE, callback_init);
	while (i2c_finished == false)
		;
	// write 0001 1111 = 0x1F to magnetometer control register 1
	// [7]: m_acal=0: auto calibration disabled
	// [6]: m_rst=0: no one-shot magnetic reset
	// [5]: m_ost=0: no one-shot magnetic measurement
	// [4-2]: m_os=111=7: 8x oversampling (for 200Hz) to reduce magnetometer noise
	// [1-0]: m_hms=11=3: select hybrid mode with accelerometer and magnetometer active
	databyte = 0x1F; // = 0x9F;
	i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_M_CTRL_REG1, &databyte, 1,
			I2C_WRITE, callback_init);
	i2c_finished = false;
	while (i2c_finished == false)
		;
	// write 0010 0000 = 0x20 to magnetometer control register 2
	// [7]: reserved
	// [6]: reserved
	// [5]: hyb_autoinc_mode=1 to map the magnetometer registers to follow the
	// accelerometer registers
	// [4]: m_maxmin_dis=0 to retain default min/max latching even though not used
	// [3]: m_maxmin_dis_ths=0
	// [2]: m_maxmin_rst=0
	// [1-0]: m_rst_cnt=00 to enable magnetic reset each cycle
	databyte = 0x20;
	i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_M_CTRL_REG2, &databyte, 1,
			I2C_WRITE, callback_init);
	i2c_finished = false;
	while (i2c_finished == false)
		;

	// write 0000 0001= 0x01 to XYZ_DATA_CFG register
	// [7]: reserved
	// [6]: reserved
	// [5]: reserved
	// [4]: hpf_out=0
	// [3]: reserved
	// [2]: reserved
	// [1-0]: fs=01 for accelerometer range of +/-4g range with 0.488mg/LSB
	databyte = 0x01;
	i2c_finished = false;
	i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_XYZ_DATA_CFG, &databyte, 1,
			I2C_WRITE, callback_init);
	while (i2c_finished == false)
		;
	// write 0000 1101 = 0x0D to accelerometer control register 1
	// [7-6]: aslp_rate=00
	// [5-3]: dr=001 for 200Hz data rate (when in hybrid mode)
	// [2]: lnoise=1 for low noise mode
	// [1]: f_read=0 for normal 16 bit reads
	// [0]: active=1 to take the part out of standby and enable sampling
	databyte = 0x0D;
	i2c_finished = false;
	i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_CTRL_REG1, &databyte, 1,
			I2C_WRITE, callback_init);
	while (i2c_finished == false)
		;
	return (I2C_OK);
}
void ReadAccelMagnData(read_data *data) {
	r_data = data;
	i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_STATUS, Buffer,
	FXOS8700CQ_READ_LEN, I2C_READ, callback_read);
}
/*******************************************************************************
 CALLBACK DEFINITION
 ******************************************************************************/
void callback_init(void) {
	i2c_finished = true;
}

void callback_read(void) {
	if (i2cStatus() == I2C_NO_FAULT) {
		r_data->pAccelData->x = (int16_t) (((Buffer[1] << 8) | Buffer[2])) >> 2;
		r_data->pAccelData->y = (int16_t) (((Buffer[3] << 8) | Buffer[4])) >> 2;
		r_data->pAccelData->z = (int16_t) (((Buffer[5] << 8) | Buffer[6])) >> 2;
		// copy the magnetometer byte data into 16 bit words
		r_data->pMagnData->x = (int16_t) (Buffer[7] << 8) | Buffer[8];
		r_data->pMagnData->y = (int16_t) (Buffer[9] << 8) | Buffer[10];
		r_data->pMagnData->z = (int16_t) (Buffer[11] << 8) | Buffer[12];
		r_data->error = I2C_OK;
		r_data->callback();
	}
}

