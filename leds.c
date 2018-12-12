char leds(int deg)
{
    char cData = 0x00;
    if((hour*15 == deg){
        cData = 0xFF;
    }
    else if((minute*6 == deg){
        cData = 0xF0;
    }
    return cData;
}

int Calc_deg(time_total,time_now)
{
    speed=360/time_total;
    deg=speed*time_now;

    return int(deg);
}