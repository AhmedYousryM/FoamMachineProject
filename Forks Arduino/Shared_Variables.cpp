#include "Shared_Variables.h"  // Your existing header

Variable Shared_Vars[4] = {
  INIT_UCHAR(0),
  INIT_UCHAR(0),
  INIT_UCHAR(0),
  INIT_UCHAR(0)

};

const int NUM_SHARED_VARS = sizeof(Shared_Vars)/sizeof(Shared_Vars[0]);
