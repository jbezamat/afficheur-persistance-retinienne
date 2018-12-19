#define F_CPU 13000000

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <stdlib.h>

#define FOSC F_CPU// Clock Speed
#define BAUD 38400
#define MYUBRR FOSC/8/BAUD-1

int hour = 0;
int minute = 0;

//float speed = 0;
int mode = 4;
int timer_value = 0;

ISR(TIMER1_COMPA_vect)
{
    minute++;


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
    DDRB |= (1 << DDB0) | (1 << DDB2) | (1 << DDB1) ;
    /* Enable SPI, Master, TODO: set clock rate fck/4 */
    SPCR |= (1 << SPE) | (1 << MSTR);
    /*Set PE4(OE), PE5 (LE)*/
    DDRE |= (1 << DDE4) | (1 << DDE5);

    PORTB &= ~(1<<PB0);
    PORTE &= ~(1 << PORTE4); // OE
    // PORTE = PORTE | _BV(PE4) | _BV(PE5);
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

//Convert char array to int
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

//Increment time
// ISR(TIMER1_COMPA_vect) 
// {
//     minute++;

//     if(minute >= 60) {
//         minute = 0;
//         hour++;
//     }

//     if(hour >= 24) {
//         hour = 0;
//     }
// }

//Displays time when 'h' received from USART
void displayTime()
{
    char buffer[64];
    sprintf(buffer, "%u:%u", hour, minute);
    USART_puts(buffer);
}

//Changes time when received through USART
unsigned char usart_buffer[5];
void changeTime(unsigned char carac)
{
    //Fill USART buffer 
    int i = 1;
    int l = sizeof(usart_buffer)/sizeof(usart_buffer[0]);
    while(i < l) {
        usart_buffer[i-1] = usart_buffer[i];
        i++;
    }

    usart_buffer[l-1] = carac;

    //If it's a time, save it
    if(usart_buffer[2] == ':') {
        char h[2] = {usart_buffer[0], usart_buffer[1]};
        char m[2] = {usart_buffer[3], usart_buffer[4]};

        hour = atoi(h, 2);
        minute = atoi(m, 2);
    }
    else if(usart_buffer[0] == 'h' 
        && usart_buffer[1] == 'e'
        && usart_buffer[2] == 'l'
        && usart_buffer[3] == 'p') {
            USART_puts("\n\r\r############\n\r ### HELP ###\n\r############\n\r\rh : returns the time\n\rhh:mm : change time\n\rm0 : analog clock\n\rm1 : enchanted clock\n\rm2 : small digital clock\n\rm3 : big digital clock");
        }
    else if(usart_buffer[0] == 'h') {
        displayTime();
    }
    else if(usart_buffer[0] == 'm'){
        change_mode(usart_buffer[1]);
    }
}

void change_mode(carac){
    int i = 1;
    int l = sizeof(usart_buffer)/sizeof(usart_buffer[0]);
    while(i < l){
        usart_buffer[i-1] = usart_buffer[i];
        i++;
    }

    usart_buffer[l-1] = carac;
    if(usart_buffer[1] == '0'){
        mode = 0;
    }
    else if(usart_buffer[1] == '1'){
        mode = 1;
    }
    else if(usart_buffer[1] == '2'){
        mode = 2;
    }
    else if(usart_buffer[1] == '3'){
        mode = 3;
    }
}

ISR(USART0_RX_vect)
{
    unsigned char carac = UDR0;
        changeTime(carac);
}

//Initialize Hall effect timer
void Init_Hall_Timer() 
{
    //Set prescaler to 256
    TCCR3B |= _BV(CS32);

    //Set CTC mode
    TCCR3B |= _BV(WGM32);
}

void Init_Hall_Interrupt()
{
    //Disable hall sensor interrupt to setup it
    EIMSK &= ~_BV(INT0);

    //Set interrupt at falling edge
    EICRA |= ISC01;

    //Enable hall sensor interrupt
    EIMSK |= _BV(INT0);
}

volatile uint16_t turnTime = 0;
volatile uint16_t TabturnTime[5] = {0,0,0,0,0};
volatile int i = 0;

volatile int tour = 0;
volatile uint16_t first_int = 200;
volatile uint16_t second_int = 4600;


ISR(INT0_vect)
{
    uint16_t temp = TCNT3;
    TCNT3 = 0;
    // i++;
    // if(i%2 == 0){
    //     SPI_MasterTransmit(0x00,0xFF);
    // }
    // else{
    //     SPI_MasterTransmit(0xFF,0x00);
    // }
    if(temp < 2000){
        first_int = temp;
        TCNT3 = first_int;
    }

    else if((temp >=3000) && (temp <= 6000)){
        //TCNT3 = 0;
        second_int = temp;
        // char buffer[64];
        // sprintf(buffer, "%d\n\r", temp);
        // //sprintf(buffer, "\n speed:%f \n turnTime:%d time_now: %d", speed, turnTime, time_now);
        // USART_puts(buffer); 
    }
    turnTime = first_int/2 + second_int;
    tour = 0;
}

uint16_t leds(uint32_t deg)
{
    uint16_t cData = 0x0000;
   
    if(((hour%12)*(turnTime/12)  >= deg-30) && ((hour%12)*(turnTime/12) <= deg+30)){
        cData = 0x000F;
    }
    if((deg <=200)&&(deg >= turnTime-200)){
        if(hour%12 == 0){
            cData = 0x000F;
        }
        if(minute == 30){
            cData = 0x00FF;
        }
    }
    if((minute*(turnTime/60) >= deg-30)&&(minute*(turnTime/60) <= deg+30)){
        cData = 0x00FF;
    }
 
    
    
    return cData;
}

int Calc_deg(time_now)
{
    uint32_t deg=time_now;
    // if(time_now >=4600){
    //     TCNT3 = 0;
    // }

    deg = (deg + turnTime/2)%turnTime;
    //char buffer[64];
    // sprintf(buffer, "%d\n\r", time_now);
    // //sprintf(buffer, "\n speed:%f \n turnTime:%d time_now: %d", speed, turnTime, time_now);
    // USART_puts(buffer);
    return deg;
}

void clock(uint16_t leds){
    char up = (leds >> 8);
    char down = leds;
    SPI_MasterTransmit(up, down);
}

#include <stdbool.h>

bool digits[10][5][3] = {
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {0, 1, 0},
        {1, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    }
};

bool colon[5][2] = {
    {0, 0},
    {1, 1},
    {0, 0},
    {1, 1},
    {0, 0},
};

bool digits2[10][14][7] = {
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0},
        {1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    }
};

bool colon2[14][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {1, 1},
    {1, 1},
    {0, 0},
    {0, 0},
    {1, 1},
    {1, 1},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
};
//int hash = 60;

bool hourHand[4][3] = {
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
};

bool minuteHand[8][3] = {
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
};

bool ledStates[60][16];

void reinitArrayBool2() {
    volatile int i = 0;
    volatile int li = sizeof(ledStates[0])/sizeof(ledStates[0][0]);
    while(i < li) {
        volatile int j = 0;
        volatile int lj = sizeof(ledStates)/sizeof(ledStates[0][0]);
        while(j < lj) {
            ledStates[i][j] = 0;
            j++;
        }
        i++;
    }
}


void hourHandToLedStates() {
    volatile int li = 3;
    volatile int i = -1*li/2;
    volatile int lj = 4 - 1;
    volatile int j = 0;
    int k = 0;
    while(i <= li/2) {
        k = (hour%12)*60/12+i;
        if(k < 0)
            k = 60 + k;
        else if(k >= 60)
            k = k - 60;
        
        j = lj;
        while(j >= 0) {
            ledStates[k][j] |= hourHand[j][i+li/2];
        
            //printf("%d \n\r", j);
            j--;
        }
        i++;
    }
}

void minuteHandToLedStates() {
    volatile int li = 3;
    volatile int i = -1*li/2;
    volatile int lj = 8 - 1;
    volatile int j = 0;
    int k = 0;
    while(i <= li/2) {
        k = minute+i;
        if(k < 0)
            k = 60 + k;
        else if(k >= 60)
            k = k - 60;
        
        j = lj;
        while(j >= 0) {
            ledStates[k][j] |= minuteHand[j][i+li/2];
        
            //printf("%d \n\r", j);
            j--;
        }
        i++;
    }
}

int digitToLedStates(int digit, int a, int li, int lj) {
    volatile int i = 0;
    //volatile int li = sizeof(digits[digit][0])/sizeof(digits[digit][0][0]);
    //printf("li : %d", li);
    //volatile int li = 3;
    volatile int j = lj;
    //volatile int lj = sizeof(digits[digit])/sizeof(digits[digit][0][0]);
    //printf("lj : %d", lj);
    //volatile int lj = 14;
    while(i < li) {
        j = lj-1;
        while(j >= 0) {
            if(mode == 2){
                ledStates[a+i][lj-j+(16-lj)/2] = digits[digit][j][i];
            }
            else if(mode == 3){
                ledStates[a+i][lj-j+(16-lj)/2] = digits2[digit][j][i];
            }
            j--;
        }
        i++;
    }

    return((a+i+1)%60);
}

int colonToLedStates(int a, int li, int lj) {
    volatile int i = 0;
    volatile int j = 0;
    while(i < li) {
        j = 0;
        while(j < lj) {
            int k = j+(16-lj)/2+((16-lj)/2)%2;
            if(mode == 2){
                ledStates[a+i][k] = colon[j][i];
            }
            else if(mode == 3){
                ledStates[a+i][k] = colon2[j][i];
            }
            j++;
        }
        i++;
    }

    return((a+i+1)%60);
}


void hourToCurveLed() {
    //Prepare data
    reinitArrayBool2();

    int h1 = hour/10;
    int h2 = hour - h1*10;
    int m1 = minute/10;
    int m2 = minute - m1*10;
    int i = 0;
    if(mode == 1) {
        hourHandToLedStates();
        minuteHandToLedStates();
    }
    else if(mode == 2){
        //i = 0;
        i = 51;
        i = digitToLedStates(h1, i, 3, 5);
        i = digitToLedStates(h2, i, 3, 5);
        i = colonToLedStates(i, 2, 5);
        i = digitToLedStates(m1, i, 3, 5);
        i = digitToLedStates(m2, i, 3, 5);
    }
    else if(mode == 3){
        i = 43;
        i = digitToLedStates(h1, i, 7, 14);
        i = digitToLedStates(h2, i, 7, 14);
        i = colonToLedStates(i, 2, 14);
        i = digitToLedStates(m1, i, 7, 14);
        i = digitToLedStates(m2, i, 7, 14);
    }
    
}

volatile int lastHour = 0;
volatile int lastMinute = 0;

volatile char char1 = 0x0000;
volatile char char2 = 0x0000;

void displayCurveTime() {
    if(lastMinute != minute || lastHour != hour) {
        hourToCurveLed();
        lastHour = hour;
        lastMinute = minute;
    }

    if(TCNT3 >= (turnTime/60)*(tour+1)) {
        tour += 1;

        char1 = 0x0000;
        char2 = 0x0000;

        volatile int j = 0;
        while(j < 16) {
            if(j < 8 && ledStates[(tour+30)%60][j])
                char1 |= 1 << (j%8);
            else if(j >= 8 && ledStates[(tour+30)%60][j])
                char2 |= 1 << (j%8);
            j++;
        }
    }
    
    SPI_MasterTransmit(char2, char1);
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

void main()
{
    // Active et allume la broche PB5 (led)
    Init_Interrupt();
    USART_Init (MYUBRR);
    SPI_MasterInit();
    Init_Watch();
    Init_Hall_Timer();
    Init_Hall_Interrupt();
    //USART_Transmit('a');

    //hourToCurveLed();

    char buffer[64];
    while (1)
    {
        watch_tick();
        if(mode == 0){
            clock(leds(Calc_deg(TCNT3)));  
        }
        if(mode == 1){
            displayCurveTime();
        }
        else if(mode == 2){
            displayCurveTime();
        }
        else if(mode == 3) {
            displayCurveTime();
        }
    }
}

