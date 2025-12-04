#ifndef SENSORS_H
#define SENSORS_H

#include "SensorFilterDriver.h"

// Declare all sensor objects as extern (defined in sensors.cpp)

extern SensorFilter SidePlateSensor1;
extern SensorFilter SidePlateSensor2;
extern SensorFilter SidePlateSensor3;
extern SensorFilter SidePlateSensor4;
extern SensorFilter SidePlateSensor5;
extern SensorFilter FixationG1LimitSensor;
extern SensorFilter FixationG2LimitSensor;

extern SensorFilter HeadG1ZUpSensor;
extern SensorFilter HeadG1ZDownSensor;
extern SensorFilter HeadG2ZUpSensor;
extern SensorFilter HeadG2ZDownSensor;

extern SensorFilter FrameFixG1YSensor;
extern SensorFilter FrameFixG2YSensor;
extern SensorFilter FixHeadG1YSensor;
extern SensorFilter FixHeadG2YSensor;
extern SensorFilter HeadFrameG1YSensor1;
extern SensorFilter HeadFrameG1YSensor2;
extern SensorFilter HeadFrameG2YSensor1;
extern SensorFilter HeadFrameG2YSensor2;

extern SensorFilter FrameG1XSensor;
extern SensorFilter G1G2XSensor;
extern SensorFilter G2FrameXSensor;

extern SensorFilter ErrorPin;
/*
extern SensorFilter FoamSensor[] = {SidePlateSensor1,SidePlateSensor2,SidePlateSensor3,SidePlateSensor4,SidePlateSensor5,
    FixationG1LimitSensor,FixationG2LimitSensor};

extern SensorFilter MotionSensor[] = {HeadG1ZUpSensor,HeadG1ZDownSensor,HeadG2ZUpSensor,HeadG2ZDownSensor,
FrameFixG1YSensor,FrameFixG2YSensor,FixHeadG1YSensor,FixHeadG2YSensor,HeadFrameG1YSensor1,
HeadFrameG1YSensor2,HeadFrameG2YSensor1,HeadFrameG2YSensor2,FrameG1XSensor,G1G2XSensor,G2FrameXSensor
};
*/


// Add more as needed

#endif
