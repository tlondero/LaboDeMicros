#include "Card.h"
using namespace std;
#include <iostream>
uint8_t* cardGetPAN(void) {
	static uint8_t PAN[PAN_LENGHT] = 
	//{0,0,0,0,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1};//ID VALIDA
	{0,0,0,0,0,5,5,5,1,1,1,1,1,1,1,1,1,1,1};//ID INVALIDA
	//cout << "Returning PAN " << endl;
	//return &PAN[0];
	cout << "Entered get PAN, returning NULL" << endl;
	return 0;
}

void cardInitDriver(void (*fun_callback)(void)) {
	cout << "Card initialized" << endl;
}