#include "Shared_Variables.h"  // Your existing header

Variable Warning_Vars[4] = {
  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false)

};

const int NUM_WARNING_VARS = sizeof(Warning_Vars)/sizeof(Warning_Vars[0]);


//getBoolValue(&CR_Variables[Frame_G1_]])