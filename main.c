#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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
    //__enable_interrupt();
    sei();
    //SREG |= (1<<_BV(SREG_I)); // Global interrupt enable
    //MCUCR = (1<<_BV(IVCE));
}

void SPI_MasterInit(void)
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

void UART_init()
{
    UBRR0H = (unsigned char)0x00;
    UBRR0L = (unsigned char)0x10; //baudrate at 115200

    UCSR0A = (_BV(U2X0));
    UCSR0B = (_BV(RXEN0)) | (_BV(TXEN0)) | (_BV(RXCIE0)); //enable RX and Tx
    UCSR0C = (3 << (UCSZ00));                             //8bit char
}

void main()
{
    // Active et allume la broche PB5 (led)
    UART_init();
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