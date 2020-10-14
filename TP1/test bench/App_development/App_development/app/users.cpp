
/***************************************************************************//**
  @file     users.c
  @brief    
  @author   MAGT
 ******************************************************************************/
#pragma once
 /*******************************************************************************
  * INCLUDE HEADER FILES
  ******************************************************************************/
#include "users.h"
  /*******************************************************************************
   * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
   ******************************************************************************/
#define MAX_USERS (15)
#define DEFAULT_USERS (5)
   /*******************************************************************************
    * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
    ******************************************************************************/
//Usuarios default:
user_t defaults[DEFAULT_USERS] = { {15,12345,true, true,false,0},{1,01234,false, true,false,0},{2,01234,false, true,false,0},{3,01234,false, true,false,0},{4,01234,false, true,false,0} };
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
static user_t users[MAX_USERS] = { 0 };
static uint8_t user_counter;
 /*******************************************************************************
  * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
  ******************************************************************************/
void initUsers(void) {
    for (user_counter = 0; user_counter < DEFAULT_USERS; user_counter++) {
        users[user_counter] = defaults[user_counter];
    }
}
void addUser(user_t new_user) {
    //tengo que ver si existe ese id y si tengo espacio en el buffer, si existe lo que hace es cambiarle la password, si no tengo espacio no hago nada
    uint8_t i = 0;
    while ((new_user.id != users[i].id) && (i < MAX_USERS) && (users[i].available)) {
        i++;
    }
    if ((new_user.id == users[i].id) || (users[i].available == false)) {
        users[i] = new_user;
        //si lo encontré le cambio la contraseña
        // y lo desbloquea si estaba bloqueado
        //por otor lado Si nunca fue ocupado se registra un nuevo usuario
    }
}
void deleteUser(uint32_t userID) {
    uint8_t i = 0;
    while ((userID != users[i].id) && (i < MAX_USERS) && (users[i].available)) {
        i++;
    }
    if (users[i].id == userID) {
        users[i].available = false;
    }
}
void addStrike(uint32_t userID) {
    uint8_t i=0;
    while ((userID != users[i].id) && (i < MAX_USERS) && (users[i].available)) {
        i++;
    }
    if (users[i].id == userID) {
        users[i].strikes += 1;
    }
    if (users[i].strikes == 3) {
        users[i].blocked = true;
    }
}
void clearStrikes(void) {
    int8_t i;
    for (i = 0; i < MAX_USERS; i++) {
        users[i].strikes = 0;
    }
}
bool getAdminStatus(uint32_t userID) {
    bool ret=false;
    uint8_t i = 0;
    while ((userID != users[i].id) && (i < MAX_USERS) && (users[i].available)) {
        i++;
    }
    if (users[i].id == userID) {
        ret= users[i].admin;
    }
    return ret;
}
bool getBlockedStatus(uint32_t userID) {
    bool ret = false;
    uint8_t i = 0;
    while ((userID != users[i].id) && (i < MAX_USERS) && (users[i].available)) {
        i++;
    }
    if (users[i].id == userID) {
        ret = users[i].blocked;
    }
    return ret;
}
