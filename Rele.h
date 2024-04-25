/*
 * rele.h
 *
 * Created: 01.04.2024 16:39:52
 *  Author: msukd
 */ 


#ifndef RELE_H_
#define RELE_H_

#pragma once
#include "helpf.h"

void initrelay() ;

void relay1_on();
void relay1_off();

void relay2_on();
void relay2_off() ;

void reley_temostat(int *data, float tepl);


#endif /* RELE_H_ */
