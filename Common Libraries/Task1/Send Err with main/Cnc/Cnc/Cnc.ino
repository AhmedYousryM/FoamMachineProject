#include "Terminal_Devices.h"
#include "CncArduino2.h"
#include "Variables.h"
 long time ;
 String ms="Omar";
 
void setup(){
  device.device_id='C';
  // put your setup code here, to run once:
  SetSerial(Serial);
  Serial3.begin(57600);
  //InitVar(ErrorVariable,102,'b');
  //InitVar(ErrorVariable,101,'b');
  SendWr(50);
  SendErr(101);
  InitVar(CRVariable,98,'i');
  SetVar(CRVariable,98,300);
  SendVar('R',98);
  InitVar(CRVariable,92,'i');
  SetVar(CRVariable,92,300);
  SendVar('R',92);
  InitVar(CRVariable,97,'S');
  SetVar(CRVariable,97,&ms);
  //SendVar('R',97);  
  //AskVar('R',97);
  InitVar(CMVariable,2,'i');
  SetVar(CMVariable,2,100);
  //SendVar('M',2);  
  InitVar(GlobalVariable,95,'f');//595
}

void loop() {
  // put your main code here, to run repeatedly:
  SendMessage();
  RecieveMessage(*device.stream);
  }
  

