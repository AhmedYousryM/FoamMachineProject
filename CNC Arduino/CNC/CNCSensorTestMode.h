#ifndef SENSOR_TEST_MODE_h
#define SENSOR_TEST_MODE_h

#include <Arduino.h>
#include <NonBlockingSequence.h>
#include "CNCsensors.h"
#include "Communication.h"
#include "CNCConstantMacros.h"
/*
 * @brief Declares a step function that reads a sensor and sends the value
 * @param StepName Name of the function to create
 * @param SensorName Sensor object with readFiltered function
 * @param SensorComMacro Numeric command macro for communication
 */
     #define DECLARE_STEP(StepName, SensorName, SensorComMacro) \
          static bool StepName() { \
          SetVar(CRVariable,SensorComMacro,SensorName.readFiltered()); \
          return true;\
          }
     
    class SensorTestMode{
     //communication.Send(SensorComMacro);    
     public:
         SensorTestMode(){};
         void begin();
         void loop();
          
         private:
         static bool ReadyToSendStep();     
          static bool FoamFixChange();
          static bool MotionHead1Change();
            static bool MotionHead2Change();
        static bool MotionFix1Change();
        static bool MotionFix2Change();
        static bool SPchange();
            NonBlockingSequence SidePlateSequence;
         
         NonBlockingSequence FoamFixationSequence;
         NonBlockingSequence MotionHead1Sequence;
         NonBlockingSequence MotionHead2Sequence;
         NonBlockingSequence MotionFixation1Sequence;
         NonBlockingSequence MotionFixation2Sequence;

         DECLARE_STEP(SP1Step,SidePlateSensor1,SIDE_PLATE_LM_1_SENSOR)
        DECLARE_STEP(SP2Step,SidePlateSensor2,SIDE_PLATE_LM_2_SENSOR)
        DECLARE_STEP(SP3Step,SidePlateSensor3,SIDE_PLATE_LM_3_SENSOR)
        DECLARE_STEP(SP4Step,SidePlateSensor4,SIDE_PLATE_LM_4_SENSOR)
        DECLARE_STEP(SP5Step,SidePlateSensor5,SIDE_PLATE_LM_5_SENSOR)

        DECLARE_STEP(FowmFix1Step,FixationG1LimitSensor,FIXATION_G1_LM)
        DECLARE_STEP(FoamFix2Step,FixationG2LimitSensor,FIXATION_G2_LM)

        DECLARE_STEP(Head1Z1Step,HeadG1ZUpSensor,HEAD_G1_Z_UP)
        DECLARE_STEP(Head1Z2Step,HeadG1ZDownSensor,HEAD_G1_Z_DOWN)
        DECLARE_STEP(Head2Z1Step,HeadG2ZUpSensor,HEAD_G2_Z_UP)
        DECLARE_STEP(Head2Z2Step,HeadG2ZDownSensor,HEAD_G2_Z_DOWN)

        DECLARE_STEP(G1XStep,FrameG1XSensor,FRAME_G1_X)
        DECLARE_STEP(G1G2XStep,G1G2XSensor,G1_G2_X)
        DECLARE_STEP(G2XStep,G2FrameXSensor,G2_FRAME_X)

        DECLARE_STEP(Fix1YStep,FrameFixG1YSensor,FRAME_FIX_Y_G1)
        DECLARE_STEP(HeadFixG1YStep,FixHeadG1YSensor,FIX_HEAD_Y_G1)
        DECLARE_STEP(Head1Y1Step,HeadFrameG1YSensor1,HEAD_FRAME_Y_G1_1)
        DECLARE_STEP(Head1Y2Step,HeadFrameG1YSensor2,HEAD_FRAME_Y_G1_2)
        DECLARE_STEP(Fix2YStep,FrameFixG2YSensor,FRAME_FIX_Y_G2)
        DECLARE_STEP(HeadFixG2YStep,FixHeadG2YSensor,FIX_HEAD_Y_G2)
        DECLARE_STEP(Head2Y1Step,HeadFrameG2YSensor1,HEAD_FRAME_Y_G2_1)
        DECLARE_STEP(Head2Y2Step,HeadFrameG2YSensor2,HEAD_FRAME_Y_G2_2)
    
         //SensorTestMode()=delete;
    };
#endif
