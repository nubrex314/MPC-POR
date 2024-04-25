/*
 * ADC.c
 *
 * Created: 03.03.2024 9:25:53
 *  Author: msukd
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/cpufunc.h>
#include <math.h>
#include "ADC.h"

volatile int16_t ADC_data=-1;

ISR(ADC_vect)
{
	//flag nulovanej interaptem
	ADC_data=ADC;
	//ADC_data=315;//simulace
}

void initADC(void)
{
	ADMUX |=(1<< REFS0);//referencni napeti VCC
	ADCSRA |=(1<<ADEN);//enable adc
	ADCSRA |=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//preddelicka na 128 oproti system clocku
	ADMUX &= ~(MUX3|MUX2|MUX1|MUX0);//vymazani
	ADCSRA |=(1<<ADIE);//interupt enable
}

void ADC_start(uint8_t ADCport)
{
	ADMUX &= ~(MUX3|MUX2|MUX1|MUX0);//vymazani
	DIDR0|=ADCport;//vypnuti portu
	ADMUX|=ADCport;//zapnuti ADC portu
	ADCSRA|=(1<<ADSC);//spusteni ADC
}

int16_t ADC_read(uint8_t ADCport)
{
	ADC_data=-1;
	ADC_start(ADCport);
	while(ADC_data==-1)
	{}
		
		return ADC_data;
}

float ADC_normtemp(uint16_t raw)
{
	float Vol = raw*5.0/1024.0; // bit na volt
	float Th1 = Vol /(5.0-Vol)*20000.0 - 1500.0;// R-R3
	return (3895.0/(log(Th1/10000.0)+3895.0/298.15))-273.15;//vypocet teploty;
}

