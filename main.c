/*
 * termostat.c
 *
 * Created: 31.03.2024 16:43:44
 * Author : msukd
 */ 
#include <stdio.h>
#include <avr/io.h>
#include "UART.h"
#include "RTC.h"
#include "EEPROM.h"
#include "ADC.h"
#include "TWI.h"
#include "keyboard.h"
#include "helpf.h"
#include "Rele.h"
#include "LCD.h"
#include "menu.h"
#include "timer.h"

FILE LCDout = FDEV_SETUP_STREAM(LCD_putchar, NULL,_FDEV_SETUP_WRITE);
//FILE UARTout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

typedef enum {
	SELECT,
	LEFT,
	DOWN,
	UP,
	RIGHT,
	RST,
	NONE
} Button;

//Pocitadlo pro casovac
volatile uint8_t gen = 0;
//obnovovaci perioda /2
uint8_t obnov=10;
//teplota v °C
float ADC_temp=0;
//hodnot ukladane do a z EEPROM
int data_EEPROM[32];

//interupt pocitadla casovace
ISR(PCINT0_vect) 
{
	gen++;
}

int main(void)
{
	initglobal();
	stdout = &LCDout;
	printf("BOOTING");
	eeprom_read(0x00, &data_EEPROM, 32);
	rtc_set_sec(data_EEPROM[0]);
	rtc_set_min(data_EEPROM[1]);
	rtc_set_hour(data_EEPROM[2]);
	rtc_set_days(data_EEPROM[3]);
	rtc_set_months(data_EEPROM[4]);
	rtc_set_year(data_EEPROM[5]);
	play_super_mario_theme();


	while (1)
	{
		//vypisovani a mereni kazdych 5s
		if (gen > obnov)
		{
			data_EEPROM[0] = rtc_read_sec();
			data_EEPROM[1] = rtc_read_min();
			data_EEPROM[2] = rtc_read_hours();
			data_EEPROM[3] = rtc_read_days();
			data_EEPROM[4] = rtc_read_months();
			data_EEPROM[5] = rtc_read_year();
			LCD_set_cursor(0, 0);
			printf("T:%2.0d.%2.0d.%2.0d,%2.0d.%2.0d", data_EEPROM[2], data_EEPROM[1],data_EEPROM[0],data_EEPROM[3], data_EEPROM[4]);
			LCD_set_cursor(1, 0);
			printf("teplota:");
			
			if (data_EEPROM[14] == 0)
			{
				ADC_temp = ADC_normtemp(ADC_read(1)) - int_to_float(data_EEPROM[8], data_EEPROM[9]);
			}
			else
			{
				ADC_temp = ADC_normtemp(ADC_read(1)) + int_to_float(data_EEPROM[8], data_EEPROM[9]);
			}
			printfloat(ADC_temp);
			reley_temostat(&data_EEPROM, ADC_temp);
			eeprom_write(0x00, &data_EEPROM, 32);
			gen = 0;
		}
		
		//ovladani pomoci tlacitek na display
		if (LCD_Button_adc_value(LCD_button()) != NONE)
		{
			if (LCD_Button_adc_value(LCD_button()) == SELECT)
			{
				menu(&data_EEPROM);
				rtc_set_sec(data_EEPROM[0]);
				rtc_set_min(data_EEPROM[1]);
				rtc_set_hour(data_EEPROM[2]);
				rtc_set_days(data_EEPROM[3]);
				rtc_set_months(data_EEPROM[4]);
				rtc_set_year(data_EEPROM[5]);
			}
			else
			{
				menu_teplota(&data_EEPROM);
			}
			eeprom_write(0x00, &data_EEPROM, 32);
		}
		
		//presun do remote modu
		if ( USART_interupt_read() == 'r')
		{
			menu_rem(&data_EEPROM);
			rtc_set_sec(data_EEPROM[0]);
			rtc_set_min(data_EEPROM[1]);
			rtc_set_hour(data_EEPROM[2]);
			rtc_set_days(data_EEPROM[3]);
			rtc_set_months(data_EEPROM[4]);
			rtc_set_year(data_EEPROM[5]);
			eeprom_write(0x00, &data_EEPROM, 32);
		}
		
		//blind mode
		if (!(PINB & (1 << DDB7)))
		{
			while (!(PINB & (1 << DDB7)))
			{
			}
			numberToMorse((int)ADC_temp);
		}
	}
}
