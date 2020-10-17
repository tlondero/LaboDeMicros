#ifndef _FEND_T_H_
#define _FEND_T_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	IDDLE_ANIMATION, ASK_PIN_ANIMATION, ACCESS_ANIMATION, OPEN_ANIMATION, USERS_ANIMATION, BRIGHTNESS_ANIMATION, INVALID_ID_ANIMATION, INVALID_PIN_ANIMATION, BLOCKED_ANIMATION,OPEN_SELECTED_ANIMATION, BRIGHTNESS_SELECTED_ANIMATION, USER_SELECTED_ANIMATION
} animation_t;

typedef struct {
	animation_t animation_opt;
	bool animation_en;

	uint8_t* id_data;
	uint8_t id_counter;
	bool id_state;

	uint8_t* pin_data;
	uint8_t pin_counter;
	bool pin_state;

	uint8_t brightness;

	bool blocked_user;
	bool bad_id;
	bool good_id;
	bool bad_pin;
	bool good_pin;
	bool open;
}FEData;
const char BRIGHTNESS_MSG[4] = { 'B','R','G','H' };
const char OPEN_MSG[4] = { 'O','P','E','N' };
const char USER_MSG[4] = { 'U','S','E','R' };

const char PASSWORD_MSG[4] = { 'P','A','S','S' };
const char ADD_USER_MSG[4] = { 'A','D','D','U' };
const char DEL_USER_MSG[4] = { 'D','E','L','U' };
#endif
