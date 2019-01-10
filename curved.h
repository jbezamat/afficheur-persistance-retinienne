#ifndef __CURVED_H__
#define __CURVED_H__

#include <stdbool.h>

void reinitArrayBool2();
void hourHandToLedStates(int hour);
void minuteHandToLedStates(int minute);
int digitToLedStates(int digit, int a, int li, int lj, bool display);
int colonToLedStates(int a, int li, int lj, bool display);
void hourToCurveLed(int hour, int minute);
void displayCurveTime(int hour, int minute);

bool ledStates[60][16];

#endif