#include <Timer.h>

     void (*my_func)();

     const int Clock_prescaler[7] = { 1, 8, 32, 64, 128, 256, 1024};

     byte prescaler_item = 1;

     unsigned long OCR = 1;

void TimerClass::Calling_func( void (*call_func)() ){
    my_func = call_func;
}

void TimerClass::SetTime_Micros(unsigned long micros){
    //set timer2 interrupt at 8kHz
    TCCR2A = 0;// set entire TCCR2A register to 0
    TCCR2B = 0;// same for TCCR2B
    TCNT2  = 0;//initialize counter value to 0
    // turn on CTC mode
    bitSet(TCCR2A,WGM21);
    // chose prescaler
     prescaler_item = 1;
    for(byte i = 0 ; i<=6 ; i++){
        OCR = round( micros * 16.0 / Clock_prescaler[i] )   - 1 ;
        if (OCR > 255){
        prescaler_item++;
        continue ;
        }else{
        break ;
        }
    }
    OCR2A  = OCR;
    OCR2B  = OCR;
    TCCR2B = (TCCR2B & 0b11111000) | prescaler_item;
}

void TimerClass::ExecuteOneTime(){
    TCNT2  = 0;
    bitSet(TIFR2,OCF2A);
    bitSet(TIMSK2,OCIE2A);
}

void TimerClass::Repeat(){
    TCNT2  = 0;
    bitSet(TIFR2,OCF2B);
    bitSet(TIMSK2,OCIE2B);
}

void TimerClass::Stop(){
    bitClear(TIMSK2,OCIE2B);
}

ISR(TIMER2_COMPA_vect){
    // call the required function
    my_func();

    // disable timer compare interrupt
    bitClear(TIMSK2,OCIE2A);
}

ISR(TIMER2_COMPB_vect){
    // call the required function
    my_func();
}

void TimerClass::pause(){
    TCCR2B = (TCCR2B & 0b11111000) | 0;
}

void TimerClass::resume(){
    TCCR2B = (TCCR2B & 0b11111000) | prescaler_item;
}