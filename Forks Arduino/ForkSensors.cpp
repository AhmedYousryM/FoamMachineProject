#include "ForkSensors.h"
#include "SensorFilterDriver.h"

// Define all sensor objects with their parameters
SensorFilter IRCNCInletSensor(22, true);
SensorFilter IRCNCExitSensor(26, true);

SensorFilter Fork2Nail1Sensor(23, true);
SensorFilter Fork2Nail2Sensor(27, true);
SensorFilter Fork2Nail3Sensor(31, true);
SensorFilter Fork3Nail1Sensor(35, true);
SensorFilter Fork3Nail2Sensor(39, true);
SensorFilter Fork3Nail3Sensor(43, true);
SensorFilter CNCGateFixationSensor1(47, true);
SensorFilter CNCGateFixationSensor2(51, true);
SensorFilter CNCGateFixationSensor3(A2, true);

SensorFilter FrameFork1XSensor(30, true);
SensorFilter Fork1Fork2XSensor(34, true);
SensorFilter Fork2Fork3XSensor(38, true);
SensorFilter Fork3FrameXSensor(42, true);

SensorFilter Fork1ZUpSensor(A6, true);
SensorFilter Fork1ZDnSensor(46, true);
SensorFilter Fork2ZUpSensor(A10, true);
SensorFilter Fork2ZDnSensor(50, true);
SensorFilter Fork3ZUpSensor(A14, true);
SensorFilter Fork3ZDnSensor(A7, true);
SensorFilter CNCGateZUpSensor(A11, true);
SensorFilter CNCGateZDnSensor(A15, true);
// Add more as needed
