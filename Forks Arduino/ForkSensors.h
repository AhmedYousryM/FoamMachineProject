#ifndef FORK_SENSORS_H
#define FORK_SENSORS_H

#include "SensorFilterDriver.h"

// Declare all sensor objects as extern (defined in sensors.cpp)

extern SensorFilter IRCNCInletSensor;
extern SensorFilter IRCNCExitSensor;

extern SensorFilter Fork2Nail1Sensor;
extern SensorFilter Fork2Nail2Sensor;
extern SensorFilter Fork2Nail3Sensor;
extern SensorFilter Fork3Nail1Sensor;
extern SensorFilter Fork3Nail2Sensor;
extern SensorFilter Fork3Nail3Sensor;
extern SensorFilter CNCGateFixationSensor1;
extern SensorFilter CNCGateFixationSensor2;
extern SensorFilter CNCGateFixationSensor3;

extern SensorFilter FrameFork1XSensor;
extern SensorFilter Fork1Fork2XSensor;
extern SensorFilter Fork2Fork3XSensor;
extern SensorFilter Fork3FrameXSensor;

extern SensorFilter Fork1ZUpSensor;
extern SensorFilter Fork1ZDnSensor;
extern SensorFilter Fork2ZUpSensor;
extern SensorFilter Fork2ZDnSensor;
extern SensorFilter Fork3ZUpSensor;
extern SensorFilter Fork3ZDnSensor;
extern SensorFilter CNCGateZUpSensor;
extern SensorFilter CNCGateZDnSensor;

#endif
