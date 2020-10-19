#include <iostream>
#include "drivers/Card.h"
#include "drivers/FRDM.h"
#include "drivers/placaVerde.h"
#include "drivers/Timer.h"
#include "app/fsm.h"
using namespace std;
void appInit(void);
void appRun(void);
void printData(FEData data);
uint32_t power(uint32_t num, uint32_t exp);
static state st;
int main(int argc, char** argv) {
	appInit();
	while (1) {
		appRun();
		
	}
	return 0;
}


void appInit(void) {
	//timerInit();
	//FRDMInit();
	//dispInit();
	//cardInitDriver(0);
	//PVencoder_init();
	//st=FSMInitState();
	uint8_t ASCII_NUM_BASE = 48;
	uint8_t DIGIT_MAX_LEN = 8;
	uint32_t num = 99999999;
	int8_t i;
	int8_t j;
	uint32_t debug;
	uint32_t id[8];
	char chars[8];
	for (i = DIGIT_MAX_LEN; i > 0 ; i--) {
		uint32_t aux = 0;
		if (i == DIGIT_MAX_LEN) {
			id[DIGIT_MAX_LEN - i] = (num / power(10, i - 1));
		}
		else {
			for (j = i; j < DIGIT_MAX_LEN; j++) {
				aux += id[j-i] * power(10, DIGIT_MAX_LEN - j);
			}
			id[DIGIT_MAX_LEN - i] = (num / power(10, i - 1)) - aux;
		}
		chars[DIGIT_MAX_LEN - i] = id[DIGIT_MAX_LEN - i] + ASCII_NUM_BASE;
		cout << chars[DIGIT_MAX_LEN - i];
	}
}
void appRun(void) {
	
	//st = FSMRun(st);
	//FEData  mydata = *(FSMGetFEData());

	//cout << st << endl;
	//printData(mydata);
}

uint32_t power(uint32_t num, uint32_t exp) {
	uint32_t ret = 1;
	uint8_t i = 0;
	if (exp == 0) {
		return 1;
	}
	else if (num == 0) {
		return 0;
	}
	else {
		for (i = 0; i < exp; i++) {
			ret *= num;
		}
		return ret;
	}
}


void printData(FEData data) {
	cout << "Data:\nAnimation Enable:  " <<(int) data.animation_en << "\nAnimation Option:  " << (int)data.animation_opt << endl;
	cout << "Bad id: " << (int)data.bad_id << "\nBad pin " << (int)data.bad_pin << "\nBlocked user:  " << (int)data.blocked_user << endl;
	cout << "Brightness: " << (int)data.brightness << "\nGood ID: " << (int)data.good_id << "\nGood pin " << (int)data.good_pin << endl;
	cout<< "Id counter: " << (int)data.id_counter << "\nId state: " << (int)data.id_state << "\nOpen: " << (int)data.open << endl;
	cout << "\npin counter: " << (int)data.pin_counter  << "\npin state: " << (int)data.pin_state;
}