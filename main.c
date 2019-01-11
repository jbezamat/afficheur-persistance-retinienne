#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdbool.h>

#include "SPI.h"
#include "USART.h"
#include "hall_sensor.h"
#include "curved.h"
#include "interface.h"

// #define F_CPU 13000000
// #define FOSC F_CPU// Clock Speed
// #define BAUD 38400
#define MYUBRR FOSC/8/BAUD-1

volatile int hour = 0;
volatile int minute = 0;
volatile int seconds = 0;

volatile uint16_t ts = 0;
//float speed = 0;
volatile int mode = 0;
int timer_value = 0;
volatile bool changedMode = false;

volatile uint16_t turnTime = 0;

volatile uint16_t first_int = 0;
volatile uint16_t second_int = 0;
volatile int i = 0;

volatile int tour = 0;

unsigned char usart_buffer[5];

volatile int lastHour = 0;
volatile int lastMinute = 0;

volatile char char1 = 0x0000;
volatile char char2 = 0x0000;

void Init_Interrupt()
{
    sei();
}

void Init_Watch() 
{
    //Set prescaler to 1024
    TCCR1B |= _BV(CS12) | _BV(CS10);

    //Set CTC mode
    TCCR1B |= _BV(WGM12);

    //Set overflow to 12695
    OCR1AH = 0x31;
    OCR1AL = 0x97;

    //Set interrupt on compare match
    TIMSK |= _BV(OCIE1A);
}


// char displaySeconds(char up, int minute){
//     int deg = Calc_deg(TCNT3);
//     if(deg <= minute*10*(turnTime/600)){
//         up = up | 0x80;
//     }
//     return up;
// }

uint16_t leds(uint32_t deg)
{
    uint16_t cData = 0x0000;
   
    if(((hour%12)*(turnTime/12)  >= deg-100) && ((hour%12)*(turnTime/12) <= deg+100)){
        cData = 0x000F;
    }
    if((deg <=100)||(deg >= turnTime-100)){
        if(hour%12 == 0){
            cData = 0x000F;
        }
        if(minute == 0){
            cData = 0x00FF;
        }
    }
    
    if((((minute)*(turnTime/60))-1 >= deg-100)&&((minute)*(turnTime/60)-1 <= deg+100)){
        cData = 0x00FF;
    }

    if(deg < minute*(turnTime/60)){
        cData = cData | 0x8000;
    }
 
    return cData;
}



void clock(uint16_t leds){
    char up = (leds >> 8);
    char down = leds;
    SPI_MasterTransmit(up, down);
}

void watch_tick(){
    if(minute >= 60) {
        minute = 0;
        hour++;
    }

    if(hour >= 24) {
        hour = 0;
    }
}

ISR(INT0_vect)
{
    uint16_t temp = TCNT3;
    TCNT3 = 0;

    if(temp < 10000){
        first_int = temp;
        TCNT3 = first_int;
    }

    else {
        second_int = temp;
        tour = 0;

    }
    turnTime = second_int;
}

ISR(USART0_RX_vect)
{
    unsigned char carac = UDR0;
    changeTime(carac);
}

ISR(TIMER1_COMPA_vect)
{
    minute++;
}

void timeStamp(){
    ts = (TCNT1H << 8) | TCNT1L;
}

int debugTime(){
    uint16_t temp = (TCNT1H << 8) | TCNT1L;
    if(temp < ts){
        return (temp + 21695-ts);
    }
    return (temp-ts);
}

void main()
{
    Init_Interrupt();
    USART_Init (MYUBRR);
    SPI_MasterInit();
    Init_Watch();
    Init_Hall_Timer();
    Init_Hall_Interrupt();
    int k = 0;
    uint32_t delta = 0;

    while (1)
    {
        watch_tick();
        if(mode == 0){
            clock(leds(Calc_deg(TCNT3)));  
        }
        if(mode == 1){
            displayCurveTime(hour, minute);
        }
        else if(mode == 2){
            displayCurveTime(hour, minute);
        }
        else if(mode == 3) {
            displayCurveTime(hour, minute);
        }
        
        else if(mode == 4){
            displayCurveTime(hour, minute);
        }
    }
}

