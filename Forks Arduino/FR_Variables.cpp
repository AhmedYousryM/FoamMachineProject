#include "FR_Variables.h"

Variable FR_Vars[32] = {
  INIT_INT(1400),
  INIT_INT(2000),
  INIT_INT(1450),
  INIT_INT(1900),
  INIT_INT(2000),
  INIT_FLOAT(325.5),
  INIT_FLOAT(643.8),
  INIT_FLOAT(150.4),

  INIT_UCHAR(0),

  INIT_BOOL(false),
  INIT_BOOL(false),

  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),

  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),

  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),

  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),
  INIT_BOOL(false),

  INIT_BOOL(false),
  INIT_BOOL(false),

  INIT_BOOL(false),
  INIT_BOOL(false),

  INIT_BOOL(false),
  INIT_BOOL(false),

  INIT_BOOL(false),
  INIT_BOOL(false)

  // Add more variables here...
};

const int NUM_FR_VARS = sizeof(FR_Vars)/sizeof(FR_Vars[0]);
