#include "interface.h"

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

void displayTime()
{
    char buffer[64];
    sprintf(buffer, "%u:%u", hour, minute);
    USART_puts(buffer);
}


void change_mode(unsigned char carac)
{
    if(carac == '0'){
        mode = 0;
    }
    else if(carac== '1'){
        mode = 1;
    }
    else if(carac == '2'){
        mode = 2;
    }
    else if(carac == '3'){
        mode = 3;
    }
    changedMode = true;
}

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
    else if(usart_buffer[1] == 'h' 
        && usart_buffer[2] == 'e'
        && usart_buffer[3] == 'l'
        && usart_buffer[4] == 'p') {
            USART_puts("\n\r\r############\n\r ### HELP ###\n\r############\n\r\rh : returns the time\n\rhh:mm : change time\n\rm0 : analog clock\n\rm1 : enchanted clock\n\rm2 : small digital clock\n\rm3 : big digital clock");
        }
    else if(usart_buffer[4] == 'h') {
        displayTime();
    }
    else if(usart_buffer[3] == 'm'){
        change_mode(usart_buffer[4]);
    }
}