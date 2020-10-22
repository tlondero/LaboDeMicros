#ifndef _FEND_T_H_
#define _FEND_T_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	IDDLE_ANIMATION, ASK_PIN_ANIMATION, ACCESS_ANIMATION, OPEN_ANIMATION, USERS_ANIMATION, BRIGHTNESS_ANIMATION, INVALID_ID_ANIMATION, INVALID_PIN_ANIMATION, BLOCKED_ANIMATION,OPEN_SELECTED_ANIMATION, BRIGHTNESS_SELECTED_ANIMATION, USER_SELECTED_ANIMATION, CLAVE_SELECTED_ANIMATION, ADD_SELECTED_ANIMATION, DEL_SELECTED_ANIMATION
} animation_t;

typedef struct {

	animation_t animation_opt; // Animation option selected
	bool animation_en; // enables the animation

	uint8_t* id_data; // pointer ti the ID data buffer
	uint8_t id_counter; // indicates the number of numbers entered by the user

	uint8_t* pin_data;
	uint8_t pin_counter;


	uint8_t brightness;
	bool br; //Controla que se cambie el brillo si y solo si hubo un cambio por parte del usuario

	bool blocked_user; // Indicates if an user was blocked
	bool bad_id; //bad id entered
	bool good_id; // id was found
	bool bad_pin;// pin wasnt correct
	bool good_pin;//pin was correct
	bool open; // represents if the door is being open
	bool del_user; //



	uint8_t del_i;  // indicates change in the delete menu
	void* del_user_ptr; //pointer to the user_t data from the BE
}FEData;

#endif
