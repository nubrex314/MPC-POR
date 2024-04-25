/*
 * helpf.h
 *
 * Created: 15.03.2024 16:31:00
 *  Author: msukd
 */ 

#ifndef HELPF_H_
#define HELPF_H_

#pragma once
#include <stdio.h>
#include <avr/io.h>
#include "ADC.h"
#include "keyboard.h"
#include "LCD.h"
#include "timer.h"
#include "UART.h"
#include "Rele.h"
#include "TWI.h"
#include "EEPROM.h"

void initglobal(void);

void string_clear(char *string);

float int_to_float(uint8_t Z,uint8_t des);

void printfloat(float in);

void print_binary(uint16_t num);

uint16_t ascii_to_number(uint16_t a);

void play_tone(uint16_t frequency, uint16_t duration_ms) ;

void play_super_mario_theme() ;

void play_morse(uint8_t morse);
void numberToMorse(int number);


#endif /* HELPF_H_ */