//Watch variables
int hour = 0;
int minute = 0;

//Initialize watch timer
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
        //Check if it's a digit
        if(s[l-i] < '0' || s[l-i] > '9')
            return val;

        //ASCII to int
        int u = s[l-i] - '0';
        
        //Power
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
    while(i < l){
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
}