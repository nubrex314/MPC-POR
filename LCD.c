/*
 * LCD.c
 *
 * Created: 06.03.2024 12:30:45
 *  Author: msukd
 */ 

#include "LCD.h"

#define LCD_RS PB0
#define LCD_E  PB1
#define LCD_D4 PD4
#define LCD_D5 PD5
#define LCD_D6 PD6
#define LCD_D7 PD7

// Definice enum pro tlaèítka
typedef enum {
	SELECT,
	LEFT,
	DOWN,
	UP,
	RIGHT,
	RST,
	NONE
} Button;

int LCD_Button_adc_value(int val_adc) {
	if(val_adc>1500)
	{
		//printf("NONE   ");
		return NONE;
	}
	else if(val_adc<70)
	{
		//printf("RIGHT");
		play_tone(300,50);
		return RIGHT;
	}
	else if(val_adc<195)
	{
		//printf("UP      ");
		play_tone(400,50);
		return UP;
	}
	else if(val_adc<380)
	{
		//printf("DOWN   ");
				play_tone(500,50);
		return DOWN;
	}
	else if(val_adc<500)
	{
		//printf("LEFT  ");
				play_tone(600,50);
		return LEFT;
	}
	else if(val_adc<700)
	{
		//printf("SELECT   ");
				play_tone(700,50);
		return SELECT;
	}
	//printf("NONE   ");
	return NONE;

}

void initLCD()
{
	//init reset
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	DDRB |= (1 << DDB0) | (1 << DDB1);
	delay(40000);
	LCD_send_4bit(0x03);
	delay(5000);
	LCD_send_4bit(0x03);
	delay(150);
	LCD_send_4bit(0x03);
	delay(5000);
	//init display
	LCD_send_4bit(0x02);
	delay(5000);
	LCD_send(0x28,0); // Function set: 4-bit/2-line
	delay(5000);
	LCD_send(0x0C,0); // Display ON; Cursor off
	delay(5000);
	LCD_clear_dis(); // Clear display
	delay(5000);
	LCD_send(0x06,0); // Entry mode: move cursor right
}

void LCD_send(uint8_t ms,uint8_t rs)
{
	if (rs) {
		PORTB |= (1 << LCD_RS);
	}
	else {
		PORTB &= ~(1 << LCD_RS);
	}
		LCD_send_4bit(ms >> 4);
		LCD_send_4bit(ms & 0x0F);
}

void LCD_send_4bit(uint8_t bit)
{
	    PORTD = (PORTD & 0x0F) | (bit << 4);
	    PORTB |= (1 << LCD_E);
	    delay(10);
	    PORTB &= ~(1 << LCD_E);
	    delay(100);
}


void LCD_print_char(char c)
{
	LCD_send(c,1);
}

int LCD_putchar(char c, FILE *stream)
{
	if (c == '\n'){
		LCD_putchar('\r', stream);
	}
	
	LCD_print_char(c);
	return 0;
}


void LCD_clear_dis(void)
{
	LCD_send(0x01,0);
}

void LCD_clear_row(uint8_t row)
{
	for(int i=0;i<40;i++)
	{
		LCD_set_cursor(row,i);
		LCD_print_char(' ');
	}
	
}

void LCD_display_on_off(uint8_t display, uint8_t cursor, uint8_t blink)
 {
	uint8_t command = 0x08;
	if (display) {
		command |= 0x04;
	}
	if (cursor) {
		command |= 0x02;
	}
	if (blink) {
		command |= 0x01;
	}
	LCD_send(command, 0);
	delay(100);
}

void LCD_cursor_shift(uint8_t direction) 
{
	uint8_t command = 0x10;
	if (direction) {
		command |= 0x04;
	}
	LCD_send(command, 0);
	delay(100);
}

void LCD_display_shift(uint8_t direction) 
{
	uint8_t command = 0x18;
	if (direction) {
		command |= 0x04;
	}
	LCD_send(command, 0);
	delay(100);
}

void LCD_set_cursor(uint8_t row, uint8_t col) 
{
	uint8_t address;

	if (row == 0) {
		address = 0x80;
		} else {
		address = 0xC0;
	}
	address += col;

	LCD_send(address, 0);
}

void LCD_rotate_display(uint8_t steps,uint8_t direction) 
{

    for (int i = 0; i < steps; i++) {
        LCD_display_shift(direction); 
        delay(50000); // zpozdeni mezi kroky
    }
}

uint16_t LCD_button(void)
{
	uint16_t val_adc=ADC_read(0);
	return val_adc;
}

