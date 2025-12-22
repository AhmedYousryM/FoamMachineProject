#ifndef COMMUINCATION_H
#define COMMUINCATION_H

#include "Terminal_Devices.h"
#include "mainArduino.h"

class Communication{
    public:
    Communication(){};
    void begin(char DEVICE_ID,HardwareSerial& comserial=Serial1);
    void SendErr(uint16_t ErrorCode);
    void SendWr(uint16_t WarningCode);
    void Send();
    void Receive();
    void AskVar(char destination_device, uint16_t var_index);
    void SetVar(Variable* arr, uint16_t Code, bool val);
    void SetVar(Variable* arr, uint16_t Code, int val);
    void SetVar(Variable* arr, uint16_t Code, float val);
    void SetVar(Variable* arr, uint16_t Code, long val);
    void SetVar(Variable* arr, uint16_t Code, String* val);
    void SetVar(Variable* arr, uint16_t Code, char val);
    bool GetVarb(Variable* arr, uint16_t Code);
    char GetVarc(Variable* arr, uint16_t Code);
    int GetVari(Variable* arr, uint16_t Code);
    float GetVarf(Variable* arr, uint16_t Code);
    long GetVarl(Variable* arr, uint16_t Code);
    String GetVarS(Variable* arr, uint16_t Code);
    void SendVar(char destination_device, uint16_t var_index);
    void InitVar(Variable* arr,uint16_t Code,char datatype);

};
extern Communication communication;

#endif