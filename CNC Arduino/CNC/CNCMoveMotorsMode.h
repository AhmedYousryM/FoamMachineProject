#ifndef MOVE_MOTORs_MODE_h
#define MOVE_MOTORs_MODE_h

#include <Arduino.h>
#include "CNCSensorTestMode.h"
#include <NonBlockingSequence.h>
#include "CNCMotors.h"





    class MoveMotorsMode{
         static void begin();
         static void loop();

         private:
         MoveMotorsMode()=delete;
         
    };
#endif
