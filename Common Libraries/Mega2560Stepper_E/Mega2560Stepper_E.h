#ifndef Mega2560Stepper_h
#define Mega2560Stepper_h

#include "Arduino.h"

class Pulse_Generator{

    public:

    private:

};

class Mega2560Stepper{

    public:

        // this construction needs other construction to define arduino Pins
        Mega2560Stepper();

        // pin1 is direction pin and pin2 in enable pin
        // step pin is not free to chose. It is pwm pin for the attached timer
        // this liberary is applied for Atmega_2560 with 4 16-bit-timers
        // Timers used are:                  1   ,   3   ,   4   &   5
        // stepper pin for each timer :      12  ,   2   ,   7   &   45
        void Begin(byte timer , byte pin1 , byte pin2);

        // this function only sets the target point. And the motor will not move
        // the run() function is needed to achieve the specified target
        void    moveTo(long absolute);

        // increment or decrement the target point by relative change
        void    move(long relative);

        // called as frequent as possible to update the motor speed to achieve the requied command
        // this function is written in loop() 
        // if the code in loop() is stuck in heavy compurtation and the run() function called with large time interval
        // the possiblity of the motor to pass over the target position without stopping at it.
        boolean run();

        void    setMaxSpeed(long speed);

        void    setAcceleration(int acceleration);

        float   speed();

        long    distanceToGo();

        long    targetPosition();

        long    currentPosition();

        void    setCurrentPosition(long position);

        // Stop with the specified acceleration/decelaration and ignore the required target position
        // also stop() function update the system speed without affecting the target point
        // either one of two function must called:
        // 1) run() : the motor will move to achieve the target point
        // 2) stop() : the motor will stop as close as possible until the next run() is called
        // you can not call both function and expect the system to stop
        void    stop();

        // set smallest width for the pulsewidth that can be sensed by the Hardware Stepper Driver
        //void    setMinPulseWidth(unsigned int minWidth);

        // return true only if reached the target point and stop there at zero speed
        bool    reachTarget();

        // The motor initially has positive direction to rotate
        // This function call invert the default direction to the oppisite
        void    InvertDirectionPin();

        void    CutMotion();

        enum ModesOfOperation { accelerating1stTime ,
                                accelerating ,                             
                                approachingTarget1stTime , 
                                approachingTarget ,
                                atTarget1stTime ,
                                atTarget ,
                                Stopping ,
                                stopped1stTime ,
                                stopped2ndTime ,
                                stopped ,
                                motionCut };

        enum ModesOfOperation myMode = atTarget;

    private:

        virtual void    disableOutputs();

        virtual void    enableOutputs();

        void           computeNewSpeed();

        byte           _timer;

        byte           _DirPin;

        byte           _EnablePin;

        long           _targetPos = 0;

        long           _currentPos = 0;

        volatile float           _speed = 0.0;

        bool           _direction = 1;

        uint32_t       _maxSpeed = 100;

        float          _acceleration_per_s_ms = 0.02;

        unsigned int   _minPulseWidth = 3;

        bool           ExecuteSpeed();

        void           ActivateStopInt();

        void           DeactivateStopInt();

        uint32_t       DeltaTime = 0;

        uint32_t       AveragedDeltaTime = 70;

        uint32_t       Prev_Update_Time = 0;

        bool           FutureUpdate();

        void           UpdateStep();

        void           SetMinMaxDeltaTime(unsigned int N1 , unsigned int N2);

        unsigned int   MinDeltaTime = 50;

        unsigned int   MaxDeltaTime = 100;

        void           compute_dec_const();

        float C1 = _acceleration_per_s_ms * MinDeltaTime / 2 ;     //a * dt /2
        float C2 = 2 / (C1*C1) * _acceleration_per_s_ms * 1000;   
        float C3 = _acceleration_per_s_ms * MaxDeltaTime;      //  a * dt
        float C4 = _acceleration_per_s_ms * MaxDeltaTime * MaxDeltaTime /1000 /2 ;

        void            StoppingDo();

        // this variable is important and represent the enable pin state
        // the motor is disenabled when there is no motion to save motor power and generated heat 
        //
        // the motor is set to (enabled == 0) at two conditions:
        // 1) the stop() function is called and the motor is already stopped at zero speed
        // 2) the system achieve the requied target point and wait
        //
        // the motor is set to (enabled == 1) at only one conditions:
        // *) the run() function is called and the target point is not achieved
        bool           enabled = 0;

};

class two_Mega2560Stepper{

    public:
    two_Mega2560Stepper(uint8_t timer , char c1 , uint8_t pin1 , uint8_t pin2, char c2  , uint8_t pin3 , uint8_t pin4);

    private:


};

#endif


/*
switch (myMode){
    case accelerating1stTime :
    break;
    case accelerating :
    break;                            
    case approachingTarget1stTime :
    break;
    case approachingTarget :
    break;
    case atTarget1stTime :
    break;
    case atTarget :
    break;
    case Stopping :
    break;
    case stopped1st :
    break;
    case stopped :
    break;
}
*/