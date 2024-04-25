/*
 * LCD.h
 *
 * Created: 06.03.2024 12:30:56
 *  Author: msukd
 */ 


#ifndef LCD_H_
#define LCD_H_


#pragma once
#include <avr/interrupt.h>//preruseni
#include <stdio.h>
#include <avr/io.h>
#include <avr/cpufunc.h>
#include "timer.h"
#include "ADC.h"
#include "helpf.h"

void initLCD();

void LCD_send(uint8_t ms,uint8_t rs);

void LCD_send_4bit(uint8_t bit);

void lcd_print_char(char c);

int LCD_putchar(char c, FILE *stream);

void LCD_clear_dis(void);
void LCD_clear_row(uint8_t row);
void LCD_display_shift(uint8_t direction) ;
void LCD_cursor_shift(uint8_t direction) ;
void LCD_display_on_off(uint8_t display, uint8_t cursor, uint8_t blink);
void LCD_set_cursor(uint8_t row, uint8_t col);
void LCD_rotate_display(uint8_t steps,uint8_t direction) ;

uint16_t LCD_button(void);
int LCD_Button_adc_value(int val_adc);
#endif /* LCD_H_ */