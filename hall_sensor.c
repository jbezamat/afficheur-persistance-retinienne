#include "hall_sensor.h"

//Initialize Hall effect timer
void Init_Hall_Timer() 
{
    //Set prescaler to 256
    TCCR3B |= _BV(CS31) | _BV(CS30);;

    //Set CTC mode
    TCCR3B |= _BV(WGM32);
}

void Init_Hall_Interrupt()
{
    //Disable hall sensor interrupt to setup it
    EIMSK &= ~_BV(INT0);

    //Set interrupt at falling edge
    EICRA |= ISC01;

    //Enable hall sensor interrupt
    EIMSK |= _BV(INT0);
}
