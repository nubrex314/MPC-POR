/*
 * RTC.c
 *
 * Created: 20.03.2024 12:50:56
 *  Author: msukd
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/cpufunc.h>
#include "TWI.h"

#define timeout			160

void initTWI(uint32_t Hz)
{
	TWSR &= ~((1<<TWPS1)|(1<<TWPS0));//predelicka nastavena na 1
	TWBR =(16000000-17*Hz)/2/Hz;//nastaveni rychlosti komunikace
	PORTC =(1<<PORTC4)|(1<<PORTC5);//piny pro komunikaci  SDA-PC4 SCL-PC5
	TWCR=(1<<TWEN);//nastaveni 2-wire
}

void twi_start(void)
 {
	// Start I2C
	uint16_t i =0;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	while (!(TWCR & (1<<TWINT))&&(i<timeout))
	{i++;
	}
	
	if ((TWSR & 0xF8) != START_ACK)
	{
		error(START_ACK_ERROR);
	}
}

void twi_restart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		uint16_t i =0;
	while (!(TWCR & (1<<TWINT))&&(i<timeout))
		{i++;
		}
	
	if ((TWSR & 0xF8) != REP_START_ACK)
	{
		error(REP_START_ACK_ERROR);
	}
}

void twi_stop(void) 
{
TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void twi_addres_write_ack(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
		uint16_t i =0;
	while (!(TWCR & (1<<TWINT))&&(i<timeout))
		{i++;
		}
	
	if ((TWSR & 0xF8) != SLA_W_ACK)
	{
		error(SLA_W_ACK_ERROR);
	}
}

void twi_data_write_ack(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
		uint16_t i =0;
	while (!(TWCR & (1<<TWINT))&&(i<timeout))
		{i++;
		}
	
	if ((TWSR & 0xF8) != DATA_W_ACK)
	{
		error(DATA_ACK_ERROR);
	}
}

void twi_addres_read_ack(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
		uint16_t i =0;
	while (!(TWCR & (1<<TWINT))&&(i<timeout))
		{i++;
		}
	
	if ((TWSR & 0xF8) != SLA_R_ACK)
	{
		error(SLA_R_ACK_ERROR);
	}
}

void twi_data_read_ack(int enable)
{
	uint16_t i = 0;
	
	if(enable != 0)
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1<<TWINT))&&(i<timeout))
		{i++;
		}
	}
	else
	{
	
			TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1<<TWINT))&&(i<timeout))
			{i++;
			}
	}
	if ((TWSR & 0xF8) != DATA_R_ACK||(TWSR & 0xF8) != DATA_R_NACK)
	{
		error(DATA_READ_ACK_ERROR);
	}
}

void twi_write(uint8_t *data, uint8_t addres, uint8_t regis,uint16_t leng) 
{
	// Zápis dat do I2C
	uint16_t i = 0;
	twi_start();
	
	TWDR=(addres<<1)|0;//+write bit
	twi_addres_write_ack();
	
	TWDR=regis;
	twi_data_write_ack();
	
	for (i=0;i<leng;i++)
	{
		TWDR = data[i];
		twi_data_write_ack();
	}
	
	twi_stop();
}

void twi_read(uint8_t *data, uint8_t addres, uint8_t regis,uint16_t leng)
{
	// Cteni dat z I2C
	uint16_t i = 0;
	twi_start();
	
	TWDR=(addres<<1)|0;//+write bit
	twi_addres_write_ack();
	
	TWDR=regis;
	twi_data_write_ack();
	
	twi_restart();
	
	TWDR=(addres<<1)|1;//+read bit
	twi_addres_read_ack();
	
	for (i=0;i<leng;i++)
	{
		twi_data_read_ack(1);
		data[i] =TWDR ;
		
	}
	twi_data_read_ack(0);
	
	twi_stop();
}

void error(int8_t err)
{
	switch (err) {
		case NO_ERROR:
		// Žádná chyba
		break;
		case START_ACK_ERROR:
		//twi_stop();
		break;
		case REP_START_ACK_ERROR:
		//twi_stop();
		break;
		case SLA_W_ACK_ERROR:
		//twi_stop();
		break;
		case DATA_ACK_ERROR:
		//twi_stop();
		break;
		case LOST_ARB_ERROR:
		//twi_stop();
		break;
		case SLA_R_ACK_ERROR:
		//twi_stop();
		break;
		case DATA_READ_ACK_ERROR:
		//twi_stop();
		break;
		default:
		// Neznámý chybový kód
		break;
	}
}