
/***************************************************************************//**
  @file     users.h
  @brief    fsm that controls the system
  @author   MAGT
 ******************************************************************************/
#ifndef _USERS_H_
#define _USERS_H_
 /*******************************************************************************
  * INCLUDE HEADER FILES
  ******************************************************************************/
#include <stdint.h>
#include "stdbool.h"
  /*******************************************************************************
   * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
   ******************************************************************************/

   /*******************************************************************************
	   * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
  ******************************************************************************/
typedef struct {
    uint32_t id;
    uint32_t password;
    bool admin;
    bool available;
    bool blocked;
    uint8_t strikes;
}user_t;
  /*******************************************************************************
		  * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
******************************************************************************/

/*******************************************************************************
* FUNCTION PROTOTYPES WITH GLOBAL SCOPE
******************************************************************************/
/**
* @brief .initUsers: loads the default users stored in memory
*
*/
#define MAX_USERS (15)

void initUsers(void);
/**
* @brief .addUser: Adds the user specefied by the usert structure
* if the user ID already exist the admin overwrites the old password with the new one.
* it also unblocks the user if it was blocked.
* if the user id didnt exist, creates the new user.
*
*/
void addUser(user_t new_user);
/**
* @brief .deleteUser: deletes the user specified.
*/
void deleteUser(uint32_t userID);
/**
* @brief addStrike: Increases the strike counter of the user ID, Three strikes and you're out.
*/
void addStrike(uint32_t userID);
/**
* @brief getStrikes: Increases the strike counter of the user ID, Three strikes and you're out.
*/
uint8_t getStrikes(uint32_t userID);
/**
* @brief clearStrikes: Clears the strikes of all users.
*/
void clearStrikes(void);

/**
* @brief getAdminStatus: returns true if the user ID specified is an admin.
*/
bool getAdminStatus(uint32_t userID);

/**
* @brief getBlockedStatus: returns true if the user ID specified is blocked.
*/
bool getBlockedStatus(uint32_t userID);


/**
* @brief checkExistance: returns true if the user ID specified exist.
*/
bool checkExistance(uint32_t user_ID);
user_t * getUsersList(void);
bool checkPassword(uint32_t user_ID, uint32_t user_pin);
#endif
