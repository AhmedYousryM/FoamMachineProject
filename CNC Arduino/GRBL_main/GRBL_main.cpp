#include "GRBL_main.h"

GRBL_Main::Grbl_Main(byte Head_Num_var, HardwareSerial &grblSerial, const String &filename):
   GRBL_Control(grblSerial),GRBL_Sender(grblSerial, filename){
    if( Head_Num_var==1 | Head_Num_var==2 ){
        Head_num = Head_Num_var;
    }else{
        /*
        send a wrong message
        we have only two heads
        stop here
        */
    }
    

}

GRBL_Main::Grbl_Main(byte Head_Num_var, HardwareSerial &grblSerial): GRBL_Control(grblSerial){
    if( Head_Num_var==1 | Head_Num_var==2 ){
        Head_num = Head_Num_var;
    }else{
        /*
        send a wrong message
        we have only two heads
        stop here
        */
    }
}

void GRBL_Main::begin(){

    Control_begin();
    Sender_begin();

    HomingSequence.AddDelayInMillis(100);
    HomingSequence.AddNewStep(&Homing1);
    HomingSequence.AddDelayInMillis(100);
    HomingSequence.AddNewStep(&Homing2);
    HomingSequence.AddDelayInMillis(100);
    HomingSequence.AddNewStep(&StartPos1_1);
    HomingSequence.AddDelayInMillis(100);
    HomingSequence.AddNewStep(&StartPos1_2);
    HomingSequence.AddDelayInMillis(100);

    LubricationSequence.AddDelayInMillis(100);
    LubricationSequence.AddNewStep(&Lubrication1);
    LubricationSequence.AddDelayInMillis(100);
    LubricationSequence.AddNewStep(&Lubrication2);
    LubricationSequence.AddDelayInMillis(100);
    LubricationSequence.Repeat(LubricationRepition);

    StartPos2Sequence.AddDelayInMillis(100);
    StartPos2Sequence.AddNewStep(&StartPos2_1);
    StartPos2Sequence.AddDelayInMillis(100);
    StartPos2Sequence.AddNewStep(&StartPos2_2);
    StartPos2Sequence.AddDelayInMillis(100);

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

    BloweringSequence.AddDelayInMillis(100);
    BloweringSequence.AddNewStep(&Lubrication1);
    BloweringSequence.AddDelayInMillis(100);
    BloweringSequence.AddNewStep(&Lubrication2);
    BloweringSequence.AddDelayInMillis(100);

}


bool GRBL_Main::Homing(){
    HomingSequence.DoSequence();
    return Homing.isFinished();
}

bool Homing1(){

    if (Homing.FirstTimeStepExecuting()==true){
        doHoming();
        return false;
    }else{
        return !( IsMoving() );
    }
    
}

bool Homing2(){
    if (Head_num==1){
        setMachinePosition( CNC_MIN_X , CNC_MAX_Y , VIRTUAL_UPPER_Z_POINT )
    }elseif(Head_num==2){
        setMachinePosition( CNC_MAX_X , CNC_MAX_Y , VIRTUAL_UPPER_Z_POINT )
    }
    return true;
}

bool StartPos1_1(){
    if (HomingSequence.FirstTimeStepExecuting()==true){
        float finalZ = VIRTUAL_UPPER_Z_POINT - SAFE_GAB;
        moveToZ( finalZ );
        return false;
    }else{
        return !IsMoving();
    }
}

bool StartPos1_2(){
    if (HomingSequence.FirstTimeStepExecuting()==true){
        float CNC_MidPoint = (CNC_MIN_X+CNC_MAX_X)/2.0;
        float finalY = CNC_MAX_Y - SAFE_GAB;
        float finalZ = VIRTUAL_UPPER_Z_POINT - SAFE_GAB;
         if (Head_num==1){
            float finalX = CNC_MidPoint - 2*HEAD_WIDTH_X;
            moveTo(finalX , finalY , finalZ);
        }elseif(Head_num==2){
            float finalX = CNC_MidPoint - 2*HEAD_WIDTH_X;
            moveTo(CNC_MAX_X , finalY , finalZ);
        }
        return false;
    }else{
        return !( Control.IsMoving() );
    }
}

bool Lubricating(){
    LubricationSequence.DoSequence();
    return Lubrication.isFinished();
}

bool Lubrication1(){
    if (EndZAdjustment==true){ // only start lubrication after zaxis adjustment
    if (LubricationSequence.FirstTimeStepExecuting()==true){
        if (Head_num==1){
            float CNC_MidPoint = (CNC_MIN_X+CNC_MAX_X)/2.0;
            float finalX = CNC_MidPoint;
            float finalY = CNC_MAX_Y/2 ;
            float finalZ = MaximumZAxisTravel - SAFE_GAB;
            moveTo(finalX, finalY, finalZ );
        }elseif(Head_num==2){
            float finalX = CNC_MAX_X - SAFE_GAB;
            float finalY = CNC_MAX_Y/2 ;
            float finalZ = MaximumZAxisTravel - SAFE_GAB;
            moveTo(finalX, finalY, finalZ );
        }
        return false;
    }else{
        return !( IsMoving() );
    }
    }else{
        return false;
    }
}

bool Lubrication2(){

    if (LubricationSequence.FirstTimeStepExecuting()==true){
        if (Head_num==1){
            float finalX = CNC_MIN_X + SAFE_GAB;
            float finalY = CNC_MAX_Y - SAFE_GAB;
            float finalZ = SAFE_GAB;
            moveTo(finalX , finalY , finalZ);
        }elseif(Head_num==2){
            float CNC_MidPoint = (CNC_MIN_X+CNC_MAX_X)/2.0;
            float finalX = CNC_MidPoint;
            float finalY = CNC_MAX_Y - SAFE_GAB;
            float finalZ = SAFE_GAB;
            moveTo(CNC_MAX_X , finalY , finalZ);
        }
        return false;
    }else{
        return !( IsMoving() );
    }

}

bool GRBL_Main::GoTo_StartPos2(){
    StartPos2Sequence.DoSequence();
}

bool StartPos2_1(){
    if (Homing.FirstTimeStepExecuting()==true){
        moveToZ( start_z );
        return false;
    }else{
        return !IsMoving();
    }
}

bool StartPos2_2(){
    if (Homing.FirstTimeStepExecuting()==true){
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

        if (EndZAdjustment==false){
        if(Head_num==1){
            ZAxis( digitalRead(36) , digitalRead(37) , digitalRead(38) )
        }elseif(Head_num==2){
            ZAxis( digitalRead(39) , digitalRead(40) , digitalRead(41) )
        }
        AdjustmentEnd( digitalRead(38) && digitalRead(41) );
        }
    
    return EndZAdjustment;
}

void ZAxis(bool Up,bool Down,bool Zero){

    static unsigned long startPressTime=0;
    static char flag_char = 0;
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
        }elseif(Up==false & Down==true & Zero==false){
            if( millis() - startPressTime < 3000 ){
                if (flag_char==3)
                    Control_run
                else{
                    flag_char=3;
                    startJog( 'Z' , '-' , 1 , 1);
                }
            }else{
                if (flag_char==4)
                    Control_run
                else{
                    flag_char=4;
                    startJog( 'Z' , '-' , 10 , 10 );
                }
            }
        }elseif(Up==false & Down==false & Zero==true){
            flag_char=0;
            stopJog();
            float woodPieceThickness = 15;
            MaximumZAxisTravel = VIRTUAL_UPPER_Z_POINT - getZAxisMachinePos() + woodPieceThicknesss;
            setMachineZ( woodPieceThickness );
        }else{
            flag_char=0;
            startPressTime=millis();
            stopJog();
            // Do nothing
        }
    }

}

void AdjustmentEnd(bool b){
    if(b==true){
        EndZAdjustment = true;
        moveToZ( MaximumZAxisTravel - SAFE_GAB);
    }
}

bool GRBL_Main::IsZAxisAdjustmentFinished(){
    return EndZAdjustment;
}
void GRBL_Main::ReAdjustZAxis(){
    EndZAdjustment=false;
}

bool GRBL_Main::Start_Cutting(){
    if (Working_enable==true)
    WorkingSequence.DoSequence();
    return working.isFinished();
}
void GRBL_Main::Stop_Cutting(){
    Working_enable=false;
    pause();
    SpindleStop();
}

void GRBL_Main::NewCuttingProgram(){
    WorkingSequence.reset();
}

bool WaitForOK(){
    return isOkReceived();
}

bool Working1(){
    UseWorkpieceCoordinates();
    return true;
}

bool Working2(){
    SpindleStart();
    return true;
}

bool Working3(){
    start();
    return true;
}
bool Working4(){
    return Sender_run();
}
bool Working5(){
    UseMachineCoordinates();
    return true;
}
bool Working5(){
    SpindleStop();
    return true;
}

bool GRBL_Main::bigBlowering(){
    BloweringSequence.DoSequence();
    return BloweringSequence.isFinished();
}
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








