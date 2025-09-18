#ifndef MOTOR_h
#define MOTOR_h



#include <Arduino.h>

#define LIBRARY_NAME Mega2560Stepper
#include <Mega2560Stepper.h>


    class Motor: public LIBRARY_NAME{

       protected:
       float pulse_per_mm;

       public:
       // this construction needs other construction to define arduino Pins
       Motor():LIBRARY_NAME();

       // pin1 is direction pin and pin2 in enable pin
        // step pin is not free to chose. It is pwm pin for the attached timer
        // this liberary is applied for Atmega_2560 with 4 16-bit-timers
        // Timers used are:                  1   ,   3   ,   4   &   5
        // stepper pin for each timer :      12  ,   2   ,   7   &   45
        Motor(byte timer , byte pin1 , byte pin2, float ppmm) : pulse_per_mm(ppmm){
            this.LIBRARY_NAME(byte timer , byte pin1 , byte pin2);
        }

        void moveTo(float mm_pos){
            this.LIBRARY_NAME::moveTo( round( mm_pos * pulse_per_mm ) );
        }
        
        void move(float mm_pos){
            this.LIBRARY_NAME::move( round( mm_pos * pulse_per_mm ) );
        }

        void    setMaxSpeed(float mm_per_second){
          this.LIBRARY_NAME::setMaxSpeed( round( mm_per_second * pulse_per_mm ) );
        }

        void    setAcceleration(float mm_per_sec_sq){
            this.LIBRARY_NAME::move( setAcceleration( mm_pos * pulse_per_mm ) );
        }

        float   speed(){
            return this.LIBRARY_NAME::speed()/pulse_per_mm;
        }

        float    distanceToGo(){
            return this.LIBRARY_NAME::distanceToGo() / pulse_per_mm;
        }

        float    targetPosition(){
            return this.LIBRARY_NAME::targetPosition() / pulse_per_mm;
        }

        float    currentPosition(){
            return this.LIBRARY_NAME::currentPosition() / pulse_per_mm;
        }

        void    setCurrentPosition(float mm_pos){
            this.LIBRARY_NAME::setCurrentPosition( round( mm_pos * pulse_per_mm ) );
        }



    };
#endif
