#ifndef FIXATION_MAIN
#define FIXATION_MAIN

#include "CNCMotors.h"
#include <NonBlockingSequence.h>
#include "CNCsensors.h"
#include "CNCConstantMacros.h"
#include "Communication.h"
#define MAX_SPEED_HOMING_1 1
#define MAX_SPEED_HOMING_2 .1
#define HIGH_SPEED 50
#define GAB 20
#define FIRST_SAFE_POS 3
#define BETWEEN_17_WOOD 75
#define SIDEWOOD_FIRST_SENSOR 1713.4
#define SIDEWOOD_BETWEEN_SENSOR 240
#define SIDEWOOD_BETWEEN_SENSOR_LAST_ONE 230
  

class Fixation_main{
    public:
    Fixation_main(Motor motor, char Fixation_num);
    bool begin();
    float safe_pos_before(float pos);
    bool homing();
    void before_Fixation(float foam_start,float foam_end);
    bool fixating();
    void RestartFixating();
    float Measure_FoamYWidth = 0;
    bool FoamYWidth_is_Correct = true ;
    bool FixatingRestart(){
        Fixating.Restart();
        FoamYWidth_is_Correct = true ;
    }
    bool goingOut();
    bool goingIn();
    bool GoingInSafe();
    bool DoingLubrication();
    void RestartLubrication();
    bool MotionReset=true;
    bool ReleaseFixation(){
        return GoingInSafe();
    }

    private:
    Motor _motor;
    char _num;
    float Ideal_FoamYWidth;

    ClassNonBlockingSequence<Fixation_main> Homing;
    ClassNonBlockingSequence<Fixation_main> Fixating;
    ClassNonBlockingSequence<Fixation_main> GoingIn;
    ClassNonBlockingSequence<Fixation_main> GoingOut;
    ClassNonBlockingSequence<Fixation_main> Lubricating;

    bool Homing_MoveLongRun();
    bool Homing_Stop();
    bool Homing_GoReverseRun();
    bool Homing_MoveShortRun();
    bool Homing_End();

    bool beforeFix();
    bool approachNearFoam();
    bool touchFoam();
    bool afterTouch();
    bool move2mm();
    bool fixationEnd();
    bool sidePlateSensorBool[5] = {false, false, false, false, false};
    SensorFilter sidePlateSensor[5] = {SidePlateSensor1, SidePlateSensor2, SidePlateSensor3, SidePlateSensor4, SidePlateSensor5};

    bool oneSensorRead();
    bool FixationLimitSensorRead();
    bool Acceptable_FoamYWidth();


};

extern Fixation_main Fix_main1;
extern Fixation_main Fix_main2;

#endif