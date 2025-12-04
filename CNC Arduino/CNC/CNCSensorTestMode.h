#ifndef SENSOR_TEST_MODE_h
#define SENSOR_TEST_MODE_h

#include <Arduino.h>
#include <NonBlockingSequence.h>
#include "CNCsensors.h"
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
         NonBlockingSequence SidePlateSequence;
         NonBlockingSequence FoamFixationSequence;
         NonBlockingSequence MotionHead1Sequence;
         NonBlockingSequence MotionHead2Sequence;
         NonBlockingSequence MotionFixation1Sequence;
         NonBlockingSequence MotionFixation2Sequence;

         SensorTestMode()=delete;
    };
#endif
