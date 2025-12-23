#ifndef SHARED_FUNCTIONS_H
#define SHARED_FUNCTIONS_H

#include <string.h>  // Add this line for strcmp()
#include "Variables_Structure.h"


// TYPE-SAFE GETTERS ---------------------------------------------------
bool GET_BOOL(Variable* var);

int GET_INT(Variable* var);

unsigned int GET_UINT(Variable* var);

float GET_FLOAT(Variable* var);

long GET_LONG(Variable* var);

unsigned long GET_ULONG(Variable* var);

char GET_CHAR(Variable* var);

unsigned char GET_UCHAR(Variable* var);

const char* GET_STRING(Variable* var);

#endif