#include "CNCSensorTestMode.h"

void SensorTestMode::begin(){
   SidePlateSequence.AddNew(&SP1Step);
   SidePlateSequence.AddNew(&ReadyToSendStep);
   SidePlateSequence.AddNew(&SP2Step);
   SidePlateSequence.AddNew(&ReadyToSendStep);
   SidePlateSequence.AddNew(&SP3Step);
   SidePlateSequence.AddNew(&ReadyToSendStep);
   SidePlateSequence.AddNew(&SP4Step);
   SidePlateSequence.AddNew(&ReadyToSendStep);
   SidePlateSequence.AddNew(&SP5Step);
   SidePlateSequence.AddNew(&ReadyToSendStep);
   SidePlateSequence.AddNew(&SPchange);

   FoamFixationSequence.AddNew(&FowmFix1Step);
   FoamFixationSequence.AddNew(&ReadyToSendStep);
   FoamFixationSequence.AddNew(&FoamFix2Step);
   FoamFixationSequence.AddNew(&ReadyToSendStep);
   FoamFixationSequence.AddNew(&FoamFixChange);
   FoamFixationSequence.AddNew(&FoamFixChange);

   MotionHead1Sequence.AddNew(&Head1Z1Step);
   MotionHead1Sequence.AddNew(&ReadyToSendStep);
   MotionHead1Sequence.AddNew(&Head1Z2Step);
   MotionHead1Sequence.AddNew(&ReadyToSendStep);
   MotionHead1Sequence.AddNew(&G1XStep);
   MotionHead1Sequence.AddNew(&ReadyToSendStep);
   MotionHead1Sequence.AddNew(&G1G2XStep);
   MotionHead1Sequence.AddNew(&ReadyToSendStep);
   MotionHead1Sequence.AddNew(&HeadFixG1YStep);
   MotionHead1Sequence.AddNew(&ReadyToSendStep);
   MotionHead1Sequence.AddNew(&Head1Y1Step);
   MotionHead1Sequence.AddNew(&ReadyToSendStep);
   MotionHead1Sequence.AddNew(&Head1Y2Step);
   MotionHead1Sequence.AddNew(&ReadyToSendStep);
   MotionHead1Sequence.AddNew(&MotionHead1Change);

   MotionHead2Sequence.AddNew(&Head2Z1Step);
   MotionHead2Sequence.AddNew(&ReadyToSendStep);
   MotionHead2Sequence.AddNew(&Head2Z2Step);
   MotionHead2Sequence.AddNew(&ReadyToSendStep);
   MotionHead2Sequence.AddNew(&G1G2XStep);
   MotionHead2Sequence.AddNew(&ReadyToSendStep);
   MotionHead2Sequence.AddNew(&G2XStep);
   MotionHead2Sequence.AddNew(&ReadyToSendStep);
   MotionHead2Sequence.AddNew(&HeadFixG2YStep);
   MotionHead2Sequence.AddNew(&ReadyToSendStep);
   MotionHead2Sequence.AddNew(&Head2Y1Step);
   MotionHead2Sequence.AddNew(&ReadyToSendStep);
   MotionHead2Sequence.AddNew(&Head2Y2Step);
   MotionHead2Sequence.AddNew(&ReadyToSendStep);
   MotionHead2Sequence.AddNew(&MotionHead2Change);

   MotionFixation1Sequence.AddNew(&G1XStep);
   MotionFixation1Sequence.AddNew(&ReadyToSendStep);
   MotionFixation1Sequence.AddNew(&G1G2XStep);
   MotionFixation1Sequence.AddNew(&ReadyToSendStep);
   MotionFixation1Sequence.AddNew(&Fix1YStep);
   MotionFixation1Sequence.AddNew(&ReadyToSendStep);
   MotionFixation1Sequence.AddNew(&HeadFixG1YStep);
   MotionFixation1Sequence.AddNew(&ReadyToSendStep);
   MotionFixation1Sequence.AddNew(&MotionFix1Change);

   MotionFixation2Sequence.AddNew(&G1G2XStep);
   MotionFixation2Sequence.AddNew(&ReadyToSendStep);
   MotionFixation2Sequence.AddNew(&G2XStep);
   MotionFixation2Sequence.AddNew(&ReadyToSendStep);
   MotionFixation2Sequence.AddNew(&Fix2YStep);
   MotionFixation2Sequence.AddNew(&ReadyToSendStep);
   MotionFixation2Sequence.AddNew(&HeadFixG2YStep);
   MotionFixation2Sequence.AddNew(&ReadyToSendStep);
   MotionFixation2Sequence.AddNew(&MotionFix2Change);
}

void SensorTestMode::loop(){
    switch( communication.get(&CR_Vars[CNC_SENSOR_PAGE]) ){
    case 0:
      SidePlateSequence.Restart();
      FoamFixationSequence.Restart();
      MotionHead1Sequence.Restart();
      MotionHead2Sequence.Restart();
      MotionFixation1Sequence.Restart();
      MotionFixation2Sequence.Restart();
    break;
    case 1:
       SidePlateSequence.DoSequence();
    break;
    case 2:
       FoamFixationSequence.DoSequence();
    break;
    case 3:
       MotionHead1Sequence.DoSequence();
    break;
    case 4:
       MotionHead2Sequence.DoSequence();
    break;
    case 5:
       MotionFixation1Sequence.DoSequence();
    break;
    case 6:
       MotionFixation2Sequence.DoSequence();
    break;
    default:
    //......warning......//
    break;
}
}

static bool ReadyToSendStep(){
    return communication.ReadyToSend();
}

static bool SPchange(){
  if( communication.ReadyToSend() ){
    if( SidePlateSensor1.readFiltered()!=CR_Vars[SIDE_PLATE_LM_1_SENSOR] ){
        SP1Step();
    }else if( SidePlateSensor2.readFiltered()!=CR_Vars[SIDE_PLATE_LM_2_SENSOR] ){
        SP2Step();
    }else if( SidePlateSensor3.readFiltered()!=CR_Vars[SIDE_PLATE_LM_3_SENSOR] ){
        SP3Step();
    }else if( SidePlateSensor4.readFiltered()!=CR_Vars[SIDE_PLATE_LM_4_SENSOR] ){
        SP4Step();
    }else if( SidePlateSensor5.readFiltered()!=CR_Vars[SIDE_PLATE_LM_5_SENSOR] ){
        SP5Step();
    }
  }
  return false;
}

static bool FoamFixChange() {
    if (communication.ReadyToSend()) {
        if (FixationG1LimitSensor.readFiltered() != CR_Vars[FIXATION_G1_LM_SENSOR]) {
            FowmFix1Step();
        } else if (FixationG2LimitSensor.readFiltered() != CR_Vars[FIXATION_G2_LM_SENSOR]) {
            FoamFix2Step();
        }
    }
    return false;
}

static bool MotionHead1Change() {
    if (communication.ReadyToSend()) {
        if (HeadG1ZUpSensor.readFiltered() != CR_Vars[HEAD_G1_Z_UP_SENSOR]) {
            Head1Z1Step();
        } else if (HeadG1ZDownSensor.readFiltered() != CR_Vars[HEAD_G1_Z_DOWN_SENSOR]) {
            Head1Z2Step();
        } else if (FrameG1XSensor.readFiltered() != CR_Vars[FRAME_G1_X_SENSOR]) {
            G1XStep();
        } else if (G1G2XSensor.readFiltered() != CR_Vars[G1_G2_X_SENSOR]) {
            G1G2XStep();
        } else if (FixHeadG1YSensor.readFiltered() != CR_Vars[FIX_HEAD_Y_G1_SENSOR]) {
            HeadFixG1YStep();
        } else if (HeadFrameG1YSensor1.readFiltered() != CR_Vars[HEAD_FRAME_Y_G1_1_SENSOR]) {
            Head1Y1Step();
        } else if (HeadFrameG1YSensor2.readFiltered() != CR_Vars[HEAD_FRAME_Y_G1_2_SENSOR]) {
            Head1Y2Step();
        }
    }
    return false;
}

static bool MotionHead2Change() {
    if (communication.ReadyToSend()) {
        if (HeadG2ZUpSensor.readFiltered() != CR_Vars[HEAD_G2_Z_UP_SENSOR]) {
            Head2Z1Step();
        } else if (HeadG2ZDownSensor.readFiltered() != CR_Vars[HEAD_G2_Z_DOWN_SENSOR]) {
            Head2Z2Step();
        } else if (G1G2XSensor.readFiltered() != CR_Vars[G1_G2_X_SENSOR]) {
            G1G2XStep();
        } else if (G2FrameXSensor.readFiltered() != CR_Vars[G2_FRAME_X_SENSOR]) {
            G2XStep();
        } else if (FixHeadG2YSensor.readFiltered() != CR_Vars[FIX_HEAD_Y_G2_SENSOR]) {
            HeadFixG2YStep();
        } else if (HeadFrameG2YSensor1.readFiltered() != CR_Vars[HEAD_FRAME_Y_G2_1_SENSOR]) {
            Head2Y1Step();
        } else if (HeadFrameG2YSensor2.readFiltered() != CR_Vars[HEAD_FRAME_Y_G2_2_SENSOR]) {
            Head2Y2Step();
        }
    }
    return false;
}

static bool MotionFix1Change() {
    if (communication.ReadyToSend()) {
        if (FrameG1XSensor.readFiltered() != CR_Vars[FRAME_G1_X_SENSOR]) {
            G1XStep();
        } else if (G1G2XSensor.readFiltered() != CR_Vars[G1_G2_X_SENSOR]) {
            G1G2XStep();
        } else if (FrameFixG1YSensor.readFiltered() != CR_Vars[FRAME_FIX_Y_G1_SENSOR]) {
            Fix1YStep();
        } else if (FixHeadG1YSensor.readFiltered() != CR_Vars[FIX_HEAD_Y_G1_SENSOR]) {
            HeadFixG1YStep();
        }
    }
    return false;
}

static bool MotionFix2Change() {
    if (communication.ReadyToSend()) {
        if (G1G2XSensor.readFiltered() != CR_Vars[G1_G2_X_SENSOR]) {
            G1G2XStep();
        } else if (G2FrameXSensor.readFiltered() != CR_Vars[G2_FRAME_X_SENSOR]) {
            G2XStep();
        } else if (FrameFixG2YSensor.readFiltered() != CR_Vars[FRAME_FIX_Y_G2_SENSOR]) {
            Fix2YStep();
        } else if (FixHeadG2YSensor.readFiltered() != CR_Vars[FIX_HEAD_Y_G2_SENSOR]) {
            HeadFixG2YStep();
        }
    }
    return false;
}

#define DECLARE_STEP(StepName, SensorName, SensorComMacro) \
    static bool StepName() { \
        communication.Set(CR_Vars[SensorComMacro], SensorName.readFiltered()); \
        communication.Send(SensorComMacro); \
        return true;\
    }

DECLARE_STEP(SP1Step,SidePlateSensor1,SIDE_PLATE_LM_1_SENSOR)
DECLARE_STEP(SP2Step,SidePlateSensor2,SIDE_PLATE_LM_2_SENSOR)
DECLARE_STEP(SP3Step,SidePlateSensor3,SIDE_PLATE_LM_3_SENSOR)
DECLARE_STEP(SP4Step,SidePlateSensor4,SIDE_PLATE_LM_4_SENSOR)
DECLARE_STEP(SP5Step,SidePlateSensor5,SIDE_PLATE_LM_5_SENSOR)

    DECLARE_STEP(FowmFix1Step,FixationG1LimitSensor,FIXATION_G1_LM_SENSOR)
    DECLARE_STEP(FoamFix2Step,FixationG2LimitSensor,FIXATION_G2_LM_SENSOR)

    DECLARE_STEP(Head1Z1Step,HeadG1ZUpSensor,HEAD_G1_Z_UP_SENSOR)
    DECLARE_STEP(Head1Z2Step,HeadG1ZDownSensor,HEAD_G1_Z_DOWN_SENSOR)
    DECLARE_STEP(Head2Z1Step,HeadG2ZUpSensor,HEAD_G2_Z_UP_SENSOR)
    DECLARE_STEP(Head2Z2Step,HeadG2ZDownSensor,HEAD_G2_Z_DOWN_SENSOR)

    DECLARE_STEP(G1XStep,FrameG1XSensor,FRAME_G1_X_SENSOR)
    DECLARE_STEP(G1G2XStep,G1G2XSensor,G1_G2_X_SENSOR)
    DECLARE_STEP(G2XStep,G2FrameXSensor,G2_FRAME_X_SENSOR)

    DECLARE_STEP(Fix1YStep,FrameFixG1YSensor,FRAME_FIX_Y_G1_SENSOR)
    DECLARE_STEP(HeadFixG1YStep,FixHeadG1YSensor,FIX_HEAD_Y_G1_SENSOR)
    DECLARE_STEP(Head1Y1Step,HeadFrameG1YSensor1,HEAD_FRAME_Y_G1_1_SENSOR)
    DECLARE_STEP(Head1Y2Step,HeadFrameG1YSensor2,HEAD_FRAME_Y_G1_2_SENSOR)
    DECLARE_STEP(Fix2YStep,FrameFixG2YSensor,FRAME_FIX_Y_G2_SENSOR)
    DECLARE_STEP(HeadFixG2YStep,FixHeadG2YSensor,FIX_HEAD_Y_G2_SENSOR)
    DECLARE_STEP(Head2Y1Step,HeadFrameG2YSensor1,HEAD_FRAME_Y_G2_1_SENSOR)
    DECLARE_STEP(Head2Y2Step,HeadFrameG2YSensor2,HEAD_FRAME_Y_G2_2_SENSOR)
