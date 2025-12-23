#include "FC_Variables.h"  // Your existing header

Variable FC_Vars[2] = {
  INIT_FLOAT(254.2),
  INIT_INT(145)
  // Add more variables here...
};

const int NUM_FC_VARS = sizeof(FC_Vars)/sizeof(FC_Vars[0]);
