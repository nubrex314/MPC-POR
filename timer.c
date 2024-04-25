/*
 * timer.c
 *
 * Created: 21.02.2024 19:56:06
 *  Author: msukdo00
 */ 
#include "timer.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/cpufunc.h>

volatile uint16_t timerloverflowcount=0;

ISR(TIMER1_OVF_vect)
{
	//flag nulovanej interaptem
	++timerloverflowcount;
}

void busyDelay(uint32_t cas)
{
	cas-=2;
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();

	
	for (; cas !=0;--cas)
	{
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
	}
}

void delay(uint32_t cas)
{
	TCNT1=0;//vynulovani casovace
	TIFR1=(1<<TOV1)|(1<<OCF1A);//smazani flag
	cas=cas<<1;//nasobeni 2
	uint16_t overflow = (uint16_t)(cas>>16);
	OCR1A=(uint16_t)(0xffff&cas);
	TCCR1B|=0b00000010;//nastaveni delicky
	OCR1A-=6;//zpresneni pro maly cisla
	
	while (overflow!=0)
	{
		if((TIFR1&(1<<TOV1))!=0)
		{
			--overflow;
			TIFR1=(1<<TOV1)|(1<<OCF1A);
		}
	}
	
	while ((TIFR1&(1<<OCF1A))==0)
	{
	}
	TCCR1B=0;//vypnuti casovace
}


void interaptDelay(uint32_t cas)
{
	Inittime();
	uint32_t change=getTimer();
	while (getTimer()<(cas+change))
	{
	}
	timeStop();
}

void Inittime(void)
{
	TCNT1=0;//vynulovani casovace
	TIFR1=(1<<TOV1)|(1<<OCF1A);//smazani flag
	TCCR1B|=0b00000010;//nastaveni delicky
	TIMSK1=1;//povoleni preruseni casovace
}

void timeStop(void)
{
	TCNT1=0;//vynulovani casovace
	TIFR1=(1<<TOV1)|(1<<OCF1A);//smazani flag
	TIMSK1=0;//povoleni preruseni casovace
	TCCR1B=0;//vypnuti casovace
}

uint32_t getTimer(void)
{
	uint32_t cas=((uint32_t)(timerloverflowcount)<<16)+(uint32_t)(TCNT1);
	cas=cas>>1;
	return cas;
}