#ifndef TERMINAL_DEVICES_H
#define TERMINAL_DEVICES_H

#include "Useable.h"
class Terminal_Devices{
    public:
    Terminal_Devices(){};
    void RecieveMessage(HardwareSerial& comserial);
    void SendMessage();
    
    private:
    String Reading(HardwareSerial& comserial,char* readarr);
    void sendERR(Message msg);
    void sendInfo(Message msg);
    void send(Message msg,uint8_t select);
    void stop_waiting(uint8_t select,InformationQueue* Queue_pt=&Queue1);

};
extern Terminal_Devices Terminal;
#endif