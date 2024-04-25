/*
 * RTC.c
 *
 * Created: 23.03.2024 19:16:50
 *  Author: msukd
 */ 

#include "RTC.h"


void rtc_set(uint8_t *data)
{
	rtc_set_sec(data[0]);
	rtc_set_min(data[1]);
	rtc_set_hour(data[2]);
	rtc_set_days(data[3]);
	rtc_set_months(data[4]);
	rtc_set_year(data[5]);
}

void rtc_read(uint8_t *data)
{
			data[0] = rtc_read_sec();
			data[1] = rtc_read_min();
			data[2] = rtc_read_hours();
			data[3] = rtc_read_days();
			data[4] = rtc_read_months();
			data[5] = rtc_read_year();
}
uint8_t rtc_read_sec()
{
	uint8_t sec = 0;
	uint8_t jednotky = 0;
	uint8_t desiatky = 0;
	twi_read(&sec,0x68,0x00, sizeof(sec));
		jednotky = sec & 0x0F;
		desiatky = ((sec & 0x70)>>4);
		return desiatky*10 + jednotky;
}

uint8_t rtc_read_min()
{
	uint8_t min = 0;
	uint8_t jednotky = 0;
	uint8_t desiatky = 0;

		twi_read(&min,0x68,0x01, sizeof(min));
		jednotky = min & 0x0F;
		desiatky = ((min & 0x70)>>4);
		return desiatky*10 + jednotky;
}

uint8_t rtc_read_hours()
{
	uint8_t hour = 0;
	uint8_t jednotky = 0;
	uint8_t desiatky = 0;

	twi_read(&hour,0x68,0x02,sizeof(hour));
		jednotky = hour & 0x0F;
		desiatky = ((hour & 0x30)>>4);
		return desiatky*10 + jednotky;
}

uint8_t rtc_read_days()
{
	uint8_t day = 0;
	uint8_t jednotky = 0;
	uint8_t desiatky = 0;

	twi_read(&day,0x68,0x04,sizeof(day));
		jednotky = day & 0x0F;
		desiatky = ((day & 0x30)>>4);
		return desiatky*10 + jednotky;
}

uint8_t rtc_read_months()
{
	uint8_t month = 0;
	uint8_t jednotky = 0;
	uint8_t desiatky = 0;

	twi_read(&month,0x68,0x05, sizeof(month));
		jednotky = month & 0x0F;
		desiatky = ((month & 0x30)>>4);
		return desiatky*10 + jednotky;
}

uint8_t rtc_read_year()
{
	uint8_t year = 0;
	uint8_t jednotky = 0;
	uint8_t desiatky = 0;

	twi_read(&year,0x68,0x06,  sizeof(year));
		jednotky = year & 0x0F;
		desiatky = ((year & 0xF0)>>4);
		return desiatky*10 + jednotky;
}

uint8_t rtc_set_sec(uint8_t seconds)
{
	if(seconds >= 60)
	{
		seconds=59;
	}	
		uint8_t en_couter = rtc_read_sec() & 0x80;
		uint8_t jednotky = seconds%10;
		uint8_t desiatky = ((seconds%100)-jednotky)/10;
		uint8_t sec_bcd = (desiatky << 4) | jednotky | en_couter ;
		return twi_write(&sec_bcd,0x68,0x00,1);
}

uint8_t rtc_set_min(uint8_t minutes)
{
	if(minutes < 60)
	{
		uint8_t state_hour = rtc_read_hours() & 0xC0;
		uint8_t jednotky = minutes%10;
		uint8_t desiatky = ((minutes%100)-jednotky)/10;
		uint8_t hour_bcd = (desiatky << 4) | jednotky | state_hour;
		return twi_write(&hour_bcd,0x68,0x01,1);
	}
	return 0xFF;
}

uint8_t rtc_set_hour(uint8_t hour)
{
	if(hour < 24)
	{
		uint8_t jednotky = hour%10;
		uint8_t desiatky = ((hour%100)-jednotky)/10;
		uint8_t hour_bcd = (desiatky << 4) | jednotky;
		return twi_write(&hour_bcd,0x68,0x02,1);
	}
	return 0xFF;
}

uint8_t rtc_set_days(uint8_t day)
{
	if(day < 32)
	{
		uint8_t jednotky = day%10;
		uint8_t desiatky = ((day%100)-jednotky)/10;
		uint8_t hour_bcd = (desiatky << 4) | jednotky;
		return twi_write(&hour_bcd,0x68,0x04,1);
	}
	return 0xFF;
}

uint8_t rtc_set_months(uint8_t month)
{
	if(month < 13)
	{
		uint8_t jednotky = month%10;
		uint8_t desiatky = ((month%100)-jednotky)/10;
		uint8_t hour_bcd = (desiatky << 4) | jednotky;
		return twi_write(&hour_bcd,0x68,0x05,1);
	}
	return 0xFF;
}

uint8_t rtc_set_year(uint8_t year)
{
	if(year < 101)
	{
		uint8_t jednotky = year%10;
		uint8_t desiatky = ((year%100)-jednotky)/10;
		uint8_t hour_bcd = (desiatky << 4) | jednotky;
		return twi_write(&hour_bcd,0x68,0x06,1);
	}
	return 0xFF;
}

uint8_t rtc_start_count()
{
	uint8_t data = rtc_read_sec() & (~(1<<7));
	return twi_write(&data,0x68,0x00,1);
}

uint8_t rtc_stop_count()
{
	uint8_t data = rtc_read_sec() | (1<<7);
	return twi_write(&data,0x68,0x00,1);
}

uint8_t rtc_start_gen()
{
	uint8_t data = (1<<4);
	return twi_write(&data,0x68,0x07,1);
}

uint8_t rtc_stop_gen()
{
	uint8_t data = 0;
	return twi_write(&data,0x68,0x07,1);
}