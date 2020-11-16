#ifndef PISO_H_
#define PISO_H_

#include<stdint.h>
#include<stdbool.h>

void piso_init (void);
uint8_t piso_transaction(uint8_t * data_ptr, uint8_t size, uint8_t * recieve_ptr);

bool piso_data_sent(void);
uint8_t piso_get_data(uint8_t * dataRecived);

#endif /* PISO_H_ */
