#define F_CPU 13000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define FOSC F_CPU// Clock Speed
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD-1

void SPI_MasterInit()
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1 << DDB3) | (1 << DDB5) ;
    /* Enable SPI, Master, TODO: set clock rate fck/4 */
    SPCR = (1 << SPE) | (1 << MSTR);
    /*Set PB3(OE), PB4 (LE)*/
    DDRD = (1 << DDD3) | (1 << DDD4);
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
    PORTD |= (0 << PORTD4);
    PORTD |= (1 << PORTD3); // OE
    _delay_ms(2000);
    PORTD |= (0 << PORTD3);
}


void main()
{
    SPI_MasterInit();
    char cData = 0x01;
    char cData2 = 0x00;
    while (1)
    {
        SPI_MasterTransmit(cData, cData2);
        _delay_ms(1000);
    }
}