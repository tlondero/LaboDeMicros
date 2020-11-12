#include "header/i2c.h"

typedef enum {
	SR, ST, DATA
} i2c_states_t;

typedef struct {

	bool finish;
	uint8_t start;
	uint8_t slave;
	uint8_t reg;
	uint8_t *data;
	uint8_t data_size;

	i2c_mode_t mode;
	i2c_states_t state;

} i2c_buffer_t;

callbackPtr callback;
static bool isInit = false;
static I2C_Type *i2cptr;
static i2c_buffer_t buffer;

/*
 #if define(DEBUG)
 #define assert(x)
 if(x) {
 __asm("bkpt #0");
 }
 #else
 #define assert(x) do {} while(0);
 #endif
 */

bool i2cInit(uint8_t chan) {
	if ((chan < I2C_COUNT) && (isInit == false)) {

		I2C_Type *i2cPeriphPtr[] = I2C_BASE_PTRS;
		SIM_Type *sim = SIM;
		uint8_t mux = 5;

		i2cptr = i2cPeriphPtr[chan];

		uint8_t sdaptr;
		uint8_t sdaPinum;

		uint8_t sclptr;
		uint8_t sclPinum;

		//Clk gating
		switch (chan) {
		case I2C_0:
			sim->SCGC4 |= SIM_SCGC4_I2C0_MASK;
			sim->SCGC5 |= SIM_SCGC5_PORTE_MASK;

			sdaptr = PE; //PIN2PORT(PORTNUM2PIN(PE, 25));
			sdaPinum = 25; //PIN2NUM(PORTNUM2PIN(PE, 25));
			sclptr = PE; //PIN2PORT(PORTNUM2PIN(PE, 24));
			sclPinum = 24; //PIN2NUM(PORTNUM2PIN(PE, 24));

			break;
		case I2C_1:
			sim->SCGC4 |= SIM_SCGC4_I2C1_MASK;
			sim->SCGC5 |= SIM_SCGC5_PORTC_MASK;
			mux = 1;

			sdaptr = PC; //PIN2PORT(PORTNUM2PIN(PC, 11));
			sdaPinum = 11; //PIN2NUM(PORTNUM2PIN(PC, 11));
			sclptr = PC; //PIN2PORT(PORTNUM2PIN(PC, 10));
			sclPinum = 10; //PIN2NUM(PORTNUM2PIN(PC, 10));

			break;
		case I2C_2:
			sim->SCGC1 |= SIM_SCGC1_I2C2_MASK;
			sim->SCGC5 |= SIM_SCGC5_PORTA_MASK;

			sdaptr = PA; //PIN2PORT(PORTNUM2PIN(PA, 13));
			sdaPinum = 13; //PIN2NUM(PORTNUM2PIN(PA, 13));
			sclptr = PA; //PIN2PORT(PORTNUM2PIN(PA, 14));
			sclPinum = 14; //PIN2NUM(PORTNUM2PIN(PA, 14));

			break;
		default:
			break;
		}

		PORT_Type *portptr[] = PORT_BASE_PTRS;

		//Set Mux
		(portPointers[sdaptr]->PCR)[sdaPinum] &= ~PORT_PCR_MUX_MASK;
		(portPointers[sdaptr]->PCR)[sdaPinum] |= PORT_PCR_MUX(mux);

		(portPointers[sclptr]->PCR)[sclPinum] &= ~PORT_PCR_MUX_MASK;
		(portPointers[sclptr]->PCR)[sclPinum] |= PORT_PCR_MUX(mux);

		//Disable int
		(portPointers[sdaptr]->PCR)[sdaPinum] &= ~PORT_PCR_IRQC_MASK;
		(portPointers[sdaptr]->PCR)[sdaPinum] |= PORT_PCR_IRQC(
				IRQ_MODE_DISABLE);

		(portPointers[sclptr]->PCR)[sclPinum] &= ~PORT_PCR_IRQC_MASK;
		(portPointers[sclptr]->PCR)[sclPinum] |= PORT_PCR_IRQC(
				IRQ_MODE_DISABLE);

		//Set open drain
		(portPointers[sdaptr]->PCR)[sdaPinum] |= PORT_PCR_ODE_MASK;
		(portPointers[sclptr]->PCR)[sclPinum] |= PORT_PCR_ODE_MASK;

		(portPointers[sdaptr]->PCR)[sdaPinum] |= (HIGH << PORT_PCR_PE_SHIFT);
		(portPointers[sclptr]->PCR)[sclPinum] |= (HIGH << PORT_PCR_PE_SHIFT);

		(portPointers[sdaptr]->PCR)[sdaPinum] |= (HIGH << PORT_PCR_PS_SHIFT);
		(portPointers[sclptr]->PCR)[sclPinum] |= (HIGH << PORT_PCR_PS_SHIFT);

		//baudrate
		i2c->F = I2C_F_ICR(0x35) | I2C_F_MULT(0b10);

		NVIC_EnableIRQ(I2C_IRQS[chan]);

		i2cptr->C1 = 0;
		i2cptr->C1 = I2C_C1_IICIE_MASK | I2C_C1_IICEN_MASK;	//module on
		i2cptr->S = I2C_S_TCF_MASK | I2C_S_IICIF_MASK;			//interrupt on
		i2cptr->FLT |= I2C_FLT_SSIE_MASK;						//startf stopf on

		isInit = true;
	}

	return isInit;
}

bool i2cTransaction(uint8_t slave, uint8_t reg, uint8_t *data, uint8_t dSize, i2c_mode_t mode_, callbackPtr callback_) {

	bool valid = false;

	if (!(i2cptr->S & I2C_S_BUSY_MASK)) {
		buffer.finish = false;
		buffer.mode = mode_;
		buffer.state = SR;
		buffer.start = 0;
		buffer.slave = slave;
		buffer.reg = reg;
		buffer.data = data;
		buffer.data_size = dSize;

		callback = callback_;

		i2cptr->C1 |= I2C_C1_TX_MASK;
		i2cptr->C1 |= I2C_C1_MST_MASK;

		valid = true;
	}

	return valid;
}

void i2cISR_HANDLER(){

}

/*
 void i2cISR_HANDLER() {
 uint32_t flags = I2C->SR;
 assert(flags & ARBL == 0);

 if (flags & RF){
 }

 }
 */
