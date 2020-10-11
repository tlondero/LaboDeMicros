#include "Card.h"
#include "FRDM.h"
#include "placaVerde.h"
#include <iostream>
#include <string>
using namespace std;
void appinit(void);
void apprun(void);
int main(void) {
	appinit();
	cout << "l -> shift left \nr-> shift right \ns -> send char \np-> get PAN \nb -> set brightness \ne-> encoder options"<<endl;
	while (1) {
		apprun();
	}
}


void appinit(void) {
	cardInitDriver(0);
	FRDMInit();
	PVInit();
}
void apprun(void) {
	static char ch;
	static int mod;
	static char s;
	static int Brightness=50;
	static uint8_t* panPointer;

	cin >> s;

	switch (s)
	{
	case 'l':
		PVdispShift(LEFT);
		break;
	case 'r':
		PVdispShift(RIGHT);
		break;
	case 's':
		cout << "You chose send char, select the module" << endl;
		cin >> mod;
		cout << "choose char to send to display " << mod<<endl;
		cin >> ch;
		PVdispSendChar(ch, mod);
		break;
	case 'p':
		panPointer = cardGetPAN();
		cout << "Printing PAN: (";
		for (int i = 0; i < PAN_LENGHT; i++){
			cout << (int)panPointer[i];
			if (i != PAN_LENGHT - 1)
				cout << ", ";
		}
		cout <<")"<< endl;
		break;
	case 'b':
		cout << "Enter number to set brightness" << endl;
		cin >> Brightness;
		PVdispBrightness(Brightness);
		break;
	case 'e':
		cout << "Encoder Events" << endl;
		cout << "p to pop, g to check events" << endl;
		cin >> s;
		if (s == 'p')
			PVencoder_pop_event(1);
		else if (s == 'g')
			PVencoder_is_there_ev(1);
			break;
	default:
		break;
	}



}
