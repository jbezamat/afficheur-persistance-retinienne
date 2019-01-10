#include "curved.h"
#include "global.h"
#include "SPI.h"

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

void reinitArrayBool2() 
{
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

void hourHandToLedStates(int hour) {
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
            j--;
        }
        i++;
    }
}

void minuteHandToLedStates(int minute) {
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
            j--;
        }
        i++;
    }
}

int digitToLedStates(int digit, int a, int li, int lj, bool display) {
    volatile int i = 0;
    volatile int j = lj;
    if(display) {
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
    }

    return((a+li+1)%60);
}

int colonToLedStates(int a, int li, int lj, bool display) {
    volatile int i = 0;
    volatile int j = 0;
    if(display) {
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
    }

    return((a+li+1)%60);
}

int lastH1 = -1;
int lastH2 = -1;
int lastM1 = -1;
int lastM2 = -1;

void hourToCurveLed(int hour, int minute) {

    int h1 = hour/10;
    int h2 = hour - h1*10;
    int m1 = minute/10;
    int m2 = minute - m1*10;
    int i = 0;
    if(mode == 1) {
        reinitArrayBool2();
        hourHandToLedStates(hour);
        minuteHandToLedStates(minute);
    }
    else if(mode == 2) {
        if(changedMode)
            reinitArrayBool2();

        //i = 0;
        i = 51;

        // if(lastH1 != h1 || changedMode) {
        //     digitToLedStates(h1, i, 3, 5);
        // }
        i = digitToLedStates(h1, i, 3, 5, lastH1 != h1 || changedMode);
        // i += 4;
        // i = i%60;

        // if(lastH2 != h2 || changedMode) {
        //     digitToLedStates(h2, i, 3, 5);
        // }
        // i += 4;
        // i = i%60;
        i = digitToLedStates(h2, i, 3, 5, lastH2 != h2 || changedMode);

        // if(changedMode)
        //     colonToLedStates(i, 2, 5);
        // i += 3;
        // i = i%60;
        i = colonToLedStates(i, 2, 5, changedMode);

        // if(lastM1 != m1 || changedMode) {
        //     digitToLedStates(m1, i, 3, 5);
        // }
        // i += 4;
        // i = i%60;
        i = digitToLedStates(m1, i, 3, 5, lastM1 != m1 || changedMode);

        // if(lastM2 != m2 || changedMode) {
        //     digitToLedStates(m2, i, 3, 5);
        // }
        // i += 4;
        // i = i%60;
        i = digitToLedStates(m2, i, 3, 5, lastM2 != m2 || changedMode);
    }
    else if(mode == 3) {
        if(changedMode)
            reinitArrayBool2();

        i = 43;
        i = i%60;

        // if(lastH1 != h1 || changedMode) {
        //     digitToLedStates(h1, i, 7, 14);
        // }
        // i += 8;
        // i = i%60;
        i = digitToLedStates(h1, i, 7, 14, lastH1 != h1 || changedMode);

        // if(lastH2 != h2 || changedMode) {
        //     digitToLedStates(h2, i, 7, 14);
        // }
        // i += 8;
        // i = i%60;
        i = digitToLedStates(h2, i, 7, 14, lastH2 != h2 || changedMode);

        // if(changedMode)
        //     colonToLedStates(i, 2, 14);
        // i += 3;
        // i = i%60;
        i = colonToLedStates(i, 2, 14, changedMode);

        // if(lastM1 != m1 || changedMode) {
        //     digitToLedStates(m1, i, 7, 14);
        // }
        // i += 8;
        // i = i%60;
        i = digitToLedStates(m1, i, 7, 14, lastM1 != m1 || changedMode);

        // if(lastM2 != m2 || changedMode) {
        //     digitToLedStates(m2, i, 7, 14);
        // }
        // i += 8;
        // i = i%60;
        i = digitToLedStates(m2, i, 7, 14, lastM2 != m2 || changedMode);

        // i = digitToLedStates(h1, i, 7, 14);
        // i = digitToLedStates(h2, i, 7, 14);
        // i = colonToLedStates(i, 2, 14);
        // i = digitToLedStates(m1, i, 7, 14);
        // i = digitToLedStates(m2, i, 7, 14);
    }

    lastH1 = h1;
    lastH2 = h2;
    lastM1 = m1;
    lastM2 = m2;
    changedMode = false;
}

void displayCurveTime(int hour, int minute) {
    if(lastMinute != minute || lastHour != hour) {
        hourToCurveLed(hour, minute);
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