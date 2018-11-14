#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define FOSC F_CPU// Clock Speed
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD-1

void write_char(unsigned char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = c;
    while ((UCSR0A & (1 << TXC0)) == 0x00)
        ;
}

ISR(USART_RX_vect)
{
    //PORTB = PORTB | _BV(PB5);
    //int b = RXB80;
    unsigned char carac = UDR0;
    write_char(carac);
}

void init_interrupt()
{
    sei();
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

void UART_init(unsigned int ubbr)
{
    UBRR0H = (unsigned char)(ubbr>>8);
    UBRR0L = (unsigned char)(ubbr);

    UCSR0A = (_BV(U2X0));
    UCSR0B = (_BV(RXEN0)) | (_BV(TXEN0)) | (_BV(RXCIE0)); //enable RX and Tx and interrupt on reception
    UCSR0C = (3 << (UCSZ00));                           , //8bit char
}

void main()
{
    // Active et allume la broche PB5 (led)
    USART_Init (MYUBRR);
    SPI_MasterInit();
    init_interrupt();
    int led = 0;
    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);
    PORTD = PORTD | _BV(PD3);  //set LDAC high

    while (1)
    {
        //write_char('x');
        _delay_ms(100);
        PORTB = PORTB | _BV(PB5);
        SPI_MasterTransmit( 'c' );
        _delay_ms(100);
        PORTB = PORTB & ~_BV(PB5);
    }
}