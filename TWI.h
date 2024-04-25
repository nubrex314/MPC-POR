/*
 * RTC.h
 *
 * Created: 20.03.2024 12:51:09
 *  Author: msukd
 */ 


#ifndef RTC_H_
#define RTC_H_

#pragma once
#include <avr/interrupt.h>


#define START_ACK		 0x08 // Start byl odeslán
#define REP_START_ACK	 0x10 // Opakovaný start byl odeslán
#define SLA_W_ACK		 0x18 // SLA+W byl odeslán a ACK pøijat
//#define SLA_W_NACK		 0x20 // SLA+W byl odeslán a NACK pøijat
#define DATA_W_ACK		 0x28 // Data byte byl odeslán a ACK pøijat
//#define DATA_W_NACK		 0x30 // Data byte byl odeslán a NACK pøijat
#define LOST_ARB		 0x38 // Ztráta arbitráže v SLA+W nebo data byte

#define SLA_R_ACK		0x40 // SLA+R byl odeslán a ACK pøijat
//#define SLA_R_NACK		0x48 // SLA+R byl odeslán a NACK pøijat
#define DATA_R_ACK		0x50 // Data byte byl pøijat a ACK odeslán
#define DATA_R_NACK		0x58 // Data byte byl pøijat a NACK odeslán

typedef enum {
	NO_ERROR = 0, // Žádná chyba
	START_ACK_ERROR=-1, // Chyba pøi odesílání startu
	REP_START_ACK_ERROR=-2, // Chyba pøi odesílání opakovaného startu
	SLA_W_ACK_ERROR=-3, // Chyba pøi odesílání SLA+W
	DATA_ACK_ERROR=-4, // Chyba pøi odesílání dat a pøijetí ACK
	LOST_ARB_ERROR=-5, // Chyba pøi ztrátì arbitráže
	SLA_R_ACK_ERROR=-6, // Chyba pøi odesílání SLA+R a pøijetí ACK
	DATA_READ_ACK_ERROR=-7, // Chyba pøi ètení dat a odesílání ACK
} I2C_ErrorCode;

void initTWI(uint32_t Hz);

void twi_start(void);
void twi_restart(void);
void twi_stop(void) ;

void twi_addres_write_ack(void);
void twi_data_write_ack(void);
void twi_addres_read_ack(void);
void twi_data_read_ack(int enable);

void twi_write(uint8_t *data, uint8_t addres, uint8_t regis,uint16_t leng);
void twi_read(uint8_t *data, uint8_t addres, uint8_t regis,uint16_t leng);

void error(int8_t err);

#endif /* RTC_H_ */