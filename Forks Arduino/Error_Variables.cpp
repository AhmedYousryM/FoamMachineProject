#include "Shared_Variables.h"  // Your existing header

Variable Error_Vars[7] = {
  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false)

};

const int NUM_ERROR_VARS = sizeof(Error_Vars)/sizeof(Error_Vars[0]);
