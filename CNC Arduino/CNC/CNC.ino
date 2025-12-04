#include "CNCMoveMotorsMode.h"
#include "CNCSensorTestMode.h"
#include "Fixation_main.h"
#include "GRBL_main.h"
void setup() {
  // put your setup code here, to run once:
  Fix_main1.begin();
  Fix_main2.begin();
  GRBL_main1.begin();
  GRBL_main2.begin();
  SensorTestMode::begin();
  MoveMotorsMode::begin();
}

void loop() {
  // put your main code here, to run repeatedly:
switch (RC_var_get(MODE)){
  case 0: // No-Mode
    switch (RC_var_get(NOMODE_SUBMODES)){
      case 0: // Nothing
      break;
      case 1: // Homing
        if(CM_var_get(CNC_OPERATION_DOING)){
          if( CM_var_get(FORK_START_HOMING) == flase ){
            if( Fix_main1.Homing() && Fix_main2.Homing() &&
              GRBL_main1.ZSensorHOmingFlagFun() &&
              GRBL_main2.ZSensorHOmingFlagFun()){
              CM_var_set(FORK_START_HOMING,true);
              CM_var_send(FORK_START_HOMING);           
            }
          }
          if ( Fix_main1.Homing() && Fix_main2.Homing() &&
           GRBL_main1.Homing() && GRBL_main2.Homing() ){
            CM_var_set(CNC_OPERATION_DOING,false);
            CM_var_send(CNC_OPERATION_DOING);
          }
        }
      break;
      case 3: // Sleeping
        // do nothing
      break;
    }
  break;
  case 1: // pre-poduction
    switch (RC_variables(PREPRODUCTION_SUBMODES)){
      case 0:
      break;
      case 1: // Z Axis Setup
          if ( GRBL_main1.GoTo_StartPos1() && GRBL_main2.GoTo_StartPos1() ){
            if ( !GRBL_main1.IsZAxisAdjustmentFinished() ){ //same variable for both heads
              GRBL_main1.ZAxis_Adjustment_Loop();
              GRBL_main2.ZAxis_Adjustment_Loop();
            }
          }
          if(CM_var_get(CNC_OPERATION_DOING)){
            if ( Fix_main1.GoingOut() &&  Fix_main2.GoingOut() ){
              Fix_main1.MotionReset=true;
              Fix_main2.MotionReset=true;   
              CM_var_set(CNC_OPERATION_DOING,false);
              CM_var_send(CNC_OPERATION_DOING);        
            }
          }
      break;
      case 2: // Start position 2
        if(CM_var_get(CNC_OPERATION_DOING)){
          if (Fix_main1.GoingInSafe() && Fix_main2.GoingInSafe() && 
          GRBL_main1.GoTo_StartPos2() && GRBL_main2.GoTo_StartPos2()){
              Fix_main1.MotionReset=true;
              Fix_main2.MotionReset=true;
              GRBL_main1.Reset_GoTo_StartPos2();
              GRBL_main2.Reset_GoTo_StartPos2();
              GRBL_main1.Reset_GoTo_StartPos1();
              GRBL_main2.Reset_GoTo_StartPos1();
              CM_var_set(CNC_OPERATION_DOING,false);
              CM_var_send(CNC_OPERATION_DOING);
          }
        }
      break;
      case 3: // Sleeping
      break;
    }
  break;
  case 2: // poduction process
    switch (RC_variables(PRODUCTION_SUBMODES)){
      case 0:
      break;
      case 1: // Production Operation
      // if(CR_var_get()){
      // Fixating Foam1 
        if(CF_var_get(G1_FIXATION_DOING)){
            if(GRBL1_main.beforeFixating()){ // start pos2 in y and mid-foam x
              if(Fix_main1.Fixating()){
                Fix_main1.ResetFixating();
                GRBL_main1.MotionReset=true;
                CF_var_set(G1_FIXATION_DOING,false);
                CF_var_send(G1_FIXATION_DOING);
              }
            }
        }
      // Fixating Foam2
        if(CF_var_get(G2_FIXATION_DOING)){
            if(GRBL2_main.beforeFixating()){ // start pos2 in y and mid-foam x
              if(Fix_main2.Fixating()){
                Fix_main2.ResetFixating();
                GRBL_main2.MotionReset=true;
                CF_var_set(G2_FIXATION_DOING,false);
                CF_var_send(G2_FIXATION_DOING);
              }
            }
        }

      // Cutting Foam 1
        if(CF_var_get(G1_CUTTING_DOING)){
          //if(GRBL_main1.GoTo_StartPos2()){ 
            if(GRBL_main1.Start_Cutting()){ // cut then goes up in z
              if(Fix_main1.GoingInSafe()){
                  Fix_main1.MotionReset=true;
                  GRBL_main1.NewCuttingProgram();
                  GRBL_main1.Reset_GoTo_StartPos2();
                  CF_var_set(G1_CUTTING_DOING,false);
                  CF_var_send(G1_CUTTING_DOING);
              }
            }
          //}
        }

        // Cutting Foam 2
        if(CF_var_get(G2_CUTTING_DOING)){
          //if(GRBL_main2.GoTo_StartPos2()){
            if(GRBL_main2.Start_Cutting()){
              if(Fix_main2.GoingInSafe()){
                  Fix_main2.MotionReset=true;
                  GRBL_main2.NewCuttingProgram();
                  GRBL_main2.Reset_GoTo_StartPos2();
                  CF_var_set(G2_CUTTING_DOING,false);
                  CF_var_send(G2_CUTTING_DOING);
              }
            }
          //}
        }
      break;
      case 2: // Blowering
        if(CM_var_get(CNC_OPERATION_DOING)){
          if( Fix_main1.DoingLubrication() && Fix_main2.DoingLubrication() &&
            GRBL_main1.bigBlowering() && GRBL_main2.bigBlowering() ){
              Fix_main1.ResetLubrication();
              Fix_main2.ResetLubrication();
              GRBL_main1.ResetBigBlowering();
              GRBL_main2.ResetBigBlowering();
              CM_var_set(CNC_OPERATION_DOING,false);
              CM_var_send(CNC_OPERATION_DOING);
            }
        }
      break;
      case 3: // Sleeping
      break;
    }
  break;
  case 3: // Move Motors
      MoveMotorsMode::loop();
  break;
  case 4: // Sensor Test
     SensorTestMode::loop();
  break;
  case 5: // pre-simulation
  
  break;
  case 6: // Simulation
  
  break;
  case 7: // calibration
  
  break;
  case 8: // error
  
  break;
  case 9: // lubrication
  
  break;
  default:
  
  break;
  }
  CheckMotionEndError();
  CheckErrorPin();
}

void CheckErrorPin(){
  if( ErrorPin.readFiltered() ){
    CM_var_set(MODE,8); // Error Mode is Active
    }
}

void CheckMotionEndError(){

if( !( (RC_var_get(MODE)==0) && (RC_var_get(NOMODE_SUBMODES)==1) ) ||
      (RC_var_get(MODE)==3) || (RC_var_get(MODE)==4) || (RC_var_get(MODE)==8) ){
  if( HeadG1ZUpSensor.readFiltered() ){
    // send error Err001
  }
  if( HeadG1ZDownSensor.readFiltered() ){
    // send error Err001
  }
  if( HeadG2ZUpSensor.readFiltered() ){
    // send error Err001
  }
  if( HeadG2ZDownSensor.readFiltered() ){
    // send error Err001
  }
  if( FrameFixG1YSensor.readFiltered() ){
    // send error Err001
  }
  if( FrameFixG2YSensor.readFiltered() ){
    // send error Err001
  }
  if( FixHeadG1YSensor.readFiltered() ){
    // send error Err001
  }
  if( FixHeadG2YSensor.readFiltered() ){
    // send error Err001
  }
  if( HeadFrameG1YSensor1.readFiltered() ){
    // send error Err001
  }    
  if( HeadFrameG1YSensor2.readFiltered() ){
    // send error Err001
  }    
  if( HeadFrameG2YSensor1.readFiltered() ){
    // send error Err001
  }    
  if( HeadFrameG2YSensor2.readFiltered() ){
    // send error Err001
  }    
  if( FrameG1XSensor.readFiltered() ){
    // send error Err001
  }    
  if( G1G2XSensor.readFiltered() ){
    // send error Err001
  }    
  if( G2FrameXSensor.readFiltered() ){
    // send error Err001
  }
}
}
