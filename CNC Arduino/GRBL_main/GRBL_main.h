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
     
    NonBlockingSequence HomingSequence;
    NonBlockingSequence LubricationSequence;
    char LubricationRepition = 5;
    NonBlockingSequence StartPos2Sequence;
    NonBlockingSequence WorkingSequence;
    NonblockingSequence BloweringSequence;

    byte Head_num;
    byte Mode;
    bool Mode_finished=false;

    float start_x;
    float start_y;
    float start_z;

    bool Working_enable=true;

    bool EndZAdjustment = false;

    float MaximumZAxisTravel;

}

#endif