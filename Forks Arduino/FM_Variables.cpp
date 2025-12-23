#include "FM_Variables.h"

Variable FM_Vars[1] = {
  INIT_BOOL(false)
  // Add more variables here...
};

const int NUM_FM_VARS = sizeof(FM_Vars)/sizeof(FM_Vars[0]);
