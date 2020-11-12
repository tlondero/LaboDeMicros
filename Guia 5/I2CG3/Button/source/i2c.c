#include "headers/i2c.h"

//ALT5 //ALT1 //ALT5
enum {
	I2C_0, I2C_1, I2C_2, I2C_COUNT
};

static bool isInit = false;
static I2C_Type *i2cptr;

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

			sdaptr = PIN2PORT(PORTNUM2PIN(PE, 25));
			sdaPinum = PIN2PORT(PORTNUM2PIN(PE, 25));
			sclptr = PIN2PORT(PORTNUM2PIN(PE, 24));
			sclPinum = PIN2PORT(PORTNUM2PIN(PE, 24));

			break;
		case I2C_1:
			sim->SCGC4 |= SIM_SCGC4_I2C1_MASK;
			sim->SCGC5 |= SIM_SCGC5_PORTC_MASK;
			mux = 1;

			sdaptr = PIN2PORT(PORTNUM2PIN(PC, 11));
			sdaPinum = PIN2PORT(PORTNUM2PIN(PC, 11));
			sclptr = PIN2PORT(PORTNUM2PIN(PC, 10));
			sclPinum = PIN2PORT(PORTNUM2PIN(PC, 10));

			break;
		case I2C_2:
			sim->SCGC1 |= SIM_SCGC1_I2C2_MASK;
			sim->SCGC5 |= SIM_SCGC5_PORTA_MASK;

			sdaptr = PIN2PORT(PORTNUM2PIN(PA, 13));
			sdaPinum = PIN2PORT(PORTNUM2PIN(PA, 13));
			sclptr = PIN2PORT(PORTNUM2PIN(PA, 14));
			sclPinum = PIN2PORT(PORTNUM2PIN(PA, 14));

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

		i2c->C1 = 0;
		i2c->C1 = I2C_C1_IICIE_MASK | I2C_C1_IICEN_MASK;	//module on
		i2c->S = I2C_S_TCF_MASK | I2C_S_IICIF_MASK;			//interrupt on
		i2c->FLT |= I2C_FLT_SSIE_MASK;						//startf stopf on

		isInit = true;
	}

	return isInit;
}

bool i2cStartTransaction( address, bytesToWrite, writeBuffer, bytesToRead,
		readBuffer) {

}

bool i2cQueryTransaction() {

}

/*
 void i2cISR_HANDLER() {
 uint32_t flags = I2C->SR;
 assert(flags & ARBL == 0);

 if (flags & RF){
 }

 }
 */
