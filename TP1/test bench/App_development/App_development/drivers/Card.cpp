#include "Card.h"
using namespace std;
#include <iostream>
uint8_t* cardGetPAN(void) {
	static uint8_t PAN[PAN_LENGHT] = 
	{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	cout << "Returning PAN " << endl;
	return &PAN[0];
}

void cardInitDriver(void (*fun_callback)(void)) {
	cout << "Card initialized" << endl;
}