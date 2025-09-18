#ifndef GRBL_Main_h
#define GRBL_Main_h

#include <Arduino.h>
#include <NonBlockingSequence.h>
#include "GRBL_Control.h"
#include "GRBL_Sender.h"

#define VIRTUAL_UPPER_Z_POINT 500

class GRBL_Main: public GRBL_Sender, public GRBL_Control {

    Grbl_Main(byte Head_Num_var, HardwareSerial &grblSerial, const String &filename);
    Grbl_Main(byte Head_Num_var, HardwareSerial &grblSerial);

    void begin();

    // inherits all functions from
    // GRBL_Control & 
    // GRBL_Sender

    // Homing and Lubrication
    bool Homing();
    bool Lubricating();

    // Production and Cutting
    bool Start_Cutting();
    void Stop_Cutting();
    void NewCuttingProgram();
    // void SetSpindleSpeed(float s); 
    // void SetWorkpieceCoordinates(float x, float y, float z);      : GRBL_Sende
    // void SetWorkpieceCoordinatesZ( float z);
    // void SetWorkpieceCoordinatesXY(float x, float y );

    // Start Position
    bool GoTo_StartPos2();
    void Set_StartPos2(float x, float y, float z);

    // Blowering
    bool bigBlowering_NearSensor();
    bool bigBlowering_FarSensor();

    // Z Axis Adjustment
    bool ZAxis_Adjustment_Loop();
    bool IsZAxisAdjustmentFinished();
    void ReAdjustZAxis();


    private :
     
    ClassNonBlockingSequence<Fixation_main> HomingSequence;
    ClassNonBlockingSequence<Fixation_main> LubricationSequence;
    char LubricationRepition = 5;
    ClassNonBlockingSequence<Fixation_main> StartPos2Sequence;
    ClassNonBlockingSequence<Fixation_main> WorkingSequence;
    ClassNonBlockingSequence<Fixation_main> BloweringSequence;

    byte Head_num;
    byte Mode;
    bool Mode_finished=false;

    float start_x;
    float start_y;
    float start_z;

    bool Working_enable=true;

    bool EndZAdjustment = false;

    float MaximumZAxisTravel;

    void ZAxis(bool , bool , bool);
    void AdjustmentEnd(bool b);
    // Step bool functions
        bool Homing1();
        bool Homing2();
        bool StartPos1_1();
        bool StartPos1_2();
        bool Lubrication1();
        bool Lubrication2();
        bool StartPos2_1();
        bool StartPos2_2();
        bool Working1();
        bool Working2();
        bool Working3();
        bool Working4();
        bool Working5();
        bool Working6();
        bool WaitForOK();

}

#endif