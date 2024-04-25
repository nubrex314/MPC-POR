/*
 * keyboard.h
 *
 * Created: 13.03.2024 12:34:29
 *  Author: msukd
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#pragma once
#include <avr/interrupt.h>//preruseni

char keyboard_read();

void initkeyboard();

#endif /* KEYBOARD_H_ */