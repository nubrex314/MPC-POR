/*
 * UART.c
 *
 * Created: 28.02.2024 12:17:25
 *  Author: msukd
 */ 

#include "UART.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/cpufunc.h>

volatile char prijate_data=' ';
volatile uint8_t poz_string=-1;

int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
	return 0;
}

int uart_getchar(FILE *stream)
{
	return USART_Receive();
}

void initUART(uint16_t bautrate)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(bautrate>>8);
	UBRR0L = (unsigned char)bautrate;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//preruseni
	UCSR0B |= (1 << RXCIE0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C |=(3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)))
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)))
	;
	/* Get and return received data from buffer */
	return UDR0;
}

ISR(USART_RX_vect) 
{
	/*if(poz_string>=15)
	{
		poz_string=0;
	}else
		poz_string++;
*/
    prijate_data = UDR0;
	USART_Transmit(prijate_data);
	
}

char USART_interupt_read()
{
	return prijate_data;
}

uint8_t USART_interupt_string_poz()
{
	return poz_string;
}

void USART_interupt_set(char data,uint8_t poz)
{
	prijate_data=data;
	poz_string=poz;
	return;
}