#define F_CPU 13000000

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
//#include "USART.h"

#define FOSC F_CPU// Clock Speed
#define BAUD 38400
#define MYUBRR FOSC/8/BAUD-1

int hour = 0;
int minute = 0;

ISR(TIMER1_COMPA_vect)
{
    minute++;

    if(minute >= 60) {
        minute = 0;
        hour++;
    }

    if(hour >= 24) {
        hour = 0;
    }
}

void USART_Init(unsigned int ubbr)
{
    UBRR0H = (unsigned char)(ubbr>>8); //Baudrate at 38400
    UBRR0L = (unsigned char)(ubbr);

    UCSR0A = (_BV(U2X0)); //double speed
    UCSR0B = (_BV(RXEN0)) | (_BV(TXEN0)) | (_BV(RXCIE0)); //enable RX and Tx and interrupt on reception
    UCSR0C = (3 << (UCSZ00));                             //8bit char
}


void Init_Interrupt()
{
    sei();
}

void SPI_MasterInit()
{    
    /* Set MOSI and SCK output, all others input */
    DDRB |= (1 << DDB2) | (1 << DDB1) ;
    /* Enable SPI, Master, TODO: set clock rate fck/4 */
    SPCR |= (1 << SPE) | (1 << MSTR);
    /*Set PE4(OE), PE5 (LE)*/
    DDRE |= (1 << DDE4) | (1 << DDE5);

    // PORTE = PORTE | _BV(PE4) | _BV(PE5);
}

void Init_Watch() 
{
    //Set prescaler to 1024
    TCCR1B |= _BV(CS10);

    //Set CTC mode
    TCCR1B |= _BV(WGM12);

    //Set overflow to 12695
    OCR1AH = 0x31;
    OCR1AL = 0x97;

    //Set interrupt on compare match
    TIMSK |= _BV(OCIE1A);
}

void USART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
    while ((UCSR0A & (1 << TXC0)) == 0x00);
}

void USART_puts(unsigned char *mot){

    //sprintf(buffer, "x=%u\r\n", TCNT1);
    int k = 0;
    while(mot[k] != '\0'){
        USART_Transmit(mot[k]);
        k++;
    } 
}

void SPI_MasterTransmit(char cData, char cData2)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
    /* Start transmission */

    SPDR = cData2;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));

    /* Latch and Output enable terminal*/
    PORTE |= (1 << PORTE5); // latch
    PORTE &= ~(1 << PORTE5);
    PORTE &= ~(1 << PORTE4); // OE
    PORTE |= (1 << PORTE4);
}


int atoi(char* s, int l)
{
    int val = 0;

    int i = 1;
    while(i <= l) {
        if(s[l-i] < '0' || s[l-i] > '9')
            return val;

        int u = s[l-i] - '0';
        
        int j = 1;
        int pow = 1;
        while(j <= i-1)
        {
            pow *= 10;
            j++;
        }
        
        val += u * pow;
        
        i++;
    }

    return val;
}

unsigned char usart_buffer[5];
ISR(USART0_RX_vect)
{
    unsigned char carac = UDR0;

    if(carac == 'h') {
        char buffer[64];
        sprintf(buffer, "%u:%u", hour, minute);
        USART_puts(buffer);
    }

    //Fill USART buffer 

    int i = sizeof(usart_buffer)/sizeof(usart_buffer[0]) - 1;
    while(i >= 0){
        usart_buffer[i+1] = usart_buffer[i];
        i--;
    }

    usart_buffer[0] = carac;

    //If it a time, save it
    if(usart_buffer[2] == ':') { // Changement d'heure avant d'avoir l'info. Mettre un flage et faire le changement après
        USART_Transmit('a');
        char h[2] = {usart_buffer[0], usart_buffer[1]};
        char m[2] = {usart_buffer[3], usart_buffer[4]};

        hour = atoi(h, 2);
        minute = atoi(m, 2);
    }

    // if(carac == 'h') {
    //     displayTime();
    // }
    //changeTime(carac);
}

//Initialize Hall effect timer
void Init_Hall_Timer() 
{
    //Set prescaler to 1024
    TCCR3B |= _BV(CS32) | _BV(CS30);

    //Set CTC mode
    TCCR3B |= _BV(WGM32);

    //Set overflow to 12695
    OCR3AH = 0x31;
    OCR3AL = 0x97;

    //Set interrupt on compare match
    TIMSK |= _BV(OCIE3A);
}

void main()
{
    // Active et allume la broche PB5 (led)
    Init_Interrupt();
    USART_Init (MYUBRR);
    SPI_MasterInit();
    Init_Watch();
    char cData = 0xFF;
    char cData2 = 0xFF;
    char buffer[64];
    while (1)
    {
        SPI_MasterTransmit(cData, cData2);
        _delay_ms(1000);
    }
}