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

event_t PVencoder_pop_event(encoder_id id) {
	static unsigned int i = 0;
	int8_t evs[6] = {RIGHT_TURN, BUTTON_PRESS};

	
	//int8_t evs[20] = { BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,RIGHT_TURN,RIGHT_TURN,BUTTON_PRESS
	//,BUTTON_PRESS,RIGHT_TURN,BUTTON_PRESS,RIGHT_TURN,BUTTON_PRESS,RIGHT_TURN,BUTTON_PRESS,RIGHT_TURN,BUTTON_PRESS,RIGHT_TURN };
	//EVENTOS ID BIEN CONTRASEÑA BIEN

	//	int8_t evs[20] = { BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,BUTTON_PRESS,RIGHT_TURN,RIGHT_TURN,BUTTON_PRESS
	//,RIGHT_TURN,RIGHT_TURN,BUTTON_PRESS,RIGHT_TURN,BUTTON_PRESS,RIGHT_TURN,BUTTON_PRESS,RIGHT_TURN,BUTTON_PRESS,RIGHT_TURN };
	//ID bien PIN mal

//aca deberías poner los eventos que querés recibir;
	cout << "Popping Event " <<(int) evs[i%2] <<endl;
	return  evs[(i++) % 2];
}

bool PVencoder_event_avb(encoder_id id) {
	//cout << "Entered Encoder is there ev: returning false" << endl;
	//return false;
	cout << "Entered ;is there ev; returning true" << endl;
	return true;
}

void PVencoder_update(void ){
}

void PVencoder_add_new_event(encoder_id id, event_t ev) {
}
//void PVAnimation(uint8_t animation,bool activate) {
//	if(activate)
//		cout << "Animation"<< (int) animation << " was activated" << endl;
//	else
//		cout << "Animation" << (int)animation << " was desactivated" << endl;
//}

void PVIncreaseBrightness(void) {
	cout << "Brightness increased" << endl;
}

void PVDecreaseBrightness(void) {
	cout << "Brightness decreased" << endl;
}