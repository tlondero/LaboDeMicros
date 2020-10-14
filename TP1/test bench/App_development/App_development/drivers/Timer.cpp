#pragma once
#include "Timer.h"
#include <iostream>
using namespace std;
void timerInit(void) {
	cout << "Timer initialized" << endl;
}

tim_id_t timerGetId(void) {
	static int i = 0;
	cout << "Timer registered " << i << endl;
	return i++;
}

void timerStart(tim_id_t id, ttick_t ticks, uint8_t mode, tim_callback_t callback) {
	cout << "Starting timer " << (int)id << endl;
}

void timerStop(tim_id_t id) {
	cout << "Stoping timer " << (int)id << endl;
}
void timerReset(tim_id_t id) {
	cout << "Restarting timer " << (int)id << endl;
}
