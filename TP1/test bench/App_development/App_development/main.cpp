#include <iostream>
#include "drivers/Card.h"
#include "drivers/FRDM.h"
#include "drivers/placaVerde.h"
#include "fsm.h"
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
	FRDMInit();
	dispInit();
	cardInitDriver(0);
	st=FSMInitState();
}
void appRun(void) {
	st = FSMRun(st);
}
