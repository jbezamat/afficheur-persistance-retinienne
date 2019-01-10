#include "SPI.h"

void SPI_MasterInit()
{    
    /* Set MOSI and SCK output, all others input */
    DDRB |= (1 << DDB0) | (1 << DDB2) | (1 << DDB1) ;
    /* Enable SPI, Master, TODO: set clock rate fck/4 */
    SPCR |= (1 << SPE) | (1 << MSTR);
    /*Set PE4(OE), PE5 (LE)*/
    DDRE |= (1 << DDE4) | (1 << DDE5);

    PORTB &= ~(1<<PB0);
    PORTE &= ~(1 << PORTE4); // OE
}

void SPI_MasterTransmit(char cData, char cData2)
{
    /* Start transmission */

    PORTE &= ~(1 << PORTE4); // OE

    PORTB &= ~(1<<PB0);
    SPDR = cData;

    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
    /* Start transmission */
    PORTB |= (1<<PB0);
    _delay_us(20);
    PORTB &= ~(1<<PB0);
    SPDR = cData2;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));

    PORTB |= (1<<PB0);

    /* Latch and Output enable terminal*/
    PORTE |= (1 << PORTE5); // latch
    PORTE &= ~(1 << PORTE5);

    PORTE |= (1 << PORTE4);
    PORTE &= ~(1 << PORTE4); // OE

}