#include "CNCMoveMotorsMode.h"

void MoveMotorsMode::begin(){
    SensorTest.begin();
    Fixation1Y.setMaxSpeed(100);
    Fixation2Y.setMaxSpeed(100);
}

void MoveMotorsMode::loop(){
    SensorTest.loop();
    
    switch( communication.GetVari(CRVariable,CNC_SENSOR_PAGE) ){
    case 0:
      
    break;
    case 1:
       //......warning......//
       SendWr(Wr202);
    break;
    case 2:
       //......warning......//
       SendWr(Wr203);
    break;
    case 3:
       MoveHead1();
    break;
    case 4:
       MoveHead2();
    break;
    case 5:
       MoveFixation1();
    break;
    case 6:
       MoveFixation2();
    break;
    default:
    //......warning......//
    SendWr(Wr204);
    break;
}
}

static void MoveMotorsMode:: MoveHead1(){
    
    if( communication.GetVarb(CRVariable,MOVE_G1_X) ){
       GRBL_MoveSpeed (Head1 , 'x' , 1 , 5 , 20 );
    }else if( communication.GetVarb(CRVariable,MOVE_HEAD_G1_Y) ){
       GRBL_MoveSpeed (Head1 , 'y' , 1 , 5 , 20 );
    }else if( communication.GetVarb(CRVariable,MOVE_HEAD_G1_Z) ){
       GRBL_MoveSpeed (Head1 , 'z' , 1 , 3 , 6 );
    }else{
       Head1.stopJog();
    }
}

static void MoveMotorsMode::MoveHead2(){
    if( communication.GetVarb(CRVariable,MOVE_G2_X) ){
       GRBL_MoveSpeed (Head2 , 'x' , 1 , 5 , 20 );
    }else if( communication.GetVarb(CRVariable,MOVE_HEAD_G2_Y) ){
       GRBL_MoveSpeed (Head2 , 'y' , 1 , 5 , 20 );
    }else if( communication.GetVarb(CRVariable,MOVE_HEAD_G2_Z) ){
       GRBL_MoveSpeed (Head2 , 'z' , 1 , 3 , 6 );
    }else{
       Head2.stopJog();
    }
}

static void MoveMotorsMode::MoveFixation1(){
    if( communication.GetVarb(CRVariable,MOVE_G1_X) ){
         GRBL_MoveSpeed(Head1 , 'x' , 1 , 5 , 20 );
    }else if( communication.GetVarb(CRVariable,MOVE_FIX_G1_Y) ){ 
      MotorSpeed( Fixation1Y , 1 , 5 , 20 );
    }else{
        Head1.stopJog();
        Fixation1Y.stop();
    }
}

static void MoveMotorsMode::MoveFixation2(){
    if( communication.GetVarb(CRVariable,MOVE_G2_X) ){
         GRBL_MoveSpeed (Head2 , 'x' , 1 , 5 , 20 );
    }else if( communication.GetVarb(CRVariable,MOVE_FIX_G2_Y) ){
      MotorSpeed( Fixation2Y , 1 , 5 , 20 );
    }else{
        Head2.stopJog();
        Fixation2Y.stop();
    }
}


void MoveMotorsMode::moveDir(Motor m, int distance){
        if( communication.GetVarb(CRVariable,MOVE_DIRECTION) ){
            Fixation1Y.moveTo( Fixation1Y.currentPosition() + distance );            
        }else{
            Fixation1Y.moveTo( Fixation1Y.currentPosition() - distance );
        }
}

void MoveMotorsMode::MotorSpeed(Motor m, float distance1, float distance2, float distance3){
    if( communication.GetVari(CRVariable,MOVE_SPEED)==0 ){
         m.stop();
    }else if( communication.GetVari(CRVariable,MOVE_SPEED)==1 ){
        // m.setMaxSpeed(100);
        moveDir( m, distance1);
    }else if( communication.GetVari(CRVariable,MOVE_SPEED)==2 ){
        // m.setMaxSpeed(100);
        moveDir( m, distance2);
    }else if( communication.GetVari(CRVariable,MOVE_SPEED)==3 ){
        // m.setMaxSpeed(100);
        moveDir( m, distance3);
    }else{
        // ......warning.....
        SendWr(Wr205);
    }
}

void MoveMotorsMode::GRBL_MoveSpeed(GRBL_Control Head, char axis, float distance1, float distance2, float distance3){
    
    char dir;
    if (communication.GetVarb(CRVariable,MOVE_DIRECTION) ){
        dir='+';
    }else{
        dir='-';
    }

    if( communication.GetVari(CRVariable,MOVE_SPEED)==0 ){
        Head.stopJog();
    }else if( communication.GetVari(CRVariable,MOVE_SPEED)==1 ){
        Head.startJog( axis , dir , 50 , distance1);
    }else if( communication.GetVari(CRVariable,MOVE_SPEED)==2 ){
        Head.startJog( axis , dir , 50 , distance2);
    }else if( communication.GetVari(CRVariable,MOVE_SPEED)==3 ){
        Head.startJog( axis , dir , 50 , distance3);
    }else{
        // ......warning.....
        SendWr(Wr206);
    }
}
SensorTestMode SensorTest;
MoveMotorsMode MoveMotors;