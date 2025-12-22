#include "GRBL_main.h"

/*
GRBL_Main::Grbl_Main(byte Head_Num_var, HardwareSerial &grblSerial, const String &filename):
   GRBL_Control(grblSerial),GRBL_Sender(grblSerial, filename){
    if( Head_Num_var==1 | Head_Num_var==2 ){
        Head_num = Head_Num_var;
    }else{
        //error( invalid Head num);
    }
}
*/
GRBL_Main::GRBL_Main(byte Head_Num_var, HardwareSerial &grblSerial): 
GRBL_Control(grblSerial),GRBL_Sender(grblSerial){
    if( Head_Num_var==1 | Head_Num_var==2 ){
        Head_num = Head_Num_var;
    }else{
        /*
        send a wrong message
        we have only two heads
        stop here
        */
       SendWr(WrWrongFcnCall);
    }
}

void GRBL_Main::begin(){

    Control_begin();
    //Sender_begin();

    HomingSequence.AttachedObj(this);
    HomingSequence.AddDelayInMillis(100);
    HomingSequence.AddNewStep(&Homing1);
    HomingSequence.AddDelayInMillis(100);
    HomingSequence.AddNewStep(&Homing2);
    HomingSequence.AddDelayInMillis(100);
    HomingSequence.AddNewStep(&StartPos1_1);
    HomingSequence.AddDelayInMillis(100);
    HomingSequence.AddNewStep(&StartPos1_2);
    HomingSequence.AddDelayInMillis(100);

    LubricationSequence.AttachedObj(this);
    LubricationSequence.AddDelayInMillis(100);
    LubricationSequence.AddNewStep(&Lubrication1);
    LubricationSequence.AddDelayInMillis(100);
    LubricationSequence.AddNewStep(&Lubrication2);
    LubricationSequence.AddDelayInMillis(100);
    LubricationSequence.Repeat(LubricationRepition);

    StartPos2Sequence.AttachedObj(this);
    StartPos2Sequence.AddDelayInMillis(100);
    StartPos2Sequence.AddNewStep(&StartPos2_1);
    StartPos2Sequence.AddDelayInMillis(100);
    StartPos2Sequence.AddNewStep(&StartPos2_2);
    StartPos2Sequence.AddDelayInMillis(100);

    StartPos1Sequence.AttachedObj(this);
    StartPos1Sequence.AddDelayInMillis(100);
    StartPos1Sequence.AddNewStep(&StartPos1_1);
    StartPos1Sequence.AddDelayInMillis(100);
    StartPos1Sequence.AddNewStep(&StartPos1_2);
    StartPos1Sequence.AddDelayInMillis(100);

    WorkingSequence.AttachedObj(this);
    WorkingSequence.AddDelayInMillis(100);
    WorkingSequence.AddNewStep(&Working1);
    WorkingSequence.AddDelayInMillis(100);
    WorkingSequence.AddNewStep(&WaitForOK);
    WorkingSequence.AddNewStep(&Working2);
    WorkingSequence.AddDelayInMillis(100);
    WorkingSequence.AddNewStep(&WaitForOK);
    WorkingSequence.AddNewStep(&Working3);
    WorkingSequence.AddDelayInMillis(100);
    WorkingSequence.AddNewStep(&WaitForOK);
    WorkingSequence.AddNewStep(&Working4);
    WorkingSequence.AddDelayInMillis(100);
    WorkingSequence.AddNewStep(&WaitForOK);
    WorkingSequence.AddNewStep(&Working5);
    WorkingSequence.AddDelayInMillis(100);
    WorkingSequence.AddNewStep(&WaitForOK);
    WorkingSequence.AddNewStep(&Working6);
    WorkingSequence.AddDelayInMillis(100);

    BloweringSequence.AttachedObj(this);
    BloweringSequence.AddDelayInMillis(100);
    BloweringSequence.AddNewStep(&Lubrication1);
    BloweringSequence.AddDelayInMillis(100);
    BloweringSequence.AddNewStep(&Lubrication2);
    BloweringSequence.AddDelayInMillis(100);

}


bool GRBL_Main::Homing(){
    HomingSequence.DoSequence();
    return HomingSequence.isFinish();
}

bool GRBL_Main::Homing1(){

    if (HomingSequence.FirstTimeStepExecuting()==true){
        doHoming();
        return false;
    }else{
        return !( IsMoving() );
    }
    
}

bool GRBL_Main::Homing2(){
    if (Head_num==1){
        setMachinePosition( CNC_MIN_X , CNC_MAX_Y , VIRTUAL_UPPER_Z_POINT );
    }else if(Head_num==2){
        setMachinePosition( CNC_MAX_X , CNC_MAX_Y , VIRTUAL_UPPER_Z_POINT );
    }
    return true;
}

bool GRBL_Main::StartPos1_1(){
    if (HomingSequence.FirstTimeStepExecuting()==true){
        float finalZ = VIRTUAL_UPPER_Z_POINT - SAFE_GAB;
        moveToZ( finalZ );
        return false;
    }else{
        return !(IsMoving());
    }
}

bool GRBL_Main::StartPos1_2(){

    if (HomingSequence.FirstTimeStepExecuting()==true){
        float finalY = CNC_MAX_Y - SAFE_GAB;
        float finalZ = VIRTUAL_UPPER_Z_POINT - SAFE_GAB;
        float finalX = CNC_MidPoint - 2*HEAD_WIDTH_X;
        if (Head_num==1){moveTo(finalX , finalY , finalZ);}
        else if(Head_num==2){moveTo(CNC_MAX_X , finalY , finalZ);}
        return false;
    }else{
        return !(IsMoving() );
    }
}

bool GRBL_Main::Lubricating(){
    LubricationSequence.DoSequence();
    return LubricationSequence.isFinish();
}

bool GRBL_Main::Lubrication1(){
    if (EndZAdjustment){ // only start lubrication after zaxis adjustment
    if (LubricationSequence.FirstTimeStepExecuting()==true){
        if (Head_num==1){moveTo(CNC_MidPoint, CNC_MAX_Y/2, MaximumZAxisTravel - SAFE_GAB );}
        else if(Head_num==2){moveTo(CNC_MAX_X - SAFE_GAB, CNC_MAX_Y/2,MaximumZAxisTravel - SAFE_GAB );}
        return false;
    }else{
        return !( IsMoving() );
    }
    }else{
        return false;
    }
}

bool GRBL_Main::Lubrication2(){

    if (LubricationSequence.FirstTimeStepExecuting()==true){
        if (Head_num==1){ moveTo(CNC_MIN_X + SAFE_GAB , CNC_MAX_Y - SAFE_GAB , SAFE_GAB);}
        else if(Head_num==2){
            //float finalX = CNC_MidPoint;
            moveTo(CNC_MAX_X , CNC_MAX_Y - SAFE_GAB , SAFE_GAB);
        }
        return false;
    }else{
        return !( IsMoving() );
    }

}

bool GRBL_Main::GoTo_StartPos2(){
    StartPos2Sequence.DoSequence();
}

bool GRBL_Main::GoTo_StartPos1(){
    StartPos1Sequence.DoSequence();
}

void GRBL_Main::Restart_GoTo_StartPos2(){
    StartPos2Sequence.Restart();
}
void GRBL_Main::Restart_GoTo_StartPos1(){
    StartPos1Sequence.Restart();
}

bool GRBL_Main::StartPos2_1(){
    if (Head_num=1){
        Set_StartPos2(communication.GetVarf(CRVariable,HEAD1_START_POSITION_X),communication.GetVarf(CRVariable,HEAD1_START_POSITION_Y),communication.GetVarf(CRVariable,FOAM_HEIGHT)+SAFE_GAB);
    }else if (Head_num=2){
        Set_StartPos2(communication.GetVarf(CRVariable,HEAD2_START_POSITION_X),communication.GetVarf(CRVariable,HEAD2_START_POSITION_Y),communication.GetVarf(CRVariable,FOAM_HEIGHT)+SAFE_GAB);
    }

    if (StartPos2Sequence.FirstTimeStepExecuting()==true){
        moveToZ( start_z );
        return false;
    }else{
        return !IsMoving();
    }
}

bool GRBL_Main::StartPos2_2(){
    if (Head_num=1){
        Set_StartPos2(communication.GetVarf(CRVariable,HEAD1_START_POSITION_X),communication.GetVarf(CRVariable,HEAD1_START_POSITION_Y),communication.GetVarf(CRVariable,FOAM_HEIGHT)+SAFE_GAB);
    }else if (Head_num=2){
        Set_StartPos2(communication.GetVarf(CRVariable,HEAD2_START_POSITION_X),communication.GetVarf(CRVariable,HEAD2_START_POSITION_Y),communication.GetVarf(CRVariable,FOAM_HEIGHT)+SAFE_GAB);
    }
    if (StartPos2Sequence.FirstTimeStepExecuting()==true){
        moveToXY(start_x, start_y);
        return false;
    }else{
        return !IsMoving();
    }
}

void GRBL_Main::Set_StartPos2(float x, float y, float z){
    start_x=x;
    start_y=y;
    start_z=z;
}

bool GRBL_Main::ZAxis_Adjustment_Loop(){
        if ( communication.GetVarb(CRVariable,ZAXIS_SETUP_DONE) ){
        if(Head_num==1){
            ZAxis( pushButtonUp1.readFiltered() , pushButtonDown1.readFiltered() , pushButtonZero1.readFiltered() );
        }else if(Head_num==2){
            ZAxis( pushButtonUp2.readFiltered() , pushButtonDown2.readFiltered() , pushButtonZero2.readFiltered() );
        }
        AdjustmentEnd( pushButtonZero1.readFiltered() && pushButtonZero2.readFiltered() );
        }
    
    return IsZAxisAdjustmentFinished();
}

void GRBL_Main::ZAxis(bool Up,bool Down,bool Zero){

    if (startPressTime==0){
        startPressTime=millis();
    }else{
        if(Up==true & Down==false & Zero==false){
            if( millis() - startPressTime < 3000 ){
                if (flag_char==1)
                    Control_run();
                else{
                    flag_char=1;
                    startJog( 'Z' , '+' , 1 , 1);   
                }                        
            }else{
                if (flag_char==2)
                    Control_run();
                else{
                    flag_char=2;
                    startJog( 'Z' , '+' , 10 , 10 );
                }
            }
        }else if(Up==false & Down==true & Zero==false){
            if( millis() - startPressTime < 3000 ){
                if (flag_char==3)
                    Control_run();
                else{
                    flag_char=3;
                    startJog( 'Z' , '-' , 1 , 1);
                }
            }else{
                if (flag_char==4)
                    Control_run();
                else{
                    flag_char=4;
                    startJog( 'Z' , '-' , 10 , 10 );
                }
            }
        }else if(Up==false & Down==false & Zero==true){
            flag_char=0;
            stopJog();
            float woodPieceThickness = 15;
            MaximumZAxisTravel = VIRTUAL_UPPER_Z_POINT - getZAxisMachinePos() + woodPieceThickness;
            setMachineZ( woodPieceThickness );
        }else{
            flag_char=0;
            startPressTime=millis();
            stopJog();
            // Do nothing
        }
    }

}

void GRBL_Main::AdjustmentEnd(bool b){
    if(b){
        communication.SetVar(CRVariable,ZAXIS_SETUP_DONE,1);
        communication.SendVar('R',ZAXIS_SETUP_DONE);
        moveToZ( MaximumZAxisTravel - SAFE_GAB);
    }
}

bool GRBL_Main::IsZAxisAdjustmentFinished(){
    return communication.GetVarb(CRVariable,ZAXIS_SETUP_DONE);
}
/*
void GRBL_Main::ReAdjustZAxis(){
    EndZAdjustment=false;
}
*/

bool GRBL_Main::Start_Cutting(){
    if (Working_enable==true)
    WorkingSequence.DoSequence();
    return WorkingSequence.isFinish();
}
void GRBL_Main::Stop_Cutting(){
    Working_enable=false;
    pause();
    SpindleStop();
}

void GRBL_Main::NewCuttingProgram(){
    WorkingSequence.Restart();
}

bool GRBL_Main::WaitForOK(){
    return isOkReceived();
}

bool GRBL_Main::Working1(){
    SetWorkpieceCoordinates( pointx_half_foam + FORK_FIXATION_GAB , CNC_MAX_Y-0.5*communication.GetVarf(CRVariable,FOAMYWIDTH),communication.GetVarf(CRVariable,FOAM_HEIGHT));
    UseWorkpieceCoordinates();
    return sender_begin();
}

bool GRBL_Main::Working2(){
    SpindleStart();
    return true;
}

bool GRBL_Main::Working3(){
    start();
    return true;
}
bool GRBL_Main::Working4(){
    return Sender_run();
}
bool GRBL_Main::Working5(){
    if (WorkingSequence.FirstTimeStepExecuting()==true){
        UseMachineCoordinates();
        moveToZ( start_z );
        return false;
    }else{
        return !(IsMoving());
    }
    
}
bool GRBL_Main::Working6(){
    SpindleStop();
    return true;
}

bool GRBL_Main::bigBlowering(){
    BloweringSequence.DoSequence();
    return BloweringSequence.isFinish();
}

void GRBL_Main::RestartBigBlowering(){
    BloweringSequence.Restart();
}

bool GRBL_Main::beforeFixating(){
    if (MotionReset==true){
        if (Head_num==1){
            //  FORK_FIXATION_GAB is the distance fork release before fixation point
            if ( communication.GetVarf(CRVariable,FIXATION_POINT_2_X)!=0 ){
                pointx_half_foam =communication.GetVarf(CRVariable,FIXATION_POINT_2_X) - FORK_FIXATION_GAB - 0.5*communication.GetVarf(CRVariable,G1_FOAM_WIDTH);
            }else{
                pointx_half_foam =communication.GetVarf(CRVariable,FORK_FIXATION_POINT_2_X) - FORK_FIXATION_GAB - 0.5*communication.GetVarf(CRVariable,G1_FOAM_WIDTH);
            }
        }else{
            //  FORK_FIXATION_GAB is the distance fork release before fixation point
            pointx_half_foam =CNC_GATE_X_POS - FORK_FIXATION_GAB - 0.5* communication.GetVarf(CRVariable,G2_FOAM_WIDTH);
        }    
        moveToXY( pointx_half_foam , start_y); 
        MotionReset=false;
        return false;
    }else{
        return !(IsMoving());
    }
}

bool GRBL_Main::sender_begin(){
    if (Head_num==1){
        return Sender_begin(communication.GetVarS(CRVariable,GCODE_FILE_PATH1));
    }else{
        return Sender_begin(communication.GetVarS(CRVariable,GCODE_FILE_PATH2));
    }    
}

bool GRBL_Main::ZSensorHOmingFlagFun(){
    if (Head_num==1){
        ZSensorHomingFlag = HeadG1ZUpSensor.readFiltered();
    }else{
        ZSensorHomingFlag = HeadG2ZUpSensor.readFiltered();
    } 
    return ZSensorHomingFlag;
}

GRBL_Main GRBL_main1(1,Serial1);
GRBL_Main GRBL_main2(1,Serial2);
/*
bool Blowering1(){
    if (BloweringSequence.FirstTimeStepExecuting()==true){
        if (Head_num==1){
            float finalX = CNC_MIN_X + SAFE_GAB;
            float finalY = CNC_MAX_Y - SAFE_GAB;
            float finalZ = VIRTUAL_UPPER_Z_POINT - SAFE_GAB;
            moveTo(finalX , finalY , finalZ);
        }elseif(Head_num==2){
            float finalX = CNC_MAX_X - SAFE_GAB;
            float finalY = CNC_MAX_Y - SAFE_GAB;
            float finalZ = VIRTUAL_UPPER_Z_POINT - SAFE_GAB;
            moveTo(CNC_MAX_X , finalY , finalZ);
        }
        return false;
    }else{
        return !( IsMoving() );
    }
}

bool Blowering2(){
    if (BloweringSequence.FirstTimeStepExecuting()==true){
        float CNC_MidPoint = (CNC_MIN_X+CNC_MAX_X)/2.0;
        if (Head_num==1){
            float finalX = CNC_MidPoint - HEAD_WIDTH_X/2.0 - SAFE_GAB;
            float finalY = CNC_MAX_Y/2 + HEAD_WIDTH_Y/2 + SAFE_GAB;
            moveTo(finalX, finalY, VIRTUAL_UPPER_Z_POINT - 50 );
        }elseif(Head_num==2){
            float finalX = CNC_MidPoint + HEAD_WIDTH_X/2.0 + SAFE_GAB;
            float finalY = CNC_MAX_Y/2 + HEAD_WIDTH_Y/2 + SAFE_GAB;
            moveTo(finalX, finalY, VIRTUAL_UPPER_Z_POINT - 50 );
        }
        return false;
    }else{
        return !( IsMoving() );
    }
}
*/


    







