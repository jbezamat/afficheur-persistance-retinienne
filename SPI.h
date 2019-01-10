
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 13000000
#define FOSC F_CPU// Clock Speed
#define MYUBRR FOSC/16/BAUD-1
#define BAUD 38400

void SPI_MasterInit();

void SPI_MasterTransmit(char cData, char cData2);