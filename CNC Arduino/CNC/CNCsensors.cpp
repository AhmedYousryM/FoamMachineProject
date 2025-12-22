#include "CNCsensors.h"
#include "SensorFilterDriver.h"

// Define all sensor objects with their parameters
// * checked  # need editing
SensorFilter SidePlateSensor1(42, true);  // Pin D42#,  reversed
SensorFilter SidePlateSensor2(43, true);   // Pin D43*, reversed
SensorFilter SidePlateSensor3(44, true);  // Pin D44#,  reversed
SensorFilter SidePlateSensor4(48, true);  // Pin D48#,  reversed
SensorFilter SidePlateSensor5(49, true);   // Pin D49#, reversed
SensorFilter FixationG1LimitSensor(27, true);  // Pin D27#,  reversed
SensorFilter FixationG2LimitSensor(28, true);  // Pin D28#,  reversed

SensorFilter HeadG1ZUpSensor(2, true);   // Pin D2*, reversed
SensorFilter HeadG1ZDownSensor(3, true);  // Pin D3*,  reversed
SensorFilter HeadG2ZUpSensor(A13, true);  // Pin A13*,  reversed
SensorFilter HeadG2ZDownSensor(12, true);   // Pin D12*, reversed

SensorFilter FrameFixG1YSensor(10, true);  // Pin D10#,  reversed
SensorFilter FrameFixG2YSensor(11, true);  // Pin D11#,  reversed
SensorFilter FixHeadG1YSensor(A11, true);   // Pin A11#, reversed
SensorFilter FixHeadG2YSensor(A12, true);  // Pin A12#,  reversed
SensorFilter HeadFrameG1YSensor1(A14, true);  // Pin A14#,  reversed
SensorFilter HeadFrameG1YSensor2(A5, true);   // Pin A5#, reversed
SensorFilter HeadFrameG2YSensor1(A15, true);  // Pin A15#,  reversed
SensorFilter HeadFrameG2YSensor2(A6, true);  // Pin A6#,  reversed

SensorFilter FrameG1XSensor(A9, true);   // Pin A9#, reversed
SensorFilter G1G2XSensor(A8, true);  // Pin A8*,  reversed
SensorFilter G2FrameXSensor(A10, true);  // Pin A10#,  reversed
// Add more as needed


SensorFilter pushButtonUp1(36 , false);  // Pin D36*,  reversed
SensorFilter pushButtonDown1(37 , false); // Pin D37*,  reversed
SensorFilter pushButtonZero1(38 , true); // Pin D38*,  reversed

SensorFilter pushButtonUp2(39 , false); // Pin D39*,  reversed
SensorFilter pushButtonDown2(40 , false); // Pin D40*,  reversed
SensorFilter pushButtonZero2(41 , true); // Pin D41*,  reversed
SensorFilter ErrorPin(20,false);