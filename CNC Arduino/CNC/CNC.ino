#include "CNCMoveMotorsMode.h"
#include "CNCSensorTestMode.h"
#include "Fixation_main.h"
#include "GRBL_main.h"
#include <Communication.h>
#include "CNCVariables.h"
void setup() {
  // put your setup code here, to run once:
  Fix_main1.begin();
  Fix_main2.begin();
  GRBL_main1.begin();
  GRBL_main2.begin();
  MoveMotors.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
switch (communication.GetVari(CMVariable,MODE)){
  case 0: // No-Mode
    switch (communication.GetVari(CMVariable,NOMODE_SUBMODES)){
      case 0: // Nothing
      break;
      case 1: // Homing
        if(communication.GetVarb(CMVariable,CNC_OPERATION_DOING)){
          if( !communication.GetVarb(CMVariable,FORK_START_HOMING)){
            if( Fix_main1.homing() && Fix_main2.homing() &&
              GRBL_main1.ZSensorHOmingFlagFun() &&
              GRBL_main2.ZSensorHOmingFlagFun()){
              communication.SetVar(CMVariable,MOVE_SPEED,1);  
              communication.SendVar('M',FORK_START_HOMING);           
            }
          }
          if ( Fix_main1.homing() && Fix_main2.homing() &&
           GRBL_main1.Homing() && GRBL_main2.Homing() ){
            communication.SetVar(CMVariable,MOVE_SPEED,0);
            communication.SendVar('M',CNC_OPERATION_DOING);
          }
        }
      break;
      case 3: // Sleeping
        // do nothing
      break;
    }
  break;
  case 1: // pre-poduction
    switch (communication.GetVari(CMVariable,PREPRODUCTION_SUBMODES)){
      case 0:
      break;
      case 1: // Z Axis Setup
          if ( GRBL_main1.GoTo_StartPos1() && GRBL_main2.GoTo_StartPos1() ){
            if ( !GRBL_main1.IsZAxisAdjustmentFinished() ){ //same variable for both heads
              GRBL_main1.ZAxis_Adjustment_Loop();
              GRBL_main2.ZAxis_Adjustment_Loop();
            }
          }
          if(communication.GetVarb(CMVariable,CNC_OPERATION_DOING)){
            if ( Fix_main1.goingOut() &&  Fix_main2.goingOut() ){
              Fix_main1.MotionReset=true;
              Fix_main2.MotionReset=true;   
              communication.SetVar(CMVariable,CNC_OPERATION_DOING,0);
              communication.SendVar('M',CNC_OPERATION_DOING);        
            }
          }
      break;
      case 2: // Start position 2
        if(communication.GetVarb(CMVariable,CNC_OPERATION_DOING)){
          if (Fix_main1.GoingInSafe() && Fix_main2.GoingInSafe() && 
          GRBL_main1.GoTo_StartPos2() && GRBL_main2.GoTo_StartPos2()){
              Fix_main1.MotionReset=true;
              Fix_main2.MotionReset=true;
              GRBL_main1.Restart_GoTo_StartPos2();
              GRBL_main2.Restart_GoTo_StartPos2();
              GRBL_main1.Restart_GoTo_StartPos1();
              GRBL_main2.Restart_GoTo_StartPos1();
              communication.SetVar(CMVariable,CNC_OPERATION_DOING,0);
              communication.SendVar('M',CNC_OPERATION_DOING);        
          }
        }
      break;
      case 3: // Sleeping
      break;
    }
  break;
  case 2: // poduction process
    switch (communication.GetVari(CMVariable,PRODUCTION_SUBMODES)){
      case 0:
      break;
      case 1: // Production Operation
      // if(CR_var_get()){
      // Fixating Foam1 
        if(communication.GetVarb(CFVariable,G1_FIXATION_DOING)){
            if(GRBL_main1.beforeFixating()){ // start pos2 in y and mid-foam x
              if(Fix_main1.fixating()){
                Fix_main1.RestartFixating();
                GRBL_main1.MotionReset=true;
                communication.SetVar(CFVariable,G1_FIXATION_DOING,0); 
                communication.SendVar('F',G1_FIXATION_DOING);
              }
            }
        }
      // Fixating Foam2
        if(communication.GetVarb(CFVariable,G2_FIXATION_DOING)){
            if(GRBL_main2.beforeFixating()){ // start pos2 in y and mid-foam x
              if(Fix_main2.fixating()){
                Fix_main2.RestartFixating();
                GRBL_main2.MotionReset=true;
                communication.SetVar(CFVariable,G2_FIXATION_DOING,0);
                communication.SendVar('F',G2_FIXATION_DOING);
              }
            }
        }

      // Cutting Foam 1
        if(communication.GetVarb(CFVariable,G1_CUTTING_DOING)){
          //if(GRBL_main1.GoTo_StartPos2()){ 
            if(GRBL_main1.Start_Cutting()){ // cut then goes up in z
              if(Fix_main1.GoingInSafe()){
                  Fix_main1.MotionReset=true;
                  GRBL_main1.NewCuttingProgram();
                  GRBL_main1.Restart_GoTo_StartPos2();
                  communication.SetVar(CFVariable,G1_CUTTING_DOING,0);
                  communication.SendVar('F',G1_CUTTING_DOING);
              }
            }
          //}
        }

        // Cutting Foam 2
        if(communication.GetVarb(CFVariable,G2_CUTTING_DOING)){
          //if(GRBL_main2.GoTo_StartPos2()){
            if(GRBL_main2.Start_Cutting()){
              if(Fix_main2.GoingInSafe()){
                  Fix_main2.MotionReset=true;
                  GRBL_main2.NewCuttingProgram();
                  GRBL_main2.Restart_GoTo_StartPos2();
                  communication.SetVar(CFVariable,G2_CUTTING_DOING,0);
                  communication.SendVar('F',G2_CUTTING_DOING);
              }
            }
          //}
        }
      break;
      case 2: // Blowering
        if(communication.GetVarb(CMVariable,CNC_OPERATION_DOING)){
          if( Fix_main1.DoingLubrication() && Fix_main2.DoingLubrication() &&
            GRBL_main1.bigBlowering() && GRBL_main2.bigBlowering() ){
              Fix_main1.RestartLubrication();
              Fix_main2.RestartLubrication();
              GRBL_main1.RestartBigBlowering();
              GRBL_main2.RestartBigBlowering();
              communication.SetVar(CMVariable,CNC_OPERATION_DOING,0);
              communication.SendVar('M',CNC_OPERATION_DOING);
            }
        }
      break;
      case 3: // Sleeping
      break;
    }
  break;
  case 3: // Move Motors
      MoveMotors.loop();
  break;
  case 4: // Sensor Test
     SensorTest.loop();
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
    communication.SetVar(CMVariable,MODE,8); // Error Mode is Active
    }
}

void CheckMotionEndError(){

if( !( (communication.GetVari(CMVariable,MODE)==0) && (communication.GetVari(CMVariable,MODE)==1) ) ||
      (communication.GetVari(CMVariable,MODE)==3) || (communication.GetVari(CMVariable,MODE)==4) || (communication.GetVari(CMVariable,MODE)==8) ){
  if( HeadG1ZUpSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR217);
  }
  if( HeadG1ZDownSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR218);
  }
  if( HeadG2ZUpSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR219);
  }
  if( HeadG2ZDownSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR220);
  }
  if( FrameFixG1YSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR221);
  }
  if( FrameFixG2YSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR222);
  }
  if( FixHeadG1YSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR223);
  }
  if( FixHeadG2YSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR224);
  }
  if( HeadFrameG1YSensor1.readFiltered() ){
    // send error Err001
    SendErr(ERR225);
  }    
  if( HeadFrameG1YSensor2.readFiltered() ){
    // send error Err001
    SendErr(ERR226);
  }    
  if( HeadFrameG2YSensor1.readFiltered() ){
    // send error Err001
    SendErr(ERR227);
  }    
  if( HeadFrameG2YSensor2.readFiltered() ){
    // send error Err001
    SendErr(ERR228);
  }    
  if( FrameG1XSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR229);
  }    
  if( G1G2XSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR230);
  }    
  if( G2FrameXSensor.readFiltered() ){
    // send error Err001
    SendErr(ERR231);
  }
}
}
