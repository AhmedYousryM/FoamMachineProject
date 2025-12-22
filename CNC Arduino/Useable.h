#ifndef USEABLE_H
#define USEABLE_H

#include "MyConfig.h"

/*
C-M:1
C-R:2
C-F:3

R-M:1
R-C:2
R-F:3

F-M:1
F-R:2
F-C:3

through main:
// FC101=10 - bridge
// FM102=10 = device
  RC || CR Queue 1
  CF || FC Queue 2
  FR || RF Queue 3
  main messages Queue5 device
*/
struct Variable{
  char datatype; // b S i f L
  union 
  {
    char charvalue;
    bool boolvalue;
    int intvalue;
    float floatvalue;
    long longvalue;
    String* stringPtr;
    byte byte_array[sizeof(float)]={};
  }value;
  
};
struct Message {
  String Content; // Er102
  uint8_t XorCheck; //
  uint8_t SumCheck;
  uint8_t Priority;
};
struct Device{
  char device_id; // 'c' ,'M',...
  uint8_t no_repeated_Errmessage = 0; // max is 3 tries
  bool Waiting_for_ErrOk=false;
  bool Waiting_for_WrOk=false;
  bool Waiting_for_Ok=false;
  long last_sent_time;
  HardwareSerial*  stream = &Serial;
};
struct InformationQueue{
  Message MessageQueue[QUEUE_SIZE_PER_DEVICE];
  bool Waiting_for_ResOk;
  bool Waiting_for_UpdateOk;
  bool Waiting_for_ErrOK;
  bool Waiting_for_WrOK;
  uint8_t no_repeated_infomessage;
  double last_sent_time;
  uint16_t Expected_answer_index;
  HardwareSerial*  stream = &Serial; 
};
const InformationQueue Queue4={};

// ------ serial -----------
void SetSerial(HardwareSerial& comserial);
void InitVar(Variable* arr,uint16_t Code,char datatype);
void SetVar(Variable* arr, uint16_t Code, bool val);
void SetVar(Variable* arr, uint16_t Code, int val);
void SetVar(Variable* arr, uint16_t Code, float val);
void SetVar(Variable* arr, uint16_t Code, long val);
void SetVar(Variable* arr, uint16_t Code, String* val);
void SetVar(Variable* arr, uint16_t Code, char val);
//------- Message Processing -------
uint8_t Getlength(char* msg);// get message content length including null terminator
Message Rebuildmsg(char* msg); // use incoming message to make a member of Message struct
bool Checksum_Validation(char* msg,Message Msg);// check if checksum is true or not
void build_message(String msg,Message* messages,uint8_t Priority); // add message string to Message struct and add to Queue
uint8_t PriorityCalc(String msg,char* rarr={});
uint8_t getLoc(String msg);
Message Checksum_calc(Message message);// calculate Checksum
bool isindex(uint16_t code,uint8_t select);// check if variable index is in any of variable arraies
void removemessage(Message* messages,uint8_t Priority);// remove message from Queue
InformationQueue* SelectQueue(String msg); // return Queue of information (1,2,3) 
Variable* SelectVarArr(String msg);// Select Variable array as CFVariable
uint8_t Selectindex(String msg);// Return values each reprensent a select key for Condition determine 
bool Answer_index_check(String msg_str);
void updatelasttime(Message SelectedMessage);
// ------- Error Category Messages -------
void RecieveErrOkWrOk(String msg); // fcn remove Err/Wr from Queue
void RecieveErr_Wr(uint16_t Code,String msg); // fcn handle Err/Wr and send Ok 
//-------- Error Messages ---------
void ErrorHandling(uint16_t ErrorCode); // to be modifed be error team 
void removeErr(); // remove last error message (first in queue)
void SendErrWt(); // add Error What Message (at end of queue)
void SendErrOk(); // add Send Error Ok Message (at end of queue)
void SendErr(uint16_t ErrorCode); // add error message (at end of queue)
// ------- Warning Messages -------
void removeWr();
void WarningHandling(uint16_t WarningCode); // to be modifed be Warning team
void SendWrOk();// add Send Warning Ok Message (at end of queue)
void SendWrWt();// add Send Warning what Message (at end of queue)
void SendWr(uint16_t WarningCode); // add warning message
// ------- information Messages -----
void RecieveInfoWt(InformationQueue Queue); // Stop program or disable waiting and increase a try
void RecieveInfoOk(String msg_str); // fcn disable waiting and remove message from Queue
void RecieveInfoRes(String msg_str,char* readarr); // fcn disable waiting and remove message from Queue
void RecieveInfoUpdate(uint16_t Code, String msg,String msg_str,char* readarr); // fcn determines which var arr to use and update var value
void RecieveInfoQuestion(uint16_t Code, String msg);// Select VarArr and select index then send response to Question
void UpdateVar(uint16_t Code,uint8_t select,String msg_str,Variable* Vararr,char* readarr); // fcn to update var value at specific var arr
void SendOk(String msg_str,Message* Vararr); // send ok to be queued in a var arr
void SendOkhandleing(String msg_str); // select a queue and send Ok to be queued
void SendQesReshandleing(String msg_str,uint16_t Code,uint8_t select);// select a queue and send Res to be queued
void SendQesRes(String msg_str,Message* messageQueue);//send Res to be queued
void SendWt(String msg_str,Message* Vararr);// send Wt to be queued
void SendWthandleing(String msg_str);// select Queue and send Wt to be queued
void AskVar(char destination_device, uint16_t var_index);
char GetVar(Variable* Vararr, uint16_t var_index);
// ------- Queuing --------------
uint8_t queueLength(Message* QueueList,uint8_t select);// length of Queue + 1(ex if empty returns 1)
void queuing(Message message,Message* messages);// add message to Queue
Message SelectMessage(Message* QueueList,uint8_t select,InformationQueue Queue=Queue4);// select message from a Queue depending on priority
Message SelectedMessagefromQueues();
// ------ Extern Variables -------
extern struct Device device;
extern Message ErrorQueue[ERROR_QUEUE_SIZE];
extern struct InformationQueue Queue1;
extern struct InformationQueue Queue2;
extern struct InformationQueue Queue3;
extern struct InformationQueue Queue5;
extern Variable GlobalVariable[]; //8
extern Variable CFVariable[]; // 1
extern Variable CRVariable[]; // 2
extern Variable CMVariable[]; // 3
extern Variable RFVariable[]; // 4
extern Variable RMVariable[]; // 5
extern Variable FMVariable[]; // 6
extern Variable ErrorVariable[]; // 7
extern struct InformationQueue *queuepointer;
// ------ Stop fcn -----------
void stop_program();
void SendVar(char destination_device, uint16_t var_index);

#endif