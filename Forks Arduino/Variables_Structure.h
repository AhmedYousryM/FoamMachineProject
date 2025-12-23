#ifndef VARIABLES_STRUCTURE_H
#define VARIABLES_STRUCTURE_H

#include <stdbool.h>
#include <stddef.h>

// Define variable types
typedef enum {
    VAR_BOOL, VAR_CHAR, VAR_UCHAR, VAR_INT, VAR_UINT, VAR_FLOAT, VAR_LONG, VAR_ULONG, VAR_STRING
} VarType;

// Variable struct (with union for values)
typedef struct {
    VarType type;
    union {
        bool asBool;
        char asChar;
        unsigned char asUChar;
        int asInt;
        unsigned int asUInt;
        float asFloat;
        long asLong;
        unsigned long asULong;
        const char* asString;
    } value;
} Variable;

// Helper macros for cleaner initialization
#define INIT_BOOL(val)     {VAR_BOOL,   {.asBool = (val)}}
#define INIT_CHAR(val)     {VAR_CHAR,   {.asChar = (val)}}
#define INIT_UCHAR(val)    {VAR_UCHAR,  {.asUChar = (val)}}
#define INIT_INT(val)      {VAR_INT,    {.asInt = (val)}}
#define INIT_UINT(val)     {VAR_UINT,   {.asUInt = (val)}}
#define INIT_FLOAT(val)    {VAR_FLOAT,  {.asFloat = (val)}}
#define INIT_LONG(val)     {VAR_LONG,   {.asLong = (val)}}
#define INIT_ULONG(val)    {VAR_ULONG,  {.asULong = (val)}}
#define INIT_STRING(val)   {VAR_STRING, {.asString = (val)}}

// Setter macros for struct instance
#define SET_BOOL(var, val)     ((var).value.asBool = (val))
#define SET_CHAR(var, val)     ((var).value.asChar = (val))
#define SET_UCHAR(var, val)    ((var).value.asUChar = (val))
#define SET_INT(var, val)      ((var).value.asInt = (val))
#define SET_UINT(var, val)     ((var).value.asUInt = (val))
#define SET_FLOAT(var, val)    ((var).value.asFloat = (val))
#define SET_LONG(var, val)     ((var).value.asLong = (val))
#define SET_ULONG(var, val)    ((var).value.asULong = (val))
#define SET_STRING(var, val)   ((var).value.asString = (val))

#endif