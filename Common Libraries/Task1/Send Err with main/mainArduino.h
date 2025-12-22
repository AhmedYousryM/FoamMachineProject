#ifndef MAINARDUINO_H
#define MAINARDUINO_H

#include "Useable.h"
/*
Queue 5 - Res - Serial1
Queue 2 - CnC - Serial2
Queue 3 - Fork - Serial3
*/
class mainArduino{
    public:
    mainArduino(){};
    void mainSendMessage();
    void mainRecieveMessage(HardwareSerial& comserial);
    void main_set_serial_for_queue();
    private:
    void mainsendInfo(Message msg);
    String mainReading(HardwareSerial& comserial,char* readarr );
    Message mainSelectedMessagefromQueues();
    void main_stop_waiting(uint8_t select);
    void main_send(Message msg,uint8_t select);
    void SendingWhat(Message msg,InformationQueue Queue);
    InformationQueue* SelectQueuefromSerial(HardwareSerial& comserial);
    InformationQueue* SelectQueueforOk(uint8_t select);
};
extern mainArduino MainArduino;
extern char Myarr1[MAX_MESSAGE_LENGTH];
extern char Myarr2[MAX_MESSAGE_LENGTH];
extern char Myarr3[MAX_MESSAGE_LENGTH];
#endif