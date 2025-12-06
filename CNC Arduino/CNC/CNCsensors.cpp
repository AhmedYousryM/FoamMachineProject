#include "CNCsensors.h"
#include "SensorFilterDriver.h"

// Define all sensor objects with their parameters
SensorFilter SidePlateSensor1(42, true);  // Pin 2,  reversed
SensorFilter SidePlateSensor2(43, true);   // Pin 3, reversed
SensorFilter SidePlateSensor3(44, true);  // Pin 4,  reversed
SensorFilter SidePlateSensor4(48, true);  // Pin 2,  reversed
SensorFilter SidePlateSensor5(49, true);   // Pin 3, reversed
SensorFilter FixationG1LimitSensor(27, true);  // Pin 4,  reversed
SensorFilter FixationG2LimitSensor(28, true);  // Pin 2,  reversed

SensorFilter HeadG1ZUpSensor(2, true);   // Pin 3, reversed
SensorFilter HeadG1ZDownSensor(3, true);  // Pin 4,  reversed
SensorFilter HeadG2ZUpSensor(A13, true);  // Pin 2,  reversed
SensorFilter HeadG2ZDownSensor(12, true);   // Pin 3, reversed

SensorFilter FrameFixG1YSensor(10, true);  // Pin 4,  reversed
SensorFilter FrameFixG2YSensor(11, true);  // Pin 2,  reversed
SensorFilter FixHeadG1YSensor(A11, true);   // Pin 3, reversed
SensorFilter FixHeadG2YSensor(A12, true);  // Pin 4,  reversed
SensorFilter HeadFrameG1YSensor1(A14, true);  // Pin 2,  reversed
SensorFilter HeadFrameG1YSensor2(A5, true);   // Pin 3, reversed
SensorFilter HeadFrameG2YSensor1(A15, true);  // Pin 4,  reversed
SensorFilter HeadFrameG2YSensor2(A6, true);  // Pin 2,  reversed

SensorFilter FrameG1XSensor(A9, true);   // Pin 3, reversed
SensorFilter G1G2XSensor(A8, true);  // Pin 4,  reversed
SensorFilter G2FrameXSensor(A10, true);  // Pin 2,  reversed
// Add more as needed


SensorFilter pushButtonUp1(36 , false);
SensorFilter pushButtonDown1(37 , false);
SensorFilter pushButtonZero1(38 , true);

SensorFilter pushButtonUp2(39 , false);
SensorFilter pushButtonDown2(40 , false);
SensorFilter pushButtonZero2(41 , true);