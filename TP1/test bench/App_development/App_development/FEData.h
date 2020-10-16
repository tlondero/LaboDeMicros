#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	IDDLE_ANIMATION, ASK_PIN_ANIMATION, ACCESS_ANIMATION, OPEN_ANIMATION, USERS_ANIMATION, BRIGHTNESS_ANIMATION, INVALID_ID_ANIMATION, INVALID_PIN_ANIMATION, BLOCKED_ANIMATION
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