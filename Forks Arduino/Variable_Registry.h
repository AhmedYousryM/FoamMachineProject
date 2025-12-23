/*#ifndef VARIABLE_REGISTRY_H
#define VARIABLE_REGISTRY_H

#include "Variables_Structure.h"

// Declare all your variable arrays here
extern Variable Shared_Var[];
extern const int NUM_SHARED_VARS;

extern Variable FC_Vars[];
extern const int NUM_FC_VARS;

extern Variable FM_Vars[];
extern const int NUM_FM_VARS;

extern Variable FR_Vars[];
extern const int NUM_FR_VARS;

typedef struct {
    Variable* array;
    int size;
} VariableArray;

// Register all arrays
const VariableArray registeredArrays[] = {
    {Shared_Var, NUM_SHARED_VARS},
    {FC_Vars, NUM_FC_VARS},
    {FM_Vars, NUM_FM_VARS},
    {FR_Vars, NUM_FR_VARS}
    // Add more arrays here
};

const int NUM_REGISTERED_ARRAYS = sizeof(registeredArrays)/sizeof(registeredArrays[0]);

#endif*/