#include "ForkSensorTestMode.h"

static bool ReadyToSendStep(){
    return communication.ReadyToSend();
}

static bool IRFoamSensorchange(){
  if( communication.ReadyToSend() ){
    if( IRCNCInletSensor.readFiltered()!=FR_Vars[IR_CNC_INLET_SENSOR] ){
        IRCNCInStep();
    }else if( IRCNCExitSensor.readFiltered()!=FR_Vars[IR_CNC_EXIT_SENSOR] ){
        IRCNCExStep();
    }
  }
  return false;
}

static bool Fork2Nailschange(){
  if( communication.ReadyToSend() ){
    if( Fork2Nail1Sensor.readFiltered()!=FR_Vars[FORK2_NAIL1_LM_SENSOR] ){
        Frk2Nail1Step();
    }else if( Fork2Nail2Sensor.readFiltered()!=FR_Vars[FORK2_NAIL2_LM_SENSOR] ){
        Frk2Nail2Step();
    }else if( Fork2Nail3Sensor.readFiltered()!=FR_Vars[FORK2_NAIL3_LM_SENSOR] ){
        Frk2Nail3Step();
    }
  }
  return false;
}

static bool Fork3Nailschange(){
  if( communication.ReadyToSend() ){
    if( Fork3Nail1Sensor.readFiltered()!=FR_Vars[FORK3_NAIL1_LM_SENSOR] ){
        Frk3Nail1Step();
    }else if( Fork3Nail2Sensor.readFiltered()!=FR_Vars[FORK3_NAIL2_LM_SENSOR] ){
        Frk3Nail2Step();
    }else if( Fork3Nail3Sensor.readFiltered()!=FR_Vars[FORK3_NAIL3_LM_SENSOR] ){
        Frk3Nail3Step();
    }
  }
  return false;
}

static bool CNCGateFixationChange() {
    if (communication.ReadyToSend()) {
        if (CNCGateFixationSensor1.readFiltered() != FR_Vars[CNC_GATE_FIXATION1_LM_SENSOR]) {
            CNCGtFix1Step();
        } else if (CNCGateFixationSensor2.readFiltered() != FR_Vars[CNC_GATE_FIXATION2_LM_SENSOR]) {
            CNCGtFix2Step();
        } else if (CNCGateFixationSensor3.readFiltered() != FR_Vars[CNC_GATE_FIXATION3_LM_SENSOR]) {
            CNCGtFix3Step();
        }
    }
    return false;
}

static bool MotionFork1Change() {
    if (communication.ReadyToSend()) {
        if (FrameFork1XSensor.readFiltered() != FR_Vars[FORK1_FRAME_X_SENSOR]) {
            Frk1FXStep();
        } else if (Fork1Fork2XSensor.readFiltered() != FR_Vars[FORK1_FORK2_X_SENSOR]) {
            Frk1Frk2XStep();
        } else if (Fork1ZUpSensor.readFiltered() != FR_Vars[FORK1_Z_UP_SENSOR]) {
            Frk1Z1Step();
        } else if (Fork1ZDnSensor.readFiltered() != FR_Vars[FORK1_Z_DOWN_SENSOR]) {
            Frk1Z2Step();
        }
    }
    return false;
}

static bool MotionFork2Change() {
    if (communication.ReadyToSend()) {
        if (Fork1Fork2XSensor.readFiltered() != FR_Vars[FORK1_FORK2_X_SENSOR]) {
            Frk1Frk2XStep();
        } else if (Fork2Fork3XSensor.readFiltered() != FR_Vars[FORK2_FORK3_X_SENSOR]) {
            Frk2Frk3XStep();
        } else if (Fork2ZUpSensor.readFiltered() != FR_Vars[FORK2_Z_UP_SENSOR]) {
            Frk2Z1Step();
        } else if (Fork2ZDnSensor.readFiltered() != FR_Vars[FORK2_Z_DOWN_SENSOR]) {
            Frk2Z2Step();
        }
    }
    return false;
}

static bool MotionFork3Change() {
    if (communication.ReadyToSend()) {
        if (Fork2Fork3XSensor.readFiltered() != FR_Vars[FORK2_FORK3_X_SENSOR]) {
            Frk2Frk3XStep();
        } else if (Fork3FrameXSensor.readFiltered() != FR_Vars[FORK3_FRAME_X_SENSOR]) {
            Frk3FXStep();
        } else if (Fork3ZUpSensor.readFiltered() != FR_Vars[FORK3_Z_UP_SENSOR]) {
            Frk3Z1Step();
        } else if (Fork3ZDnSensor.readFiltered() != FR_Vars[FORK3_Z_DOWN_SENSOR]) {
            Frk3Z2Step();
        }
    }
    return false;
}

static bool MotionCNCGateChange() {
    if (communication.ReadyToSend()) {
        if (CNCGateZUpSensor.readFiltered() != FR_Vars[CNC_GATE_Z_UP_SENSOR]) {
            CNCGtZ1Step();
        } else if (CNCGateZDnSensor.readFiltered() != FR_Vars[CNC_GATE_Z_DOWN_SENSOR]) {
            CNCGtZ2Step();
        }
    }
    return false;
}

#define DECLARE_STEP(StepName, SensorName, SensorComMacro) 
    static bool StepName() { 
        SET_BOOL(FR_Vars[SensorComMacro], SensorName.readFiltered());
        //communication.Send(SensorComMacro);
        return true;
    }

    DECLARE_STEP(IRCNCInStep,IRCNCInletSensor,IR_CNC_INLET_SENSOR)
    DECLARE_STEP(IRCNCExStep,IRCNCExitSensor,IR_CNC_EXIT_SENSOR)

    DECLARE_STEP(Frk2Nail1Step,Fork2Nail1Sensor,FORK2_NAIL1_LM_SENSOR)
    DECLARE_STEP(Frk2Nail2Step,Fork2Nail2Sensor,FORK2_NAIL2_LM_SENSOR)
    DECLARE_STEP(Frk2Nail3Step,Fork2Nail3Sensor,FORK2_NAIL3_LM_SENSOR)

    DECLARE_STEP(Frk3Nail1Step,Fork3Nail1Sensor,FORK3_NAIL1_LM_SENSOR)
    DECLARE_STEP(Frk3Nail2Step,Fork3Nail2Sensor,FORK3_NAIL2_LM_SENSOR)
    DECLARE_STEP(Frk3Nail3Step,Fork3Nail3Sensor,FORK3_NAIL3_LM_SENSOR)

    DECLARE_STEP(CNCGtFix1Step,CNCGateFixationSensor1,CNC_GATE_FIXATION1_LM_SENSOR)
    DECLARE_STEP(CNCGtFix2Step,CNCGateFixationSensor2,CNC_GATE_FIXATION2_LM_SENSOR)
    DECLARE_STEP(CNCGtFix3Step,CNCGateFixationSensor3,CNC_GATE_FIXATION3_LM_SENSOR)

    DECLARE_STEP(Frk1FXStep,FrameFork1XSensor,FORK1_FRAME_X_SENSOR)
    DECLARE_STEP(Frk1Frk2XStep,Fork1Fork2XSensor,FORK1_FORK2_X_SENSOR)
    DECLARE_STEP(Frk2Frk3XStep,Fork2Fork3XSensor,FORK2_FORK3_X_SENSOR)
    DECLARE_STEP(Frk3FXStep,Fork3FrameXSensor,FORK3_FRAME_X_SENSOR)

    DECLARE_STEP(Frk1Z1Step,Fork1ZUpSensor,FORK1_Z_UP_SENSOR)
    DECLARE_STEP(Frk1Z2Step,Fork1ZDnSensor,FORK1_Z_DOWN_SENSOR)
    DECLARE_STEP(Frk2Z1Step,Fork2ZUpSensor,FORK2_Z_UP_SENSOR)
    DECLARE_STEP(Frk2Z2Step,Fork2ZDnSensor,FORK2_Z_DOWN_SENSOR)
    DECLARE_STEP(Frk3Z1Step,Fork3ZUpSensor,FORK3_Z_UP_SENSOR)
    DECLARE_STEP(Frk3Z2Step,Fork3ZDnSensor,FORK3_Z_DOWN_SENSOR)
    DECLARE_STEP(CNCGtZ1Step,CNCGateZUpSensor,CNC_GATE_Z_UP_SENSOR)
    DECLARE_STEP(CNCGtZ2Step,CNCGateZDnSensor,CNC_GATE_Z_DOWN_SENSOR)

static void SensorTestMode::begin(){
    IRFoamSensorSequence.AddNew(&IRCNCInStep);
    IRFoamSensorSequence.AddNew(&ReadyToSendStep);
    IRFoamSensorSequence.AddNew(&IRCNCExStep);
    IRFoamSensorSequence.AddNew(&ReadyToSendStep);
    IRFoamSensorSequence.AddNew(&IRFoamSensorchange);

    Fork2NailsSequence.AddNew(&Fork2Nail1Step);
    Fork2NailsSequence.AddNew(&ReadyToSendStep);
    Fork2NailsSequence.AddNew(&Fork2Nail2Step);
    Fork2NailsSequence.AddNew(&ReadyToSendStep);
    Fork2NailsSequence.AddNew(&Fork2Nail3Step);
    Fork2NailsSequence.AddNew(&ReadyToSendStep);
    Fork2NailsSequence.AddNew(&Fork2Nailschange);

    Fork3NailsSequence.AddNew(&Fork3Nail1Step);
    Fork3NailsSequence.AddNew(&ReadyToSendStep);
    Fork3NailsSequence.AddNew(&Fork3Nail2Step);
    Fork3NailsSequence.AddNew(&ReadyToSendStep);
    Fork3NailsSequence.AddNew(&Fork3Nail3Step);
    Fork3NailsSequence.AddNew(&ReadyToSendStep);
    Fork3NailsSequence.AddNew(&Fork3Nailschange);

    CNCGateFixationSequence.AddNew(&CNCGtFix1Step);
    CNCGateFixationSequence.AddNew(&ReadyToSendStep);
    CNCGateFixationSequence.AddNew(&CNCGtFix2Step);
    CNCGateFixationSequence.AddNew(&ReadyToSendStep);
    CNCGateFixationSequence.AddNew(&CNCGtFix3Step);
    CNCGateFixationSequence.AddNew(&ReadyToSendStep);
    CNCGateFixationSequence.AddNew(&CNCGateFixationChange);

    MotionFork1Sequence.AddNew(&Frk1FXStep);
    MotionFork1Sequence.AddNew(&ReadyToSendStep);
    MotionFork1Sequence.AddNew(&Frk1Frk2XStep);
    MotionFork1Sequence.AddNew(&ReadyToSendStep);
    MotionFork1Sequence.AddNew(&Frk1Z1Step);
    MotionFork1Sequence.AddNew(&ReadyToSendStep);
    MotionFork1Sequence.AddNew(&Frk1Z2Step);
    MotionFork1Sequence.AddNew(&ReadyToSendStep);
    MotionFork1Sequence.AddNew(&MotionFork1Change);

    MotionFork2Sequence.AddNew(&Frk1Frk2XStep);
    MotionFork2Sequence.AddNew(&ReadyToSendStep);
    MotionFork2Sequence.AddNew(&Frk2Frk3XStep);
    MotionFork2Sequence.AddNew(&ReadyToSendStep);
    MotionFork2Sequence.AddNew(&Frk2Z1Step);
    MotionFork2Sequence.AddNew(&ReadyToSendStep);
    MotionFork2Sequence.AddNew(&Frk2Z2Step);
    MotionFork2Sequence.AddNew(&ReadyToSendStep);
    MotionFork2Sequence.AddNew(&MotionFork2Change);

    MotionFork3Sequence.AddNew(&Frk2Frk3XStep);
    MotionFork3Sequence.AddNew(&ReadyToSendStep);
    MotionFork3Sequence.AddNew(&Frk3FXStep);
    MotionFork3Sequence.AddNew(&ReadyToSendStep);
    MotionFork3Sequence.AddNew(&Frk3Z1Step);
    MotionFork3Sequence.AddNew(&ReadyToSendStep);
    MotionFork3Sequence.AddNew(&Frk3Z2Step);
    MotionFork3Sequence.AddNew(&ReadyToSendStep);
    MotionFork3Sequence.AddNew(&MotionFork3Change);

    MotionCNCGateSequence.AddNew(&CNCGtZ1Step);
    MotionCNCGateSequence.AddNew(&ReadyToSendStep);
    MotionCNCGateSequence.AddNew(&CNCGtZ2Step);
    MotionCNCGateSequence.AddNew(&ReadyToSendStep);
    MotionCNCGateSequence.AddNew(&MotionCNCGateChange);
}

void SensorTestMode::loop(){
    switch( getUCharValue(&FR_Vars[FORK_SENSOR_PAGE]) ){
        case 0:
            IRFoamSensorSequence.Restart();
            Fork2NailsSequence.Restart();
            Fork3NailsSequence.Restart();
            CNCGateFixationSequence.Restart();
            MotionFork1Sequence.Restart();
            MotionFork2Sequence.Restart();
            MotionFork3Sequence.Restart();
            MotionCNCGateSequence.Restart();
        break;
        case 1:
            IRFoamSensorSequence.DoSequence();
        break;
        case 2:
            Fork2NailsSequence.DoSequence();
        break;
        case 3:
            Fork3NailsSequence.DoSequence();
        break;
        case 4:
            CNCGateFixationSequence.DoSequence();
        break;
        case 5:
            MotionFork1Sequence.DoSequence();
        break;
        case 6:
            MotionFork2Sequence.DoSequence();
        break;
        case 7:
            MotionFork3Sequence.DoSequence();
        break;
        case 8:
            MotionCNCGateSequence.DoSequence();
        break;
        default:
        //......warning......//
        break;
    }
}