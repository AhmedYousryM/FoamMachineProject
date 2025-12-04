#ifndef TERMINAL_DEVICES_H
#define TERMINAL_DEVICES_H

#include "Useable.h"
String Reading(HardwareSerial& comserial);
void sendERR(Message msg);
void sendInfo(Message msg);
void SendMessage();
void RecieveMessage(HardwareSerial& comserial);
void send(Message msg,uint8_t select);
void stop_waiting(uint8_t select,InformationQueue* Queue_pt=&Queue1);

#endif