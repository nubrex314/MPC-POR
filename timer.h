/*
 * timer.h
 *
 * Created: 21.02.2024 19:56:34
 *  Author: msukd
 */ 
 /*!****************************************************************************
 * @file timer.h
 * @brief funkce zabezpečujici měreni času/spoždeni 
 * init 	sei();
 * end 		cli();
 *******************************************************************************/


#ifndef TIMER_H_
#define TIMER_H_

#pragma once
#include <avr/interrupt.h>//preruseni

/**
*\brief vytvoreni funkce na zaklade asmblerovskeho prikazu
*/
#define NOP() asm volatile ("nop")

/**
 * \brief presny casovac na zaklade počtu instrukci v asm a použiti operatoru nop
 * \param cas doba kterou casovac pretrvava ve funkci
 */
void busyDelay(uint32_t cas);

/**
 * \brief casovac pracujici na zaklade odecitani hodnoty z TIFR1 a OCF1A
 * \param cas doba kterou casovac pretrvava ve funkci
 */
void delay(uint32_t cas);

/**
 * \brief casovac pracujici poci funkci - timeInit,timeStop,getTimer a Přerušeni
 * \param cas doba kterou casovac pretrvava ve funkci
 */
void interaptDelay(uint32_t cas);

/**
 * \brief nastaveni casovace 
 *Vynulovani,smazani flagu,nastaveni deličky na 8 a povoleni preruseni
 */
void Inittime(void);

/**
 * \brief vypnuti casovace 
 *Vynulovani,smazani flagu a zakazani preruseni
 */
void timeStop(void);

/**
 * \brief ziskani aktualni hodnoty casu
 * hodnota casovace je vynasobena 2 
 * \return aktualni hodnota casu od zapnuti casovace
 */
uint32_t getTimer(void);

#endif /* TIMER_H_ */
