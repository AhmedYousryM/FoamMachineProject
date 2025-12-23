#include "Shared_Functions.h"
#include "Arduino.h"
// GETTER FUNCTIONS


bool GET_BOOL(Variable* var) {
    if (var->type == VAR_BOOL) {
        return var->value.asBool;
    } else {
        printf("Error: getBoolValue() - Wrong type!\n");
        return false;
    }
}

char GET_CHAR(Variable* var) {
    if (var->type == VAR_CHAR) {
        return var->value.asChar;
    } else {
        printf("Error: getCharValue() - Wrong type!\n");
        return -1;
    }
}

unsigned char GET_UCHAR(Variable* var) {
    if (var->type == VAR_UCHAR)
        return var->value.asUChar;
    else {
        printf("Error: getUCharValue() - Wrong type!\n");
        return 0;
    }
}

int GET_INT(Variable* var) {
    if (var->type == VAR_INT) {
        return var->value.asInt;
    } else {
        printf("Error: getIntValue() - Wrong type!\n");
        return -1;
    }
}

unsigned int GET_UINT(Variable* var) {
    if (var->type == VAR_UINT)
        return var->value.asUInt;
    else {
        printf("Error: getUIntValue() - Wrong type!\n");
        return 0;
    }
}

float GET_FLOAT(Variable* var) {
    if (var->type == VAR_FLOAT) {
        return var->value.asFloat;
    } else {
        printf("Error: getFloatValue() - Wrong type!\n");
        return 0.0f;
    }
}

long GET_LONG(Variable* var) {
    if (var->type == VAR_LONG)
        return var->value.asLong;
    else {
        printf("Error: getLongValue() - Wrong type!\n");
        return -1;
    }
}

unsigned long GET_ULONG(Variable* var) {
    if (var->type == VAR_ULONG)
        return var->value.asULong;
    else {
        printf("Error: getULongValue() - Wrong type!\n");
        return 0;
    }
}

const char* GET_STRING(Variable* var) {
    if (var->type == VAR_STRING)
        return var->value.asString;
    else {
        printf("Error: getStringValue() - Wrong type!\n");
        return "(invalid)";
    }
}
