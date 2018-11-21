#define F_CPU 13000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define FOSC F_CPU// Clock Speed
#define BAUD 38400
#define MYUBRR FOSC/8/BAUD-1

void USART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
    while ((UCSR0A & (1 << TXC0)) == 0x00);
}

ISR(USART_RX_vect)
{
    unsigned char carac = UDR0;
    USART_Transmit(carac);
}

void Init_Interrupt()
{
    //sei();
}

void SPI_MasterInit()
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1 << DDB3) | (1 << DDB5);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}
void SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
}

void USART_Init(unsigned int ubbr)
{
    UBRR0H = (unsigned char)(ubbr>>8); //Baudrate 
    UBRR0L = (unsigned char)(ubbr);

    UCSR0A = (_BV(U2X0)); //
    UCSR0B = (_BV(RXEN0)) | (_BV(TXEN0)) | (_BV(RXCIE0)); //enable RX and Tx and interrupt on reception
    UCSR0C = (3 << (UCSZ00));                             //8bit char
}

void main()
{
    // Active et allume la broche PB5 (led)
    USART_Init (MYUBRR);
    //SPI_MasterInit();
    Init_Interrupt();

    while (1)
    {
        //USART_Transmit('x');
    }
}