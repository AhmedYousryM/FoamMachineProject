#include "CNCSensorTestMode.h"

void SensorTestMode::begin(){
   SidePlateSequence.AddNewStep(&SP1Step);
   SidePlateSequence.AddNewStep(&ReadyToSendStep);
   SidePlateSequence.AddNewStep(&SP2Step);
   SidePlateSequence.AddNewStep(&ReadyToSendStep);
   SidePlateSequence.AddNewStep(&SP3Step);
   SidePlateSequence.AddNewStep(&ReadyToSendStep);
   SidePlateSequence.AddNewStep(&SP4Step);
   SidePlateSequence.AddNewStep(&ReadyToSendStep);
   SidePlateSequence.AddNewStep(&SP5Step);
   SidePlateSequence.AddNewStep(&ReadyToSendStep);
   SidePlateSequence.AddNewStep(&SPchange);

   FoamFixationSequence.AddNewStep(&FowmFix1Step);
   FoamFixationSequence.AddNewStep(&ReadyToSendStep);
   FoamFixationSequence.AddNewStep(&FoamFix2Step);
   FoamFixationSequence.AddNewStep(&ReadyToSendStep);
   FoamFixationSequence.AddNewStep(&FoamFixChange);
   FoamFixationSequence.AddNewStep(&FoamFixChange);

   MotionHead1Sequence.AddNewStep(&Head1Z1Step);
   MotionHead1Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead1Sequence.AddNewStep(&Head1Z2Step);
   MotionHead1Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead1Sequence.AddNewStep(&G1XStep);
   MotionHead1Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead1Sequence.AddNewStep(&G1G2XStep);
   MotionHead1Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead1Sequence.AddNewStep(&HeadFixG1YStep);
   MotionHead1Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead1Sequence.AddNewStep(&Head1Y1Step);
   MotionHead1Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead1Sequence.AddNewStep(&Head1Y2Step);
   MotionHead1Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead1Sequence.AddNewStep(&MotionHead1Change);

   MotionHead2Sequence.AddNewStep(&Head2Z1Step);
   MotionHead2Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead2Sequence.AddNewStep(&Head2Z2Step);
   MotionHead2Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead2Sequence.AddNewStep(&G1G2XStep);
   MotionHead2Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead2Sequence.AddNewStep(&G2XStep);
   MotionHead2Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead2Sequence.AddNewStep(&HeadFixG2YStep);
   MotionHead2Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead2Sequence.AddNewStep(&Head2Y1Step);
   MotionHead2Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead2Sequence.AddNewStep(&Head2Y2Step);
   MotionHead2Sequence.AddNewStep(&ReadyToSendStep);
   MotionHead2Sequence.AddNewStep(&MotionHead2Change);

   MotionFixation1Sequence.AddNewStep(&G1XStep);
   MotionFixation1Sequence.AddNewStep(&ReadyToSendStep);
   MotionFixation1Sequence.AddNewStep(&G1G2XStep);
   MotionFixation1Sequence.AddNewStep(&ReadyToSendStep);
   MotionFixation1Sequence.AddNewStep(&Fix1YStep);
   MotionFixation1Sequence.AddNewStep(&ReadyToSendStep);
   MotionFixation1Sequence.AddNewStep(&HeadFixG1YStep);
   MotionFixation1Sequence.AddNewStep(&ReadyToSendStep);
   MotionFixation1Sequence.AddNewStep(&MotionFix1Change);

   MotionFixation2Sequence.AddNewStep(&G1G2XStep);
   MotionFixation2Sequence.AddNewStep(&ReadyToSendStep);
   MotionFixation2Sequence.AddNewStep(&G2XStep);
   MotionFixation2Sequence.AddNewStep(&ReadyToSendStep);
   MotionFixation2Sequence.AddNewStep(&Fix2YStep);
   MotionFixation2Sequence.AddNewStep(&ReadyToSendStep);
   MotionFixation2Sequence.AddNewStep(&HeadFixG2YStep);
   MotionFixation2Sequence.AddNewStep(&ReadyToSendStep);
   MotionFixation2Sequence.AddNewStep(&MotionFix2Change);
}

void SensorTestMode::loop(){
    switch( communication.GetVari(CRVariable,CNC_SENSOR_PAGE) ){
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
    SendWr(WrWrongFcnCall);
    break;
}
}

static bool SensorTestMode::ReadyToSendStep(){
    if(device.stream==&Serial||device.stream==&Serial1||device.stream==&Serial2||device.stream==&Serial3){
        return true;
    }else{
        return false;
    }
}

static bool SensorTestMode::SPchange(){
  if( ReadyToSendStep() ){
    if( SidePlateSensor1.readFiltered()!= communication.GetVarb(CRVariable,SIDE_PLATE_LM_1_SENSOR)){
        SP1Step();
    }else if( SidePlateSensor2.readFiltered()!= communication.GetVarb(CRVariable,SIDE_PLATE_LM_2_SENSOR)){
        SP2Step();
    }else if( SidePlateSensor3.readFiltered()!= communication.GetVarb(CRVariable,SIDE_PLATE_LM_3_SENSOR)){
        SP3Step();
    }else if( SidePlateSensor4.readFiltered()!= communication.GetVarb(CRVariable,SIDE_PLATE_LM_4_SENSOR)){
        SP4Step();
    }else if( SidePlateSensor5.readFiltered()!= communication.GetVarb(CRVariable,SIDE_PLATE_LM_5_SENSOR)){
        SP5Step();
    }
  }
  return false;
}

static bool SensorTestMode::FoamFixChange() {
    if (ReadyToSendStep()) {
        if (FixationG1LimitSensor.readFiltered() != communication.GetVarb(CRVariable,FIXATION_G1_LM)) {
            FowmFix1Step();
        } else if (FixationG2LimitSensor.readFiltered() != communication.GetVarb(CRVariable,FIXATION_G2_LM)) {
            FoamFix2Step();
        }
    }
    return false;
}

static bool SensorTestMode::MotionHead1Change() {
    if (ReadyToSendStep()) {
        if (HeadG1ZUpSensor.readFiltered() != communication.GetVarb(CRVariable,HEAD_G1_Z_UP)) {
            Head1Z1Step();
        } else if (HeadG1ZDownSensor.readFiltered() != communication.GetVarb(CRVariable,HEAD_G1_Z_DOWN)) {
            Head1Z2Step();
        } else if (FrameG1XSensor.readFiltered() != communication.GetVarb(CRVariable,FRAME_G1_X)) {
            G1XStep();
        } else if (G1G2XSensor.readFiltered() != communication.GetVarb(CRVariable,G1_G2_X)) {
            G1G2XStep();
        } else if (FixHeadG1YSensor.readFiltered() != communication.GetVarb(CRVariable,FIX_HEAD_Y_G1)) {
            HeadFixG1YStep();
        } else if (HeadFrameG1YSensor1.readFiltered() != communication.GetVarb(CRVariable,HEAD_FRAME_Y_G1_1)) {
            Head1Y1Step();
        } else if (HeadFrameG1YSensor2.readFiltered() != communication.GetVarb(CRVariable,HEAD_FRAME_Y_G1_2)) {
            Head1Y2Step();
        }
    }
    return false;
}

static bool SensorTestMode::MotionHead2Change() {
    if (ReadyToSendStep()) {
        if (HeadG2ZUpSensor.readFiltered() != communication.GetVarb(CRVariable,HEAD_G2_Z_UP)) {
            Head2Z1Step();
        } else if (HeadG2ZDownSensor.readFiltered() != communication.GetVarb(CRVariable,HEAD_G2_Z_DOWN)) {
            Head2Z2Step();
        } else if (G1G2XSensor.readFiltered() != communication.GetVarb(CRVariable,G1_G2_X)) {
            G1G2XStep();
        } else if (G2FrameXSensor.readFiltered() != communication.GetVarb(CRVariable,G2_FRAME_X)) {
            G2XStep();
        } else if (FixHeadG2YSensor.readFiltered() != communication.GetVarb(CRVariable,FIX_HEAD_Y_G2)) {
            HeadFixG2YStep();
        } else if (HeadFrameG2YSensor1.readFiltered() != communication.GetVarb(CRVariable,HEAD_FRAME_Y_G2_1)) {
            Head2Y1Step();
        } else if (HeadFrameG2YSensor2.readFiltered() != communication.GetVarb(CRVariable,HEAD_FRAME_Y_G2_2)) {
            Head2Y2Step();
        }
    }
    return false;
}

static bool SensorTestMode::MotionFix1Change() {
    if (ReadyToSendStep()) {
        if (FrameG1XSensor.readFiltered() != communication.GetVarb(CRVariable,FRAME_G1_X)) {
            G1XStep();
        } else if (G1G2XSensor.readFiltered() != communication.GetVarb(CRVariable,G1_G2_X)) {
            G1G2XStep();
        } else if (FrameFixG1YSensor.readFiltered() != communication.GetVarb(CRVariable,FRAME_FIX_Y_G1)) {
            Fix1YStep();
        } else if (FixHeadG1YSensor.readFiltered() != communication.GetVarb(CRVariable,FIX_HEAD_Y_G1)) {
            HeadFixG1YStep();
        }
    }
    return false;
}

static bool SensorTestMode::MotionFix2Change() {
    if (ReadyToSendStep()) {
        if (G1G2XSensor.readFiltered() != communication.GetVarb(CRVariable,G1_G2_X)) {
            G1G2XStep();
        } else if (G2FrameXSensor.readFiltered() != communication.GetVarb(CRVariable,G2_FRAME_X)) {
            G2XStep();
        } else if (FrameFixG2YSensor.readFiltered() != communication.GetVarb(CRVariable,FRAME_FIX_Y_G2)) {
            Fix2YStep();
        } else if (FixHeadG2YSensor.readFiltered() != communication.GetVarb(CRVariable,FIX_HEAD_Y_G2)) {
            HeadFixG2YStep();
        }
    }
    return false;
}

