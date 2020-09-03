/*
 * gpio.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */


#include "gpio.h"
#include "MK64F12.h"

//Clock gating masks
const static uint32_t SIM_SCGC5_PORT_MASKS[] = {SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK };

//Port arrays
const static PORT_Type * PORT_SELECTORS[] = {PORTA, PORTB, PORTC, PORTD, PORTE};

//GPIO arrays
const static GPIO_Type * GPIO_SELECTORS[] = {PTA, PTB, PTC, PTD, PTE}; //GPIO_BASE_PTRS

//linea 10460

void gpioCG(pin_t pin, uint8_t state){

	//Enable Clock Gating
	if (state == CG_ON)
	{
		SIM->SCGC5 |= SIM_SCGC5_PORT_MASKS[PIN2PORT(pin)];
	}
	//Disable Clock Gating
	else if (state == CG_OFF)
	{
		SIM->SCGC5 &= ~SIM_SCGC5_PORT_MASKS[PIN2PORT(pin)];
	}
}

/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void gpioMode (pin_t pin, uint8_t mode){

	//Enable Port Clock Gating
	gpioCG(pin, CG_ON);

	//PCR CONFIG
	uint8_t port = PIN2PORT(pin);
	uint32_t number = PIN2NUM(pin);
	PORT_Type*  port_pointer = PORT_SELECTORS[port];
	GPIO_Type* gpio_pointer = GPIO_SELECTORS[port];

	port_pointer->PCR[number] = 0x00;
	// Establecemos el pin como GPIO
	port_pointer->PCR[number] |= PORT_PCR_MUX(PORT_mGPIO);
	port_pointer->PCR[number] |= PORT_PCR_IRQC(0);

	if (mode == OUTPUT){
		gpio_pointer->PDDR |= (1<<number);
	}
	else{
		// Set pin as INPUT
		gpio_pointer->PDDR |= (1<<number);

		switch (mode) {
			case INPUT_PULLDOWN:
				port_pointer->PCR[number] |= PORT_PCR_PE(number); // PULL ENABLE
				port_pointer->PCR[number] |= PORT_PCR_PS(0);	//SET PULL DOWN
				break;
			case INPUT_PULLUP:
				port_pointer->PCR[number] |= PORT_PCR_PE(number); // PULL ENABLE
				port_pointer->PCR[number] |= PORT_PCR_PS(1); //SET PULL DOWN
				break;
			default:
				break;
		}
	}
}

/**
 * @brief Write a HIGH or a LOW value to a digital pin
 * @param pin the pin to write (according PORTNUM2PIN)
 * @param val Desired value (HIGH or LOW)
 */
void gpioWrite (pin_t pin, bool value){
	uint8_t port = PIN2PORT(pin);
	uint32_t number = PIN2NUM(pin);
	PORT_Type*  port_pointer = PORT_SELECTORS[port];
	GPIO_Type* gpio_pointer = GPIO_SELECTORS[port];

	if (value == HIGH){
		gpio_pointer->PDOR |= (1<<number);
	}
	else if(value == LOW){
		gpio_pointer->PDOR &= ~(1<<number);
	}


}


















