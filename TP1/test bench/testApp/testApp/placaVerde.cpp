#include "placaVerde.h"
#include <iostream>
using namespace std;
static char disp[4] = {' ',' ', ' ', ' '};
static int bright = 50;
void PVInit(void) {
	cout << "Placa verde initialized!" << endl;
	PVencoder_init();
	dispInit();
}
void dispInit(void) {
	cout << "Display initialized" << endl;
}

bool PVdispSendChar(char ch, uint8_t seven_seg_module) {
	cout << ch << " sent to display " << (int)seven_seg_module << endl;
	disp[seven_seg_module] = ch;
	cout << (char)disp[0] << (char)disp[1] << (char)disp[2] << (char)disp[3] << endl;
	return true;
}

void PVdispShift(direction_t direction) {
	if (direction == RIGHT) {
		cout << "Message shifted to the Right" << endl;
		disp[3] = disp[2];
		disp[2] = disp[1];
		disp[1] = disp[0];
		disp[0] = ' ';
		cout << (char)disp[0] << (char)disp[1] << (char)disp[2] << (char)disp[3] << endl;
	}
	else if(direction == LEFT) {
		cout << "Message shifted to the Left" << endl;
		disp[0] = disp[1];
		disp[1] = disp[2];
		disp[2] = disp[3];
		disp[3] = ' ';
		cout << (char)disp[0] << (char)disp[1] << (char)disp[2] << (char)disp[3] << endl;
	}
}
void PVdispClear(void) {
	cout << "Display cleared" << endl;
	disp[0] = ' ';
	disp[1] = ' ';
	disp[2] = ' ';
	disp[3] = ' ';
	cout << (char) disp[0] << (char)disp[1] << (char)disp[2] << (char)disp[3] << endl;
}
void PVdispBrightness(uint8_t brightness) {
	bright = brightness;
	cout << "Brightness set to " << bright << endl;
}

void PVencoder_init(void) {
	cout << "Encoder initalized" << endl;
}

encoder_id PVencoder_register(void) {
	static int id = 0;
	cout << "Enconder registered as id " << id << endl;
	return id++;
}

int8_t PVencoder_pop_event(encoder_id id) {
	static unsigned int i = 0;
	int8_t evs[10] = { 0,1,2,3,4,5,6,7,8,9 };
	//aca deber�as poner los eventos que quer�s recibir;
	cout << "Popping Event " << evs[i%10] <<endl;
	return  evs[(++i) % 10];
}

bool PVencoder_is_there_ev(encoder_id id) {
	cout << "Entered ;is there ev; returning true" << endl;
	return true;
}

