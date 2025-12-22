//#include "Terminal_Devices.h"
//#include "CncArduino2.h"
#include "CNCVariables.h"
#include "Communication.h"
 long time ;
  String ms="GCode.txt";
void setup(){
  Initial();
  communication.begin('C',Serial);
  Serial3.begin(57600);

  communication.SendWr(0);
  communication.SendErr(1);

  communication.SetVar(CRVariable,CNC_SENSOR_PAGE,3);
  communication.SendVar('R',CNC_SENSOR_PAGE);

  communication.SetVar(CRVariable,FIXATION_POINT_2_X,(float)300.2);
  communication.SendVar('R',FIXATION_POINT_2_X);

  
  communication.SetVar(CRVariable,GCODE_FILE_PATH1,&ms);
  //Serial3.println(*(CRVariable[GCODE_FILE_PATH1].value.stringPtr));
  communication.SendVar('R',GCODE_FILE_PATH1);
  //SendVar('R',97);  
  //AskVar('R',97);
  //InitVar(CMVariable,2,'i');
  communication.SetVar(CMVariable,MODE,2);
  //communication.SendVar('M',MODE);  
  //InitVar(GlobalVariable,95,'f');//595
  //Serial3.println(GlobalVariable[0].datatype);
  //Serial3.println(GlobalVariable[0].value.floatvalue);
}

void loop() {
  // put your main code here, to run repeatedly:
  communication.Send();
  communication.Receive();
}
  

