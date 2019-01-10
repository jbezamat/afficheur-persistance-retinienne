#ifndef __HALL_SENSOR_H__
#define __HALL_SENSOR_H__


#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// #define F_CPU 13000000
// #define FOSC F_CPU// Clock Speed
// #define MYUBRR FOSC/8/BAUD-1

//Initialize Hall effect timer
void Init_Hall_Timer();

void Init_Hall_Interrupt();

#endif

