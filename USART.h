
#ifndef __USART_H__
#define __USART_H__


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 13000000
#define FOSC F_CPU// Clock Speed
#define BAUD 38400
#define MYUBRR FOSC/8/BAUD-1

void USART_Init(unsigned int ubbr);

void USART_Transmit(unsigned char data);

void USART_puts(unsigned char *mot);

#endif