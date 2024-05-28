#pragma once
#ifndef TIMER_h
#define TIMER_h

/* This liberary uses board mega 2560 timer 2*/

#include "Arduino.h"

class TimerClass{
    public:

    /* any function in the form void func(void)
    * can be entered
    */
    void Calling_func(void (*call_func)());

    // the maximum possilbe periodic time is 16,384
    // Set the timer periodic time to the closest possible 
    // determine value for OCR and prescaler
    void SetTime_Micros(unsigned long micros);

    // start the count and execute only once
    void ExecuteOneTime();

    void Repeat();

    void Stop();

    // stop the counter at this point
    void pause();

    // continue the timer after using pause() function
    void resume();

};

extern TimerClass Timer;

#endif

