#include "mainArduino.h"
#include "Variables.h"
#include "Main.h"
int i=1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);  
  main_set_serial_for_queue();
  device.device_id='M';
  InitVar(CMVariable,2,'i');
  InitVar(GlobalVariable,95,'f');
  float ss=100.12;
  SetVar(GlobalVariable,95,(float)100.12);
  SendVar('C',595);  
  //SendVar('R',595);  
}

void loop() {
  // put your main code here, to run repeatedly:
  mainSendMessage();
  mainRecieveMessage(Serial2);
  mainRecieveMessage(Serial1);
  mainRecieveMessage(Serial3);
  if(i==1){
    //SendVar('C',595);
    i=2;
  }
}
