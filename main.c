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

void USART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
    while ((UCSR0A & (1 << TXC0)) == 0x00);
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


ISR(USART_RX_vect)
{
    unsigned char carac = UDR0;
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
void SPI_MasterTransmit(char cData, char cData2)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
    /* Start transmission */
    _delay_ms(10);
    SPDR = cData2;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
    USART_Transmit('b');

    /* Latch and Output enable terminal*/
    PORTD |= (1 << PORTD4); // latch
    PORTD &= ~(1 << PORTD4);
    PORTD |= (1 << PORTD3); // OE
    _delay_us(10);
    PORTD &= ~(1 << PORTD3);
}

void main()
{
    // Active et allume la broche PB5 (led)
    USART_Init (MYUBRR);
    Init_Interrupt();
    SPI_MasterInit();
    char cData = 0x01;
    char cData2 = 0x01;
    while (1)
    {
        //USART_Transmit('a');
        SPI_MasterTransmit(cData, cData2);
        _delay_ms(1000);
        //USART_Transmit('b');
        //_delay_ms(1000);
    }
}