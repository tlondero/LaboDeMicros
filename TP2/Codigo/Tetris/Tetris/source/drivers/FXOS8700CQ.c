/***************************************************************************//**
 @file     FXOS8700CQ.h
 @brief    Driver for thea accelerometer and magnetometer
 @author   MAGT
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <../header/drivers/FXOS8700CQ.h>
#include <../header/drivers/i2c.h>
#include "hardware.h"
#include "header/drivers/timer.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// FXOS8700CQ I2C address
#define FXOS8700CQ_SLAVE_ADDR 0x1D



// FXOS8700CQ internal register addresses
#define FXOS8700CQ_STATUS 		0x00
#define FXOS8700CQ_WHOAMI 		0x0D
#define FXOS8700CQ_XYZ_DATA_CFG 0x0E
#define FXOS8700CQ_CTRL_REG1 	0x2A
#define FXOS8700CQ_M_CTRL_REG1 	0x5B
#define FXOS8700CQ_M_CTRL_REG2 	0x5C
#define FXOS8700CQ_WHOAMI_VAL 	0xC7

// number of bytes to be read from the FXOS8700CQ
#define FXOS8700CQ_READ_LEN 	13 // uno de status, y 6*2 de data accel/magn

#define I2C0_NUM (0)
#define G_CONVERSION	(0.000488)		//G
#define	TESLA_CONVERSION	(0.1)		//uT
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

uint8_t buffer[FXOS8700CQ_READ_LEN]; // read buffer
static bool i2c_done = false;
static bool new_data=false;
static SRAWDATA_f Acc;
static SRAWDATA_f Mag;
/*******************************************************************************
 * FUNCTION DECLARATION WITH LOCAL SCOPE
 ******************************************************************************/

static void FXOS8700CQConfiguration(void);
void callback_init(void);
void callback_read();
/*******************************************************************************
 * FUNCTION DEFINITION WITH LOCAL SCOPE
 ******************************************************************************/

void callback_init(void)
{
	i2c_done = true;
}



void callback_read()
{

	int16_t Acx = (int16_t)(((buffer[1] << 8) | buffer[2])) >> 2;
	int16_t Acy = (int16_t)(((buffer[3] << 8) | buffer[4])) >> 2;
	int16_t Acz = (int16_t)(((buffer[5] << 8) | buffer[6])) >> 2;

	int16_t mgx = (buffer[7] << 8) | buffer[8];
	int16_t mgy = (buffer[9] << 8) | buffer[10];
	int16_t mgz = (buffer[11] << 8) | buffer[12];

	float Ax = Acx*G_CONVERSION;
	float Ay = Acy*G_CONVERSION;
	float Az = Acz*G_CONVERSION;
	float Mx = mgx*TESLA_CONVERSION;
	float My = mgy*TESLA_CONVERSION;
	float Mz = mgz*TESLA_CONVERSION;
	Acc.x=Ax;
	Acc.y=Ay;
	Acc.z=Az;
	Mag.x=Mx;
	Mag.y=My;
	Mag.z=Mz;
	new_data = true;
}

/*******************************************************************************
 * FUNCTION DEFINITION WITH GLOBAL SCOPE
 ******************************************************************************/

void FXOS8700CQInit(void)
{
	i2cInit(I2C0_NUM);
	FXOS8700CQConfiguration();
	timerInit();
	tim_id_t tid = timerGetId();
	timerStart(tid, TIMER_MS2TICKS(200),TIM_MODE_PERIODIC,FXOS8700CQ_ReadAccelMagnData);
}


void FXOS8700CQConfiguration(void)
{

    uint8_t databyte;
    i2c_done = false;

    // read and check the FXOS8700CQ WHOAMI register
    i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_WHOAMI,&databyte, (uint8_t)1, I2C_READ, &callback_init);
    while(!i2c_done);
    i2c_done=false;

    if (databyte != FXOS8700CQ_WHOAMI_VAL)		//no responde bien el acelerómetro
    	return;

     /*write 0000 0000 = 0x00 to accelerometer control register 1 to place FXOS8700CQ into standby
     [7-1] = 0000 000
     [0]: active=0*/
    databyte = 0x00;

    i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_CTRL_REG1, &databyte, 1,	I2C_WRITE, callback_init);
    while(!i2c_done);
    i2c_done=false;

    /* write 0001 1111 = 0x1F to magnetometer control register 1
     [7]: m_acal=0: auto calibration disabled
     [6]: m_rst=0: no one-shot magnetic reset
     [5]: m_ost=0: no one-shot magnetic measurement
     [4-2]: m_os=111=7: 8x oversampling (for 200Hz) to reduce magnetometer noise
     [1-0]: m_hms=11=3: select hybrid mode with accel and magnetometer active*/
    databyte = 0x1F;
    i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_M_CTRL_REG1, &databyte, 1,	I2C_WRITE, callback_init);
    while(!i2c_done);
    i2c_done=false;

     /*write 0010 0000 = 0x20 to magnetometer control register 2
     [5]: hyb_autoinc_mode=1 to map the magnetometer registers to follow the
     accelerometer registers
     [4]: m_maxmin_dis=0 to retain default min/max latching even though not used
     [3]: m_maxmin_dis_ths=0
     [2]: m_maxmin_rst=0
     [1-0]: m_rst_cnt=00 to enable magnetic reset each cycle*/
    databyte = 0x20;

    i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_M_CTRL_REG2, &databyte, 1,	I2C_WRITE, callback_init);
    while(!i2c_done);
    i2c_done=false;

    /* write 0000 0001= 0x01 to XYZ_DATA_CFG register
     [4]: hpf_out=0
     [1-0]: fs=01 for accelerometer range of +/-4g range with 0.488mg/LSB*/
    databyte = 0x01;
    i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_XYZ_DATA_CFG, &databyte, 1,I2C_WRITE, callback_init);
    while(!i2c_done);
    i2c_done=false;

    /* write 0000 1101 = 0x0D to accelerometer control register 1
     [7-6]: aslp_rate=00
     [5-3]: dr=001 for 200Hz data rate (when in hybrid mode)
     [2]: lnoise=1 for low noise mode
     [1]: f_read=0 for normal 16 bit reads
     [0]: active=1 to take the part out of standby and enable sampling*/
    databyte = 0x0D;
    i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_CTRL_REG1, &databyte, 1,I2C_WRITE, callback_init);
    while(!i2c_done);
    i2c_done=false;
}

void FXOS8700CQ_ReadAccelMagnData(void){
	new_data = false;
	i2cTransaction(FXOS8700CQ_SLAVE_ADDR, FXOS8700CQ_STATUS, buffer,FXOS8700CQ_READ_LEN, I2C_READ, callback_read);
}


SRAWDATA_f FXOS8700CQ_getMag(void)
{
	return Mag;
}

SRAWDATA_f FXOS8700CQ_getAcc(void)
{
	return Acc;
}

bool FXOS8700CQ_getDataFlag(void)
{
	return new_data;
}

