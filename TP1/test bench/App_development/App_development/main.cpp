#include <iostream>
#include "drivers/Card.h"
#include "drivers/FRDM.h"
#include "drivers/placaVerde.h"
#include "drivers/Timer.h"
#include "app/fsm.h"
using namespace std;
void appInit(void);
void appRun(void);
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
}
