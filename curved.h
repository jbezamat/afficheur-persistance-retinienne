#ifndef __CURVED_H__
#define __CURVED_H__

#include <stdbool.h>
#include <avr/io.h>

uint32_t Calc_deg(int time_now);
void reinitArrayBool2();
char displaySeconds(char up, int minute);
void hourHandToLedStates(int hour);
void minuteHandToLedStates(int minute);
int digitToLedStates(int digit, int a, int li, int lj, bool display);
int digitToLedStatesRect(int digit, int a, int li, int lj, bool display);
int colonToLedStates(int a, int li, int lj, bool display);
void hourToCurveLed(int hour, int minute);
void displayCurveTime(int hour, int minute);


#endif