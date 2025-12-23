#ifndef FORK_SENSOR_TEST_MODE_H
#define FORK_SENSOR_TEST_MODE_H

#include <Arduino.h>
#include <NonBlockingSequence.h>
#include "ForkSensors.h"
#include "Communication.h"
#include "FR_Variables.h"
#include "Shared_Functions.h" 


/*
 * @brief Declares a step function that reads a sensor and sends the value
 * @param StepName Name of the function to create
 * @param SensorName Sensor object with readFiltered function
 * @param SensorComMacro Numeric command macro for communication
 */


    class SensorTestMode{
         static void begin();
         static void loop();

         private:
         NonBlockingSequence IRFoamSensorSequence;
         NonBlockingSequence Fork2NailsSequence;
         NonBlockingSequence Fork3NailsSequence;
         NonBlockingSequence CNCGateFixationSequence;
         NonBlockingSequence MotionFork1Sequence;
         NonBlockingSequence MotionFork2Sequence;
         NonBlockingSequence MotionFork3Sequence;
         NonBlockingSequence MotionCNCGateSequence;

         SensorTestMode()=delete;
    };
#endif
