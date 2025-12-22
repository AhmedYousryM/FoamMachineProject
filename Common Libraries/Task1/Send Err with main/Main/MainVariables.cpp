#include "MainVariables.h"
Variable GlobalVariable[1]={}; // 7
Variable CMVariable[6]={}; // 3
Variable RMVariable[1]={}; // 5
Variable FMVariable[1]={}; // 6
// unused
Variable ErrorVariable[1]={};
Variable RFVariable[1]={}; // 4
Variable CFVariable[1]={}; // 1
Variable CRVariable[1]={}; // 2


void Initial(){
    // ------ CM Variable -------
    InitVar(CMVariable,MODE,'i');
    InitVar(CMVariable,NOMODE_SUBMODES,'i');
    InitVar(CMVariable,PREPRODUCTION_SUBMODES,'i');
    InitVar(CMVariable,PRODUCTION_SUBMODES,'i');
    InitVar(CMVariable,CNC_OPERATION_DOING,'i');
    InitVar(CMVariable,FORK_START_HOMING,'i');
    // ----- Global Var ----------
    InitVar(GlobalVariable,0,'f');
}
