#ifndef FIXATION_MAIN
#define FIXATION_MAIN

#include "CNCMotors.h"
#include <NonBlockingSequence.h>
#include "CNCsensors.h"

#define MAX_SPEED_HOMING_1 1
#define MAX_SPEED_HOMING_2 .1
#define HIGH_SPEED 50
#define GAB 20
#define FIRST_SAFE_POSs 3
#define BETWEEN_17_WOOD 75
#define SIDEWOOD_FIRST_SENSOR 1713.4
#define SIDEWOOD_BETWEEN_SENSOR 240
#define SIDEWOOD_BETWEEN_SENSOR_LAST_ONE 230



class Fixation_main{
    Fixation_main(Motor motor, char Fixation_num);
    bool begin();

    bool Homing();
    void before_Fixation(float foam_start,float foam_end);
    bool Fixating();
    float Measure_FoamYWidth = 0;
    bool FoamYWidth_is_Correct = true ;
    bool FixatingReset(){
        Fixating.Reset();
        FoamYWidth_is_Correct = true ;
    }
    bool GoingOut();
    bool GoingInSafe();
    bool DoingLubrication();

    bool ReleaseFixation(){
        return GoingInSafe();
    }

    private:
    Motor _motor;
    char _num;
    float Ideal_FoamYWidth;

    ClassNonBlockingSequence<Fixation_main> Homing;
    ClassNonBlockingSequence<Fixation_main> Fixating;
    ClassNonBlockingSequence<Fixation_main> Lubricating;

    bool Homing_MoveLongRun();
    bool Homing_Stop();
    bool Homing_GoReverseRun();
    bool Homing_MoveShortRun();
    bool Homing_End();

    bool approachNearFoam();
    bool touchFoam();
    bool afterTouch();

    bool sidePlateSensorBool[] = {false, false, false, false, false};
    SensorFilter SidePlateSensor[] = {SidePlateSensor1, SidePlateSensor2, SidePlateSensor3, SidePlateSensor4, SidePlateSensor5};

    bool oneSensorRead();
    bool FixationLimitSensorRead();
    bool Acceptable_FoamYWidth();


}

extern Fixation_main Fix_main1;
extern Fixation_main Fix_main2;

#endif