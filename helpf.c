/*
 * helpf.c
 *
 * Created: 15.03.2024 16:30:45
 *  Author: msukd
 */ 
#include "helpf.h"

uint8_t morseNumbers[] = {
	31, 30, 28, 24, 16, 0, 15, 7, 3, 1
};


void initglobal(void)
{
	sei();
	Inittime();
	initADC();
	initLCD();
	initUART(25);
	initrelay();
	initTWI(100000);
	delay(10000);
	rtc_start_count();
	rtc_start_gen();
	// Povolen externch peruen pro PCINT0 (pin B3 pro tlatko)
	PCICR = (1 << PCIE0);
	// Povolen peruen pro konkrtn pin (B4)
	PCMSK0 = (1 << PCINT4);
	// Výstupní pin pro reproduktor
	DDRB |= (1 << PINB3);
}


void string_clear(char *string)
{
	for (int i = 0; i < 16; i++) {
		string[i] = '\0'; // Smazání obsahu øetìzce
	}
}

void printfloat(float in)
{
	int16_t Z=(int)in; // celecisla
	int16_t des=(in-Z)*100;//2 desetine cisla
	if(des<10)
		printf("%d.0%d C",Z ,des);
	else
		printf("%d.%d C",Z ,des);
		
	return;
}

float int_to_float(uint8_t Z,uint8_t des)
{
	return ((float)Z + ((float)des/100));
}

void print_binary(uint16_t num) {
    for (int8_t bit = sizeof(uint16_t)*8-7; bit >= 0; bit--) {
        putchar((num & (1 << bit)) ? '1' : '0');
    }
}

uint16_t ascii_to_number(uint16_t a)
{
	return a-48;
}

void play_tone(uint16_t frequency, uint16_t duration_ms) 
{
	// Výpoèet poètu cyklù pro danou frekvenci
	uint16_t cycles = 1000000 / (2 * frequency);

	// Generování tónu
	for (uint16_t i = 0; i < duration_ms; i++) {
		PORTB |= (1 << PINB3); // Zapnutí reproduktoru
		busyDelay(cycles); // Pauza
		PORTB &= ~(1 << PINB3); // Vypnutí reproduktoru
		busyDelay(cycles);  // Pauza
	}
}

// Funkce pro pøehrávání Super Mario melodie
void play_super_mario_theme()
 {
	uint16_t melody[] = {
		 659, 659, 0, 659, 0, 523, 659, 0, 784, 0, 392, 0, 523, 0, 392,
		 0, 330, 0, 440, 0, 494, 0, 466, 0, 440, 0, 392, 0, 659, 0, 784,
		 0, 880, 0, 698, 0, 784, 0, 659, 0, 523, 587, 494, 523, 0, 392,
		 0, 330, 0, 440, 0, 494, 0, 466, 0, 440, 0, 392
	};
	uint16_t duration= 50;

	for (uint8_t i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
		if(melody[i]!=0)
		{
			play_tone(melody[i], duration);
			delay(7000); // Pauza mezi tóny
		}
		else
		{
			delay(70000); // Pauza mezi tóny
		}
	}
}

void numberToMorse(int number)
 {
	 if (number==0)
	 {
		 play_morse(morseNumbers[0]);
		 return;
	 }
	while (number > 0) {
		int digit = number % 10;
		play_morse(morseNumbers[digit]);
		number /= 10;
	}
	return;
}

void play_morse(uint8_t morse)
{
	uint8_t carka=0;
	for (uint8_t i = 5; i > 0; i--) {
		carka=(morse&(1<<(i-1)))>>(i-1);
		if(carka==0)
		{
			play_tone(800, 100);//ton carky
			delay(70000);
		}
		else
		{
			play_tone(600, 100);//ton tecky
			delay(70000);
		}
	}
	play_tone(400, 100);//ton mezi cislama
}
