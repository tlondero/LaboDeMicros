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
static state st;
int main(int argc, char** argv) {
	appInit();
	while (1) {
		appRun();
		
	}
	return 0;
}


void appInit(void) {
	timerInit();
	FRDMInit();
	dispInit();
	cardInitDriver(0);
	PVencoder_init();
	st=FSMInitState();
}
void appRun(void) {
	
	st = FSMRun(st);
	FEData  mydata = *(FSMGetFEData());

	cout << st << endl;
	printData(mydata);
}



void printData(FEData data) {
	cout << "Data:\nAnimation Enable:  " <<(int) data.animation_en << "\nAnimation Option:  " << (int)data.animation_opt << endl;
	cout << "Bad id: " << (int)data.bad_id << "\nBad pin " << (int)data.bad_pin << "\nBlocked user:  " << (int)data.blocked_user << endl;
	cout << "Brightness: " << (int)data.brightness << "\nGood ID: " << (int)data.good_id << "\nGood pin " << (int)data.good_pin << endl;
	cout<< "Id counter: " << (int)data.id_counter << "\nId state: " << (int)data.id_state << "\nOpen: " << (int)data.open << endl;
	cout << "\npin counter: " << (int)data.pin_counter  << "\npin state: " << (int)data.pin_state;
}