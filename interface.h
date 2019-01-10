#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "global.h"
#include "USART.h"
#include <stdio.h>

int atoi(char* s, int l);
void displayTime();
void change_mode(unsigned char carac);
void changeTime(unsigned char carac);

#endif