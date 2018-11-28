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

void Init_Interrupt()
{
    sei();
}

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


unsigned char[5] usart_buffer;
ISR(USART0_RX_vect)
{
    unsigned char carac = UDR0;

    if(carac == 'h') {
        char buffer[64];
        sprintf(buffer, "%u:%u", hour, minute);
        USART_puts(buffer);
    }

    //Fill USART buffer 
    for(int i = sizeof(usart_buffer)/sizeof(usart_buffer[0]) - 1; i >= 0; i--) {
        usart_buffer[i+1] = usart_buffer[i];
    }

    usart_buffer[0] = carac;

    //If it a time, save it
    if(usart_buffer[2] == ':') {
        hour = usart_buffer[0] << 8 | usart_buffer[1];
        minute = usart_buffer[3] << 8 | usart_buffer[4];
    }

    USART_Transmit(carac);
}

void SPI_MasterInit()
{
    /* Set MOSI and SCK output, all others input */
    DDRB |= (1 << DDB3) | (1 << DDB5) ;
    /* Enable SPI, Master, TODO: set clock rate fck/4 */
    SPCR |= (1 << SPE) | (1 << MSTR);
    /*Set PB3(OE), PB4 (LE)*/
    DDRD |= (1 << DDD3) | (1 << DDD4);

    PORTD = PORTD | _BV(PD3) | _BV(PD4);
}

void Init_Seconds() 
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
    PORTD |= (1 << PORTD4); // latch
    PORTD &= ~(1 << PORTD4);
    PORTD &= ~(1 << PORTD3); // OE
    PORTD |= (1 << PORTD3);
}

void main()
{
    // Active et allume la broche PB5 (led)
    Init_Interrupt();
    USART_Init (MYUBRR);
    SPI_MasterInit();
    Init_Seconds();
    char cData = 0xFF;
    char cData2 = 0xFF;
    char buffer[64];
    while (1)
    {
        
    }
}