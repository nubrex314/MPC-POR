/*
 * keyboard.c
 *
 * Created: 13.03.2024 12:34:15
 *  Author: msukd
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/cpufunc.h>
#include "keyboard.h"
#include "timer.h"

// Definice pinù pro øádky a sloupce
#define ROWS 4
#define COLS 3

char keys[ROWS][COLS] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}
};


// Funkce pro inicializaci klávesnice
void initkeyboard()
{
	// Nastavení øádkù jako výstupy s pull-up odpory
	DDRD |= (1 << PORTD2) | (1 << PORTD3);
	PORTD |= (1 << PORTD2) | (1 << PORTD3);

	DDRB |= (1 << PORTB3) | (1 << PORTB4);
	PORTB |= (1 << PORTB3) | (1 << PORTB4);
	// Nastavení sloupcù jako vstupy s pull-up odpory
	DDRC &= ~((1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3));
	PORTC |= (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);
}

// Funkce pro ètení stisknuté klávesy
char keyboard_read()
 {
	for (int row = 0; row < ROWS; row++) {
		// Aktivace øádku
		if (row<2)
		{
			PORTD &= ~(1 << (PORTD2 + row));
		}else
		{
			PORTB &= ~(1 << (PORTB3 + row-2));
		}
		
		delay(1000); // Krátké zpoždìní pro stabilitu

		for (int col = 1; col < COLS+1; col++) 
		{
			// Ètení sloupce
			if (!(PINC & (1 << col))) {
				// Klávesa stisknuta
				while(!(PINC&(1<<col)))
				{
					delay(1000);
				}
					
				return keys[row][col];
			}
		}

		if (row<2)
		{
			PORTD |= (1 << (PORTD2 + row));
		}else
		{
			PORTB |= (1 << (PORTB3 + row-2));
		}
		}

	return '\0'; // Žádná klávesa stisknuta
}