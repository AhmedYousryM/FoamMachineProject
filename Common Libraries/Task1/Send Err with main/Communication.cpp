#include "Communication.h"
struct Device device={};
Message ErrorQueue[ERROR_QUEUE_SIZE];
char readdarr[MAX_MESSAGE_LENGTH];
struct InformationQueue Queue1;
struct InformationQueue Queue2;
struct InformationQueue Queue3;
struct InformationQueue Queue5;
InformationQueue* queuepointer;
char Myarr1[MAX_MESSAGE_LENGTH]={};
char Myarr2[MAX_MESSAGE_LENGTH]={};
char Myarr3[MAX_MESSAGE_LENGTH]={};

void Communication::begin(char DEVICE_ID,HardwareSerial& comserial){
    device.device_id=DEVICE_ID;
    if (device.device_id=='M'){
     MainArduino.main_set_serial_for_queue();
    }else{
     device.stream=&comserial;
     device.stream->begin(SERIAL_BAUD_RATE);
    }
       
}
void Communication::SendVar(char destination_device, uint16_t var_index){
    // Create message string: "DeviceIndex=Value"
    String msg = String(device.device_id) + String(destination_device) + String(var_index) + "=";
    if(device.device_id=='M'){
    build_message(msg,Queue1.MessageQueue,20);
    }
    else{
      InformationQueue* Queue_ptr = SelectQueue(msg);
      build_message(msg,Queue_ptr->MessageQueue,20);
    } 
}
void Communication::Send(){
  if (device.device_id!='M'){Terminal.SendMessage();}
  else{MainArduino.mainSendMessage();}
}
void Communication::Receive(){
  if (device.device_id!='M'){Terminal.RecieveMessage(*device.stream);}
  else{
    MainArduino.mainRecieveMessage(Serial2);
    MainArduino.mainRecieveMessage(Serial1);
    MainArduino.mainRecieveMessage(Serial3);
  }
}
void Communication::SendErr(uint16_t ErrorCode){
    String msg = "Er" + String(ErrorCode);
    build_message(msg,ErrorQueue,1);
}
void Communication::SendWr(uint16_t WarningCode){
    String msg = "Wr" + String(WarningCode);
    build_message(msg,ErrorQueue,2);
}
void Communication::InitVar(Variable* arr,uint16_t Code,char datatype){
  arr[Code].datatype=datatype;
}
void Communication::SetVar(Variable* arr, uint16_t Code, bool val){
  arr[Code].value.boolvalue=val;
}
void Communication::SetVar(Variable* arr, uint16_t Code, int val){
  arr[Code].value.intvalue=val;
}
void Communication::SetVar(Variable* arr, uint16_t Code, float val){
  arr[Code].value.floatvalue=val;
}
void Communication::SetVar(Variable* arr, uint16_t Code, long val){
  arr[Code].value.longvalue=val;
}
void Communication::SetVar(Variable* arr, uint16_t Code, String* val){
  arr[Code].value.stringPtr=val;
}
void Communication::SetVar(Variable* arr, uint16_t Code, char val){
  arr[Code].value.charvalue=val;
}

bool Communication::GetVarb(Variable* arr, uint16_t Code){
        if(Code>=500){Code=Code-500;}
        return arr[Code].value.boolvalue;
}
char Communication::GetVarc(Variable* arr, uint16_t Code){
        if(Code>=500){Code=Code-500;}
        return arr[Code].value.charvalue;
}
int Communication::GetVari(Variable* arr, uint16_t Code){
        if(Code>=500){Code=Code-500;}
        return arr[Code].value.intvalue;
}
float Communication::GetVarf(Variable* arr, uint16_t Code){
        if(Code>=500){Code=Code-500;}
        return arr[Code].value.floatvalue;
}
long Communication::GetVarl(Variable* arr, uint16_t Code){
    
        if(Code>=500){Code=Code-500;}
        return arr[Code].value.longvalue;

}
String Communication::GetVarS(Variable* arr, uint16_t Code){
        if(Code>=500){Code=Code-500;}
        return *arr[Code].value.stringPtr;
}

void Communication::AskVar(char destination_device, uint16_t var_index){
     // Create message string: "DeviceIndex=Value"
    String msg = String(device.device_id) + String(destination_device) + String(var_index) + "?";
    InformationQueue* Queue_ptr = SelectQueue(msg);
    Queue_ptr->Expected_answer_index=var_index;
    build_message(msg,Queue_ptr->MessageQueue,20);
}
Communication communication;