#ifndef GRBL_Main_h
#define GRBL_Main_h

#include <Arduino.h>
#include <NonBlockingSequence.h>
#include "GRBL_Control.h"
#include "GRBL_Sender.h"
#include "CNCsensors.h"
#include "Fixation_main.h"

#define VIRTUAL_UPPER_Z_POINT 500

class GRBL_Main: public GRBL_Sender, public GRBL_Control {
    public:
    //Grbl_Main(byte Head_Num_var, HardwareSerial &grblSerial, const String &filename);
    GRBL_Main(byte Head_Num_var, HardwareSerial &grblSerial);

    void begin();
    unsigned long startPressTime=0;
    char flag_char = 0;
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
    //void SetWorkpieceCoordinates();
    // Start Position
    bool GoTo_StartPos2();
    bool GoTo_StartPos1();
    void Restart_GoTo_StartPos2();
    void Restart_GoTo_StartPos1();

    // Blowering
    bool bigBlowering();
    void RestartBigBlowering();
    // Z Axis Adjustment
    bool ZAxis_Adjustment_Loop();
    bool IsZAxisAdjustmentFinished();
    //void ReAdjustZAxis();

    bool beforeFixating();
    bool MotionReset = true;

    bool ZSensorHOmingFlagFun();
    

    private :
    ClassNonBlockingSequence<GRBL_Main> HomingSequence;
    ClassNonBlockingSequence<GRBL_Main> LubricationSequence;
    char LubricationRepition = 5;
    ClassNonBlockingSequence<GRBL_Main> StartPos2Sequence;
    ClassNonBlockingSequence<GRBL_Main> StartPos1Sequence;
    ClassNonBlockingSequence<GRBL_Main> WorkingSequence;
    ClassNonBlockingSequence<GRBL_Main> BloweringSequence;
 
    
    byte Head_num;
    byte Mode;
    bool Mode_finished=false;

    float start_x;
    float start_y;
    float start_z;
    float pointx_half_foam;
    bool Working_enable=true;
    bool sender_begin();

    bool EndZAdjustment = false;

    float MaximumZAxisTravel;

    void Set_StartPos2(float x, float y, float z);
    bool start_pos_set_done = false;

    void ZAxis(bool Up,bool Down,bool Zero);
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

        bool ZSensorHomingFlag = false;

};

extern GRBL_Main GRBL_main1;
extern GRBL_Main GRBL_main2;

#endif