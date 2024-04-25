/*
 * menu.h
 *
 * Created: 04.04.2024 16:24:04
 *  Author: msukd
 */ 


#ifndef MENU_H_
#define MENU_H_

#pragma once
#include "UART.h"
#include "LCD.h"
#include "EEPROM.h"

//menu prostrednictvim LCD
void menu(int *data);
uint8_t menu_set(int *data,uint8_t ind);
void menu_teplota (int *data);

//UART menu
void menu_rem(int data);
int rem_set(int *data,uint8_t ind);
void rem_kal_set(int *data,uint8_t ind);
void rem_t_hys(int *data);

#endif /* MENU_H_ */