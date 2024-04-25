/*
 * Rele.c
 *
 * Created: 01.04.2024 16:39:36
 *  Author: msukd
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/cpufunc.h>
#include "Rele.h"

// Definice pinù pro relé
#define RELAY1_PIN PORTC3
#define RELAY2_PIN PORTC2

uint8_t chladim=0;
uint8_t topim=0;

// Funkce pro inicializaci pinù relé
void initrelay() 
{
	// Nastavíme piny jako výstupy
	DDRC = (1 << RELAY1_PIN) | (1 << RELAY2_PIN);
	relay1_off();
	relay2_off();
}

// Funkce pro zapnutí relé 1
void relay1_on() 
{
	PORTC |= (1 << RELAY1_PIN);
	topim=1;
}

// Funkce pro vypnutí relé 1
void relay1_off() 
{
	PORTC &= ~(1 << RELAY1_PIN);
		topim=0;
}

// Funkce pro zapnutí relé 2
void relay2_on() 
{
	PORTC |= (1 << RELAY2_PIN);
	chladim=1;
}

// Funkce pro vypnutí relé 2
void relay2_off()
 {
	PORTC &= ~(1 << RELAY2_PIN);
	chladim=0;
}

//Funkce prepinani dle aktualni teploty a hystereze
void reley_temostat(int *data, float tepl)
{
	float T1=int_to_float(data[10],data[11]);
	float T2=int_to_float(data[12],data[13]);
	float hystereze=int_to_float(data[6],data[7]);
	if(topim==0&&chladim==0)
	{
		if(tepl>T1)
		{
			relay1_on();
		}
		else if (tepl<T2)
		{
			relay2_on();
		}
	}else if (topim==1)
	{
		if(tepl<(T1-hystereze))
		{
			relay1_off();
		}
	}else if (chladim==1)
	{
		if(tepl>(T2+hystereze))
		{
			relay2_off();
		}
	}
}