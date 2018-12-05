#include "USART.h"

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
