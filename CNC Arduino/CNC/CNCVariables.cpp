#include "CNCVariables.h"
Variable GlobalVariable[1]={}; // 7
Variable CFVariable[7]={}; // 1

Variable CRVariable[46]={}; // 2
Variable CMVariable[6]={}; // 3

// unused
Variable ErrorVariable[1]={};
Variable RFVariable[1]={}; // 4
Variable RMVariable[1]={}; // 5
Variable FMVariable[1]={}; // 6

void Initial(){
    // ------ CF Variable -------
    InitVar(CFVariable,G1_FIXATION_DOING,'b');
    InitVar(CFVariable,G2_FIXATION_DOING,'b');
    InitVar(CFVariable,G1_CUTTING_DOING,'b');
    InitVar(CFVariable,G2_CUTTING_DOING,'b');
    InitVar(CFVariable,G1_FOAM_WIDTH,'f');
    InitVar(CFVariable,G2_FOAM_WIDTH,'f');
    InitVar(CFVariable,FORK_FIXATION_POINT_2_X,'f');
    // ------ CF Variable -------
    InitVar(CRVariable,ZAXIS_SETUP_DONE,'b');
    InitVar(CRVariable,GCODE_FILE_PATH1,'S');
    InitVar(CRVariable,GCODE_FILE_PATH2,'S');
    InitVar(CRVariable,HEAD1_START_POSITION_X,'f');
    InitVar(CRVariable,HEAD1_START_POSITION_Y,'f');
    InitVar(CRVariable,HEAD2_START_POSITION_X,'f');
    InitVar(CRVariable,HEAD2_START_POSITION_Y,'f');
    InitVar(CRVariable,FOAM_HEIGHT,'f');
    InitVar(CRVariable,FOAMYWIDTH,'f');
    InitVar(CRVariable,FIXATION_POINT_2_X,'f');
    InitVar(CRVariable,CNC_SENSOR_PAGE,'i');
    InitVar(CRVariable,SIDE_PLATE_LM_1_SENSOR,'b');
    InitVar(CRVariable,SIDE_PLATE_LM_2_SENSOR,'b');
    InitVar(CRVariable,SIDE_PLATE_LM_3_SENSOR,'b');
    InitVar(CRVariable,SIDE_PLATE_LM_4_SENSOR,'b');
    InitVar(CRVariable,SIDE_PLATE_LM_5_SENSOR,'b');
    InitVar(CRVariable,FIXATION_G1_LM,'b');
    InitVar(CRVariable,FIXATION_G2_LM,'b');
    InitVar(CRVariable,HEAD_G1_Z_UP,'b');
    InitVar(CRVariable,HEAD_G1_Z_DOWN,'b');
    InitVar(CRVariable,HEAD_G2_Z_UP,'b');
    InitVar(CRVariable,HEAD_G2_Z_DOWN,'b');
    InitVar(CRVariable,FRAME_G1_X,'b');
    InitVar(CRVariable,G1_G2_X,'b');
    InitVar(CRVariable,G2_FRAME_X,'b');
    InitVar(CRVariable,FRAME_FIX_Y_G1,'b');
    InitVar(CRVariable,FIX_HEAD_Y_G1,'b');
    InitVar(CRVariable,HEAD_FRAME_Y_G1_1,'b');
    InitVar(CRVariable,HEAD_FRAME_Y_G1_2,'b');
    InitVar(CRVariable,FRAME_FIX_Y_G2,'b');
    InitVar(CRVariable,FIX_HEAD_Y_G2,'b');
    InitVar(CRVariable,HEAD_FRAME_Y_G2_1,'b');
    InitVar(CRVariable,HEAD_FRAME_Y_G2_2,'b');
    InitVar(CRVariable,MOVE_DIRECTION,'b');
    InitVar(CRVariable,MOVE_SPEED,'i');
    InitVar(CRVariable,MOVE_G1_X,'b');
    InitVar(CRVariable,MOVE_HEAD_G1_Y,'b');
    InitVar(CRVariable,MOVE_HEAD_G1_Z,'b');
    InitVar(CRVariable,MOVE_FIX_G1_Y,'b');
    InitVar(CRVariable,MOVE_G2_X,'b');
    InitVar(CRVariable,MOVE_HEAD_G2_Y,'b');
    InitVar(CRVariable,MOVE_HEAD_G2_Z,'b');
    InitVar(CRVariable,MOVE_FIX_G2_Y,'b');
    // ------ CM Variable -------
    InitVar(CMVariable,MODE,'i');
    InitVar(CMVariable,NOMODE_SUBMODES,'i');
    InitVar(CMVariable,PREPRODUCTION_SUBMODES,'i');
    InitVar(CMVariable,PRODUCTION_SUBMODES,'i');
    InitVar(CMVariable,CNC_OPERATION_DOING,'i');
    InitVar(CMVariable,FORK_START_HOMING,'i');
}
