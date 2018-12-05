#define F_CPU 13000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define FOSC F_CPU// Clock Speed
#define BAUD 38400
#define MYUBRR FOSC/8/BAUD-1


void USART_Transmit(unsigned char data);


void USART_Init(unsigned int ubbr);