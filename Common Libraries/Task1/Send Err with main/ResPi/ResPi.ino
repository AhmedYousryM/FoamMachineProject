#include "Communication.h"
#include "ResPiVariables.h"
String mm ="Engineer";
void setup() {
  // put your setup code here, to run once:
  Initial();
  communication.begin('R',Serial);
  Serial3.begin(57600);
  SetVar(CRVariable,GCODE_FILE_PATH1,&mm);
  AskVar('M',500);
  Serial3.println(ErrorVariable[1].datatype);
}

void loop() {
  // put your main code here, to run repeatedly:
  communication.Send();
  communication.Receive();
}
