/*
 * EEPROM.c
 *
 * Created: 20.03.2024 12:51:54
 *  Author: msukd
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/cpufunc.h>
#include "timer.h"
#include "TWI.h"
#include "EEPROM.h"

#define Addr_eeprom 0x50

void eeprom_write(uint16_t addr,int *data,uint16_t size) 
{
	// Cteni dat z I2C
	uint16_t i = 0;
	twi_start();
	
	TWDR=(Addr_eeprom<<1)|0;//+write bit
	twi_addres_write_ack();
	
	uint8_t high = (addr&0x1f00)>>8;
	TWDR=high;
	twi_data_write_ack();
	
	uint8_t low = (addr&0x00ff);
	TWDR=low;
	twi_data_write_ack();
	
	for (i=0;i<size;i++)
	{
		TWDR = data[i];
		twi_data_write_ack();
	}
	twi_stop();
}

void eeprom_read(uint16_t add,int *data,uint16_t size)
{
	// Cteni dat z I2C
	uint16_t i = 0;
	twi_start();
	
	TWDR=(Addr_eeprom<<1)|0;//+write bit
	twi_addres_write_ack();
	
	uint8_t high = (add&0x1f00)>>8;
	TWDR=high;
	twi_data_write_ack();
	
	uint8_t low = (add&0x00ff);
	TWDR=low;
	twi_data_write_ack();
	
	twi_restart();
	
	TWDR=(Addr_eeprom<<1)|1;//+read bit
	twi_addres_read_ack();
	
	
	for (i=0;i<size;i++)
	{
		twi_data_read_ack(1);
		data[i] =TWDR ;
		
	}
	twi_data_read_ack(0);
	
	twi_stop();
}