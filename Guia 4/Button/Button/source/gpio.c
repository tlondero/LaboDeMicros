/*
 * gpio.c
 *
 *  Created on: 31 ago. 2020
 *      Author: MAGT
 */


#include "header/SysTick.h"
#include "header/board.h"
#include "MK64F12.h"
#include "hardware.h"
//Clock gating masks
const static uint32_t SIM_SCGC5_PORT_MASKS[] = {SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK };

//Port arrays
const static PORT_Type * PORT_SELECTORS[] = {PORTA, PORTB, PORTC, PORTD, PORTE};

//GPIO arrays
const static GPIO_Type * GPIO_SELECTORS[] = {PTA, PTB, PTC, PTD, PTE}; //GPIO_BASE_PTRS

//Matriz de callbacks de cada puerto, cada pin
pinIrqFun_t port_pin_interrupt[5][32];
uint32_t pin_irq_configured[5][32];

static void interrupt_init(uint32_t IRQn);

static void interrupt_init(uint32_t IRQn){

	if ( !(NVIC_GetEnableIRQ(IRQn)) ){  //Si el puerto no tiene las IRQ habilitadas
			NVIC_EnableIRQ(IRQn); 					 //las habilito
		}
}

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
		gpio_pointer->PDDR &= ~(1<<number);
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


bool gpioIRQ (pin_t pin, uint8_t irqMode, pinIrqFun_t irqFun){

	uint32_t port = PIN2PORT(pin);      //Tomo el puerto
	uint32_t number = PIN2NUM(pin);		//Tomo el numero
	PORT_Type*  port_pointer = PORT_SELECTORS[port];
	uint32_t IRQn = PORTA_IRQn + port;
	
	//Identifico a este pin como que tiene la interrupcion configurada
	if(irqMode == GPIO_IRQ_MODE_DISABLE){
		pin_irq_configured[port][number] = 0;
	}
	else{
		pin_irq_configured[port][number] = 1;
	}
	port_pin_interrupt[port][number] = irqFun;  //Me guardo el callback de ese pin
	
	port_pointer->PCR[number] &= ~PORT_PCR_IRQC_MASK;
	port_pointer->PCR[number] |= PORT_PCR_IRQC(irqMode); //Seteo el tipo de interrupcion para ese pin
	
	///PENSAR: Esto de habilitar las irq del puerto en el nvic se tendria que hacer apenas se configura un pin
	///como hice aca o deberia de ir en app_init despues de configurar TODOS los pines? (que pasa si por ya haberlo
	///habilitado me viene una interrupcion de un pin que aun no le configure el PCR? o empieza por default el PCR
	///de cada pin de manera tal que no puede tomar interrupciones?)
	
	interrupt_init(IRQn);

}



///PENSAR: Que hacer con estos? Son los handlers de IRQ por puerto, pero yo tengo un callback por cada pin de cada puerto?
__ISR__ PORTA_IRQHandler(void){
	uint32_t i = 0;
	uint32_t exit_cond = 0;
	while(!exit_cond){
		if(pin_irq_configured[PA][i] == 1){
			if((PORTA->PCR[i] & PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK){
				PORTA->PCR[i] |= PORT_PCR_ISF_MASK;
				port_pin_interrupt[PA][i]();
				exit_cond = 1;
			}
		}
		i++;
		if(i == 32){
			exit_cond = 1;
		}
	}
}
__ISR__ PORTB_IRQHandler(void){
	uint32_t i = 0;
	uint32_t exit_cond = 0;
	while(!exit_cond){
		if(pin_irq_configured[PB][i] == 1){

			if((PORTB->PCR[i] & PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK){
				PORTB->PCR[i] |= PORT_PCR_ISF_MASK;
				port_pin_interrupt[PB][i]();
				exit_cond = 1;
			}
		}
		i++;
		if(i == 32){
			exit_cond = 1;
		}
	}
}
__ISR__ PORTC_IRQHandler(void){
	uint32_t i = 0;
	uint32_t exit_cond = 0;
	while(!exit_cond){
		if(pin_irq_configured[PC][i] == 1){
			if((PORTC->PCR[i] & PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK){
				PORTC->PCR[i] |= PORT_PCR_ISF_MASK;
				port_pin_interrupt[PC][i]();
				exit_cond = 1;
			}
		}
		i++;
		if(i == 32){
			exit_cond = 1;
		}
	}
}
__ISR__ PORTD_IRQHandler(void){
	uint32_t i = 0;
	uint32_t exit_cond = 0;
	while(!exit_cond){
		if(pin_irq_configured[PD][i] == 1){
			if((PORTD->PCR[i] & PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK){
				PORTD->PCR[i] |= PORT_PCR_ISF_MASK;
				port_pin_interrupt[PD][i]();
				exit_cond = 1;
			}
		}
		i++;
		if(i == 32){
			exit_cond = 1;
		}
	}
}
__ISR__ PORTE_IRQHandler(void){
	uint32_t i = 0;
	uint32_t exit_cond = 0;
	while(!exit_cond){
		if(pin_irq_configured[PE][i] == 1){
			if((PORTE->PCR[i] & PORT_PCR_ISF_MASK) == PORT_PCR_ISF_MASK){
				PORTE->PCR[i] |= PORT_PCR_ISF_MASK;
				port_pin_interrupt[PE][i]();
				exit_cond = 1;
			}
		}
		i++;
		if(i == 32){
			exit_cond = 1;
		}
	}
}

void gpioToggle (pin_t pin){
	uint32_t port = PIN2PORT(pin);
	uint32_t number = PIN2NUM(pin);
	GPIO_Type* gpio_pointer = GPIO_SELECTORS[port];
	gpio_pointer->PTOR |= (1<<number);
}

bool gpioRead (pin_t pin){
	uint32_t port = PIN2PORT(pin);
	uint32_t number = PIN2NUM(pin);
	GPIO_Type* gpio_pointer = GPIO_SELECTORS[port];
	bool res =  gpio_pointer->PDIR & (1<<number);
	return res;

}
