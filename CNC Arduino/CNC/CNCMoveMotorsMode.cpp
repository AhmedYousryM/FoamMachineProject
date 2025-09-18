#include "CNCMoveMotorsMode.h"

void MoveMotorsMode::begin(){
    SensorTest.begin();
    Fixation1Y.setMaxSpeed(100);
    Fixation2Y.setMaxSpeed(100);
}

void MoveMotorsMode::loop(){
    SensorTest.loop();
    switch( communication.get(&CR_Vars[CNC_SENSOR_PAGE]) ){
    case 0:
      
    break;
    case 1:
       //......warning......//
    break;
    case 2:
       //......warning......//
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
       MoveFixation();
    break;
    default:
    //......warning......//
    break;
}
}

static void MoveHead1(){
    if(CR_Vars[MOVE_G1_X]==true){
       GRBL_MoveSpeed (Head1 , 'x' , 1 , 5 , 20 );
    }else if(CR_Vars[MOVE_HEAD_G1_Y]==true){
       GRBL_MoveSpeed (Head1 , 'y' , 1 , 5 , 20 );
    }else if(CR_Vars[MOVE_HEAD_G1_Z]==true){
       GRBL_MoveSpeed (Head1 , 'z' , 1 , 3 , 6 );
    }else{
       Head1.stopJog();
    }
}

static void MoveHead2(){
    if(CR_Vars[MOVE_G1_X]==true){
       GRBL_MoveSpeed(Head2 , 'x' , 1 , 5 , 20 );
    }else if(CR_Vars[MOVE_HEAD_G1_Y]==true){
       GRBL_MoveSpeed(Head2 , 'y' , 1 , 5 , 20 );
    }else if(CR_Vars[MOVE_HEAD_G1_Z]==true){
       GRBL_MoveSpeed(Head2 , 'z' , 1 , 3 , 6 );
    }else{
       Head2.stopJog();
    }
}

static void MoveFixation1(){
    if(CR_Vars[MOVE_G1_X]==true){
         GRBL_MoveSpeed(Head1 , 'x' , 1 , 5 , 20 );
    }else if(CR_Vars[MOVE_FIX_G1_Y]==true){ 
      MotorSpeed( Fixation1Y , 1 , 5 , 20 );
    }else{
        Head1.stopJog();
        Fixation1Y.stop();
    }
}

static void MoveFixation2(){
    if(CR_Vars[MOVE_G2_X]==true){
         GRBL_MoveSpeed (Head2 , 'x' , 1 , 5 , 20 );
    }else if(CR_Vars[MOVE_FIX_G2_Y]==true){
      MotorSpeed( Fixation2Y , 1 , 5 , 20 );
    }else{
        Head2.stopJog();
        Fixation2Y.stop();
    }
}


void moveDir(Motor m, int distance){
        if( CR_Vars[MOVE_DIRECTION] == true ){
            Fixation1Y.moveTo( Fixation1Y.currentPosition() + distance );            
        }else if( CR_Vars[MOVE_DIRECTION] == false ){
            Fixation1Y.moveTo( Fixation1Y.currentPosition() - distance );
        }
}

void MotorSpeed(Motor m, float distance1, float distance2, float distance3){
    if( CR_Vars[MOVE_SPEED] == 0 ){
         m.stop();
    }else if( CR_Vars[MOVE_SPEED] == 1 ){
        // m.setMaxSpeed(100);
        moveDir( m, distance1);
    }else if( CR_Vars[MOVE_SPEED] == 2 ){
        // m.setMaxSpeed(100);
        moveDir( m, distance2);
    }else if( CR_Vars[MOVE_SPEED] == 3 ){
        // m.setMaxSpeed(100);
        moveDir( m, distance3);
    }else{
        // ......warning.....
    }
}

void GRBL_MoveSpeed(GRBL_Control Head, char axis, float distance1, float distance2, float distance3){
    
    char dir;
    if (CR_Vars[MOVE_DIRECTION]==true){
        dir='+';
    }else if(CR_Vars[MOVE_DIRECTION]==false){
        dir='-';
    }else{

    }
    
    if( CR_Vars[MOVE_SPEED] == 0 ){
        Head.stopJog();
    }else if( CR_Vars[MOVE_SPEED] == 1 ){
        Head.startJog( axis , dir , 50 , distance1);
    }else if( CR_Vars[MOVE_SPEED] == 2 ){
        Head.startJog( axis , dir , 50 , distance2);
    }else if( CR_Vars[MOVE_SPEED] == 3 ){
        Head.startJog( axis , dir , 50 , distance3);
    }else{
        // ......warning.....
    }
}
