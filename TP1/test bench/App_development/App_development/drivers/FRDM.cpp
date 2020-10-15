#include <iostream>
#include "FRDM.h"
using namespace std;
void FRDMInit(void) {
	cout << "FRDM initialized" << endl;
	static int id_led = led_init_led(1, 2);
	static int id_button = ButtonInit(1, 2);
}

int8_t ButtonInit(pin_t pin, uint8_t mode) {
	static int8_t id = 0;
	cout << "New Button initialized, id " <<(int) id << endl;
	return id++;
}

void ButtonSetIRQ(int8_t id, uint8_t IRQ_mode, pinIrqFun_t fcallback) {
	cout << "IRQ set to button " << (int)id << endl;;
}

const ButtonEvent* ButtonGetEvent(int8_t id) {
	static ButtonEvent evs[5] = {EOQ};
	cout << "Event returned " << endl;
	return &evs[0];
}

int8_t led_init_led(pin_t pin, uint8_t pin_mode) {
	static int8_t id = 0;
	cout << "New Led initialized, id " << (int)id << endl;;
	return id++;
}

void led_destroy_led(int8_t led_id) {
	cout << "Led " << led_id << " destroyed" << endl;
}

void led_configure(void) {
	cout << "Led configured" << endl;
}
void led_set_state(int8_t led_id, uint8_t norm_state) {
	cout << "Led " << led_id << " set to state " << norm_state << endl;
}
void led_flash(int8_t led_id) {
	cout << "Led " << (int) led_id << " is flashing now" << endl;
}
void led_poll(void) {
	cout << "Polling Leds" << endl;
}

bool FRDMButtonIRQ(uint8_t button, uint8_t IRQ_mode, pinIrqFun_t fcallback) {
	return true;
}