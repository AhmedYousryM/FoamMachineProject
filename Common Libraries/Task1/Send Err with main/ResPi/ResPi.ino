#include "Terminal_Devices.h"
#include "ResPiArduino2.h"
#include "Variables.h"
String mm ="Engineer";
void setup() {
  // put your setup code here, to run once:
  device.device_id='R';
  SetSerial(Serial);
  Serial3.begin(57600);
  InitVar(ErrorVariable,50,'b');
  InitVar(ErrorVariable,101,'b');
  InitVar(CRVariable,98,'i');
  InitVar(CRVariable,97,'S');
  SetVar(CRVariable,97,&mm);
  InitVar(GlobalVariable,95,'f');
  AskVar('M',595);
}

void loop() {
  // put your main code here, to run repeatedly:
  SendMessage();
  RecieveMessage(*device.stream);
}
