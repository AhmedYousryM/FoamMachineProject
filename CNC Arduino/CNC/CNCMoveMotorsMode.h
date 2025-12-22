#ifndef MOVE_MOTORs_MODE_h
#define MOVE_MOTORs_MODE_h

#include <Arduino.h>
#include "CNCSensorTestMode.h"
#include <NonBlockingSequence.h>
#include "CNCMotors.h"
#include "Communication.h"
extern SensorTestMode SensorTest;

class MoveMotorsMode{
        public:
        void begin();
        void loop();
        MoveMotorsMode(){};
        private:
        void MoveHead1();
        void MoveHead2();
        void MoveFixation1();
        void MoveFixation2();
        void moveDir(Motor m, int distance);

        void GRBL_MoveSpeed(GRBL_Control head, char axis, float dir, float minSpeed, float maxSpeed);
        void MotorSpeed(Motor motor, float dir, float minSpeed, float maxSpeed);
         
};
extern MoveMotorsMode MoveMotors;
#endif
