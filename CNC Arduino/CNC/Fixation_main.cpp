#include "Fixation_main.h"

// Define functions, those do not belong to class





// Define functions, those do not belong to class

    Fixation_main::Fixation_main(Motor motor, char Fixation_num ){
        _motor=motor;
        if (Fixation_num==1 || Fixation_num==2){
            _num=Fixation_num;
        }else{
            ///error
        }
        
    }

    bool Fixation_main::begin(){
        Homing.AttachedObj(this);
        Fixating.AttachedObj(this);
        GoingIn.AttachedObj(this);
        Lubricating.AttachedObj(this);

        Homing.AddNewStep(&Homing_MoveLongRun);
        Homing.AddDelayInMillis(100);
        Homing.AddNewStep(&Homing_Stop);
        Homing.AddDelayInMillis(100);
        Homing.AddNewStep(&Homing_GoReverseRun);
        Homing.AddDelayInMillis(100);
        Homing.AddNewStep(&Homing_MoveShortRun);
        Homing.AddDelayInMillis(100);
        Homing.AddNewStep(&Homing_Stop);
        Homing.AddDelayInMillis(100);        
        Homing.AddNewStep(&Homing_End);
        Homing.AddDelayInMillis(100);
        Homing.AddNewStep(&Homing_GoReverseRun);

        Fixating.AddNewStep(&beforeFix);
        Fixating.AddDelayInMillis(100);
        Fixating.AddNewStep(&approachNearFoam);
        Fixating.AddDelayInMillis(100);
        Fixating.AddNewStep(&touchFoam);
        Fixating.AddNewStep(&move2mm);
        Fixating.AddDelayInMillis(100);
        Fixating.AddNewStep(&fixationEnd);


    }

    bool Fixation_main::Homing(){
        Homing.DoSequence();
    }

    bool Fixation_main::Fixating(){
        Fixating.DoSequence();
    }
    void Fixation_main::ResetFixating(){
        Fixating.Reset();
    }

    bool Fixation_main::GoingOut(){
        GoingOut.DoSequence();

    }

    bool Fixation_main::GoingIn(){
        GoingIn.DoSequence();
    }


    bool Fixation_main::DoingLubrication(){
        return Lubricatin.DoSequence();
    }
    void Fixation_main::ResetLubrication(){
        Lubricatin.Reset();
    }



// Sequence Step
    bool Fixation_main::Homing_MoveLongRun(){
        if(Homing.FirstTimeStepExecuting==true){
            _motor.setMaxSpeed(MAX_SPEED_HOMING_1);
            _motor.move(-CNC_MAX_Y-20);
            return false;
        }else{
            _motor.run();
            if (_motor.distanceToGo()){
                if(_num==1){
                    return FrameFixG1YSensor.readFiltered();
                }else if(_num==2){
                    return FrameFixG2YSensor.readFiltered();
                }
                else{
                    /////error
                }
            }else{
                ///error //
                // cross the cnc limits
            }
        }
    }

    bool Fixation_main::Homing_Stop(){
        _motor.stop();
        if(Homing.FirstTimeStepExecuting==true){
            return false;
        }else{
            return (_motor.speed()==0);
        }
    }

    bool Fixation_main::Homing_GoReverseRun(){
        if(Homing.FirstTimeStepExecuting==true){
            _motor.move(30);
        }else{
            _motor.run();
        }
        return (_motor.distanceToGo()==0);
    }

    bool Fixation_main::Homing_MoveShortRun(){
        if(Homing.FirstTimeStepExecuting==true){
            _motor.setMaxSpeed(MAX_SPEED_HOMING_2);
            _motor.move(-50);
            return false;
        }else{
            _motor.run();
            if (_motor.distanceToGo()){
                if(_num==1){
                    return FrameFixG1YSensor.readFiltered();
                }else if(_num==2){
                    return FrameFixG2YSensor.readFiltered();
                }
                else{
                    /////error
                }
            }else{
                ///error
            }
        }
    }

    bool Fixation_main::Homing_End(){
        _motor.setCurrentPosition(0);
        _motor.setMaxSpeed(HIGH_SPEED);
        return true;
    }

    bool Fixation_main::GoingOut(){
        if(MotionReset==true){
            _motor.moveTo(GAB);
            MotionReset=false;
            return false;
        }else{
            _motor.run();
            return ( _motor.distanceToGo()==0 );
        }
    }

    bool Fixation_main::GoingInSafe(){
        if (MotionReset==true){
            float target_pos = safe_pos_before( CNC_MAX_Y - getValue(&CR_Vars[FOAMYWIDTH]) - BETWEEN_17_WOOD - GAB );
            _motor.moveTo( target_pos );
        }else{
            _motor.run();
            return ( _motor.distanceToGo()==0 );
        }
    }

// helper functions
float safe_pos_before(float pos){
    // the woods are numbered in order similar to y axis
    // the first safe postion is above the wood number 0
    int wood_no = floor( (pos - FIRST_SAFE_POS)/BETWEEN_17_WOOD );
    return FIRST_SAFE_POS + wood_no * BETWEEN_17_WOOD;
}

void Fixation_main::before_Fixation(float foam_start,float foam_end){
    for(int i=0 ; i<4; i++){
        float sensor_pos = SIDEWOOD_FIRST_SENSOR + i * SIDEWOOD_BETWEEN_SENSOR;
        if ( ( sensor_pos > foam_start + 10 ) && ( sensor_pos < foam_end - 10 ) )
            sidePlateSensorBool[i] = true; 
        else
            sidePlateSensorBool[i] = false;
    }
    int i = 4;
    float sensor_pos = SIDEWOOD_FIRST_SENSOR + i * SIDEWOOD_BETWEEN_SENSOR_LAST_ONE;
        if ( ( sensor_pos > foam_start + 10 ) && ( sensor_pos < foam_end - 10 ) )
            sidePlateSensorBool[i] = true; 
        else
            sidePlateSensorBool[i] = false;
}

bool Fixation_main::beforeFix(){
    if(_num==1){
        //  FORK_FIXATION_GAB is the distance fork release before fixation point
        float end_point=RC_var_get(FIXATION_POINT_2_X) - FORK_FIXATION_GAB;
        before_Fixation(end_point-FC_var_get(G1_FOAM_WIDTH),end_point);
    }else{
        //  FORK_FIXATION_GAB is the distance fork release before fixation point
        float end_point=CNC_GATE_X_POS - FORK_FIXATION_GAB;
        before_Fixation(end_point-FC_var_get(G2_FOAM_WIDTH),end_point);
    }
}

bool Fixation_main::approachNearFoam(){
    if(Homing.FirstTimeStepExecuting==true){
        if ( ( _motor.currentPosition() < CNC_MAX_Y - RCvar[FOAMYWIDTH] - GAB ) &&
        ( _motor.speed()==0 ) ) {
            _motor.moveTo( CNC_MAX_Y - getValue(&CR_Vars[FOAMYWIDTH]) - GAB  );
            return false;
        }else{
            /// error function call in wrong possiton
        }
    }else{
        if (oneSensorRead() && FixationLimitSensorRead()){
            FoamYWidth_is_Correct = false ;
            // warning
            return false;
        }
        _motor.run();
        return( _motor.distanceToGo()==0 );
    }
}


bool Fixation_main::touchFoam(){
    if(Homing.FirstTimeStepExecuting==true){
        // the maximum tolerance in foam width is +6 or -6 mm
        _motor.setMaxSpeed(MAX_SPEED_HOMING_2);
        _motor.moveTo( CNC_MAX_Y - getValue(&CR_Vars[FOAMYWIDTH]) + 6 );
        return false;
    }else{
        if ( _motor.distanceToGo()==0 ){
            FoamYWidth_is_Correct = false;
            return false;
            // release the foam and send message to reject the foam size
            // warning
            // if the this warning repeated many times (3 times) continuously
            // without any complete fixation. then an error arises.
            // if a complete fixation occures, then it will start count from 0
        }
        _motor.run();
        return ( oneSensorRead() && FixationLimitSensorRead());
        // if there is a promblem in fixation angle
        // then it may sense more than one sensor
    }    
}

bool Fixation_main::oneSensorRead(){
    bool onetouch = false;
    for (int i=0; i<5 ; i++){
        if (sidePlateSensorBool[i]){
            if sidePlateSensor[i].readFiltered(){
                onetouch = true;
                break;
            }
        }
    }
    return onetouch;
}

bool Fixation_main::move2mm(){
    if(Homing.FirstTimeStepExecuting==true){
        _motor.moveTo( _motor.currentPosition() + 2 );
        return false;
    }else{
        _motor.run();
        return ( _motor.distanceToGo()==0 );  
    }
}

bool Fixation_main::fixationEnd(){
    if (!Acceptable_FoamYWidth()){
        FoamYWidth_is_Correct = false ;
        // warning
        return false;
    }

    for (int i=0; i<5 ; i++){
        if (sidePlateSensorBool[i]){
            if (!(sidePlateSensor[i].readFiltered())){
                // warning
            }
        }
    }

    if ( !FixationLimitSensorRead() ){
        // warning
    }

    return true;
 
}



bool Fixation_main::Acceptable_FoamYWidth(){
    Measure_FoamYWidth = CNC_MAX_Y - _motor.currentPosition() + 2; // due to move2mm() 
    float dif = (Measure_FoamYWidth - getValue(&CR_Vars[FOAMYWIDTH]) ) ;
    return ( dif * dif < 25) ;// (5mm * 5mm)
}

bool Fixation_main::FixationLimitSensorRead(){
    if (_num == 1) return FixationG1LimitSensor.readFiltered();
    else if (_num == 2) return FixationG2LimitSensor.readFiltered();
    else{
        // error
    }
}

// Defie objects from the call
Fixation_main Fix_main1(Fixation1Y,1);
Fixation_main Fix_main2(Fixation2Y,2);