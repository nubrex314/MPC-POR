/*
 * UART.h
 *
 * Created: 28.02.2024 12:17:48
 *  Author: msukd
 */ 
#pragma once
#include <avr/interrupt.h>//preruseni

#ifndef UART_H_
#define UART_H_
#include <stdio.h>

int uart_getchar(FILE *stream);

int uart_putchar(char c, FILE *stream);

void initUART(uint16_t bautrate);

void USART_Transmit(unsigned char data);

unsigned char USART_Receive(void);

void USART_interupt_set(char data,uint8_t poz);

#endif /* UART_H_ */