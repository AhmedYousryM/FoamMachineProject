#include "MainVariables.h"
#include "Communication.h"
void setup() {
  // put your setup code here, to run once:
  Initial();
  communication.begin('M');
  Serial.begin(57600);  
  communication.SetVar(GlobalVariable,0,(float)100.11);
  Serial.println(GlobalVariable[0].datatype);
  Serial.println(communication.GetVarf(GlobalVariable,0));
  communication.SendVar('C',500);  
  //SendVar('R',595);  
}

void loop() {
  // put your main code here, to run repeatedly:
  communication.Send();
  communication.Receive();
}
