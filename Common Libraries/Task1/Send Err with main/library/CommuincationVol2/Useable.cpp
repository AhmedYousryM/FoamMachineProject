#include "Useable.h"

// ----- Serial ------------
void SetSerial(HardwareSerial& comserial){
  device.stream=&comserial;
  device.stream->begin(SERIAL_BAUD_RATE);
}
void InitVar(Variable* arr,uint16_t Code,char datatype){
  arr[Code].datatype=datatype;
}
void SetVar(Variable* arr, uint16_t Code, bool val){
  arr[Code].value.boolvalue=val;
}
void SetVar(Variable* arr, uint16_t Code, int val){
  arr[Code].value.intvalue=val;
}
void SetVar(Variable* arr, uint16_t Code, float val){
  arr[Code].value.floatvalue=val;
}
void SetVar(Variable* arr, uint16_t Code, long val){
  arr[Code].value.longvalue=val;
}
void SetVar(Variable* arr, uint16_t Code, String* val){
  arr[Code].value.stringPtr=val;
}
void SetVar(Variable* arr, uint16_t Code, char val){
  arr[Code].value.charvalue=val;
}
// ----- Message Processing --------
uint8_t Getlength(char* msg){
  uint8_t loc;
  for(int i=0;i<=MAX_MESSAGE_LENGTH;i++){
    if(msg[i]=='x'){
      loc= i;
    }
  }
  return loc;
}
Message Rebuildmsg(char* msg){
  struct Message MSG;
  uint8_t len = Getlength(msg);// Er102JJ'\0'
  if(len<=8){
    MSG.Content =String(msg).substring(0,len-2);
  }else{
    MSG.Content=String(msg).substring(0,getLoc(String(msg))+1);
    /*switch (len){
      case 11:
      MSG.Content = String(msg).substring(0,len-6);
      break;
      case 10:
      MSG.Content = String(msg).substring(0,len-5);
      break;
      case 9:
      MSG.Content = String(msg).substring(0,len-4);
      break;
      default:
      MSG.Content=String(msg).substring(0,getLoc(String(msg))+1);
      break;
    }*/
  }//cr98=ahmedhesham!!x
  MSG.Priority = PriorityCalc(MSG.Content,msg);
  MSG.XorCheck = (uint8_t)msg[len-2];
  MSG.SumCheck = (uint8_t)msg[len-1];  
  return MSG;
}
bool Checksum_Validation(char* msg,Message Msg){
    uint8_t XorCheck = 0;
    uint8_t SumCheck = 0;
    uint8_t len = Getlength(msg);
      for(int i = 0; i < len-2; i++) {
        XorCheck ^= msg[i]; // E^W- Er102 -- E^r^1^0^2
        SumCheck += msg[i];    
      }
    if(XorCheck==Msg.XorCheck&&SumCheck==Msg.SumCheck){return true;}
    else{return false;}
}
Message Checksum_calc(Message message) {
    message.XorCheck = 0;
    message.SumCheck = 0;
    for(int i = 0; i < message.Content.length(); i++) {
        message.XorCheck ^= message.Content[i];
        message.SumCheck += message.Content[i];    
    }
    if(message.Priority==15||(message.Priority==20&&message.Content[getLoc(message.Content)]=='=')){
      Variable* arr =SelectVarArr(message.Content);
      uint16_t Code = message.Content.substring(2,getLoc(message.Content)).toInt(); 
      if(Code>=500){
        Code=Code-500;
        arr=GlobalVariable;
      }
      switch(arr[Code].datatype){
        case 'b':
        case 'c':
          for(int i = 0; i < 1; i++) {
            message.XorCheck ^= arr[Code].value.byte_array[i];
            message.SumCheck += arr[Code].value.byte_array[i];
          }  
        break;
        // CR102
        case 'i':
          for(uint8_t i=0;i<2;i++){
            message.XorCheck ^= arr[Code].value.byte_array[i];
            message.SumCheck += arr[Code].value.byte_array[i];    
          }
        break;
        case 'f':
        case 'l':
          for(uint8_t i=0;i<4;i++){
          message.XorCheck ^= arr[Code].value.byte_array[i];
            message.SumCheck += arr[Code].value.byte_array[i];    
          }  
        break;
        case 'S':
          String perm=*(arr[Code].value.stringPtr);
          Serial3.println(perm);
          for(uint8_t i=0;i<perm.length();i++){
          message.XorCheck ^= perm[i];
          message.SumCheck += perm[i];    
          }
        break;
      }
      
    }
    return message;
}
uint8_t PriorityCalc(String msg,char* rarr){
  switch(msg[0]){
    case 'E': // Er101 - ErWt - ErOk
      switch(msg[2]){
        case 'O': //ErOk
        return 4;
        break;
        case 'W': //ErWt
        return 3;
        break;
        default: //Er101
        return 1;
        break;
      }
    break;
    case 'W': // Wr101 - WrWt - WrOk
      switch(msg[2]){
        case 'O': //WrOk
        return 6;
        break;
        case 'W': //WrWt
        return 5;
        break;
        default: //Wr101
        return 2;
        break;
      }
    break;
    default: // information (FCWt-FCOk-RC045?=25-CR045?-FC136=1)
      switch(msg[2]){
          case 'W': //FCWt
          return 10;
          break;
          case 'O': //FCOk
          return 11;
          break;
          default: //(RC045?25-CR045?-FC136=1)
          if(msg[getLoc(msg)]=='?'&&msg[Getlength(rarr)-3]!='?'){ // RC045?25ccx 
            return 15;
          }else{ // CR045?ccx or FC136=1
            return 20; 
          }        
          break;
        }
      break;
    break;
  }
}
void build_message(String msg,Message* messages,uint8_t Priority){
    // Check message length
    if(msg.length() > MAX_MESSAGE_LENGTH) {
     // what to do
    }else{
      struct Message message ={};
      message.Content=msg;
      message.Priority = Priority;  
      message=Checksum_calc(message);
      queuing(message,messages);
    }
     
} 
bool isindex(uint16_t code,uint8_t select){
  if(select==1){ // CF OR FC
  // ErrorVar -> Er101 --- ErrorVar[101]=
    if(code <= 499){
      if(CFVariable[code].datatype=='b'||CFVariable[code].datatype=='i'||CFVariable[code].datatype=='f'||CFVariable[code].datatype=='S'||CFVariable[code].datatype=='l'||CFVariable[code].datatype=='c'){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    } 
  }else if (select==2){ // CR OR RC
    if(code <= 499){
      if(CRVariable[code].datatype=='b'||CRVariable[code].datatype=='i'||CRVariable[code].datatype=='f'||CRVariable[code].datatype=='S'||CRVariable[code].datatype=='l'||CRVariable[code].datatype=='c'){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    } 
  }else if (select==3){ // CM OR MC
    if(code <= 499){
      if(CMVariable[code].datatype=='b'||CMVariable[code].datatype=='i'||CMVariable[code].datatype=='f'||CMVariable[code].datatype=='S'||CMVariable[code].datatype=='l'||CRVariable[code].datatype=='c'){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    } 
  }else if (select==4){ // CF OR FC
    if(code <= 499){
      if(RFVariable[code].datatype=='b'||RFVariable[code].datatype=='i'||RFVariable[code].datatype=='f'||RFVariable[code].datatype=='S'||RFVariable[code].datatype=='l'||RFVariable[code].datatype=='c'){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    } 
  }else if (select==5){ // CF OR FC
    if(code <= 499){
      if(RMVariable[code].datatype=='b'||RMVariable[code].datatype=='i'||RMVariable[code].datatype=='f'||RMVariable[code].datatype=='S'||RMVariable[code].datatype=='l'||RMVariable[code].datatype=='c'){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    } 
  }else if (select==6){ // CF OR FC
    if(code <= 499){
      if(FMVariable[code].datatype=='b'||FMVariable[code].datatype=='i'||FMVariable[code].datatype=='f'||FMVariable[code].datatype=='S'||FMVariable[code].datatype=='l'||FMVariable[code].datatype=='c'){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    } 
  }else if (select==7){ // CF OR FC
    if(code <= 499){
      if(ErrorVariable[code].datatype=='b'||ErrorVariable[code].datatype=='i'||ErrorVariable[code].datatype=='f'||ErrorVariable[code].datatype=='S'||ErrorVariable[code].datatype=='l'||ErrorVariable[code].datatype=='c'){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    } 
  }else if (select==8){ // CF OR FC
    if(code <= 499){

      if(GlobalVariable[code].datatype=='b'||GlobalVariable[code].datatype=='i'||GlobalVariable[code].datatype=='f'||GlobalVariable[code].datatype=='S'||GlobalVariable[code].datatype=='l'||GlobalVariable[code].datatype=='c'){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    } 
  }else{
    return false;
  }
}
void removemessage(Message* messages,uint8_t Priority){
  uint8_t len;
  uint8_t location;
  if(Priority>=1&&Priority<=6){
    len = queueLength(messages,1);
  }else{
    len = queueLength(messages,2);
  }
  switch(Priority){
    case 21:
     for(uint8_t i=0;i<=len-1;i++){
        if(messages[i].Priority==Priority-1&&(messages[i].Content[getLoc(messages[i].Content)]=='=')){
          location= i;
          break;
        }
      }  
    break;
    case 20:
      for(uint8_t i=0;i<=len-1;i++){
        if(messages[i].Priority==Priority&&(messages[i].Content[getLoc(messages[i].Content)]=='?')){
          location= i;
          break;
        }
      }  
    break;
    default:
      for(uint8_t i=0;i<=len-1;i++){
        if(messages[i].Priority==Priority){
          location= i;
          break;
        }
      }  
    break;
  }
  
  for(uint8_t i= location;i<=len-1;i++){
    messages[i]=messages[i+1];
  }
}
InformationQueue* SelectQueue(String msg){
  if(device.device_id!='M'){
    if(msg[1]=='M'||msg[0]=='M'){ //Queue 1
    return &Queue1;
  }
  if((msg[0]=='C'&&msg[1]=='R')||(msg[0]=='R'&&msg[1]=='C')||(msg[0]=='F'&&msg[1]=='R')){// Queue 2
    return &Queue2;  
  }else{
    return &Queue3;
  }
  }else{
    if((msg[0]=='C'&&msg[1]=='R')||(msg[0]=='F'&&msg[1]=='R')||(msg[0]=='M'&&msg[1]=='R')){
      return &Queue5;
    }
    if((msg[0]=='F'&&msg[1]=='C')||(msg[0]=='R'&&msg[1]=='C')||(msg[0]=='M'&&msg[1]=='C')){
      return &Queue2;
    }
    if((msg[0]=='C'&&msg[1]=='F')||(msg[0]=='R'&&msg[1]=='F')||(msg[0]=='M'&&msg[1]=='F')){
      return &Queue3;
    }else{
      return &Queue1;
    }
  }
  
}
Variable* SelectVarArr(String msg){
  if((msg[0]=='C'&&msg[1]=='F')||(msg[0]=='F'&&msg[1]=='C')){ // CF - FC
   return CFVariable;
  }else if((msg[0]=='C'&&msg[1]=='R')||(msg[0]=='R'&&msg[1]=='C')){ // CR or RC
    return CRVariable;
  }else if((msg[0]=='C'&&msg[1]=='M')||(msg[0]=='M'&&msg[1]=='C')){ // CM or MC
    return CMVariable;
  }else if ((msg[0]=='R'&&msg[1]=='F')||(msg[0]=='F'&&msg[1]=='R')){ // RF or FR
    return RFVariable;
  }else if ((msg[0]=='R'&&msg[1]=='M')||(msg[0]=='M'&&msg[1]=='R')){ // RM or MR
    return RMVariable;
  }else if ((msg[0]=='F'&&msg[1]=='M')||(msg[0]=='M'&&msg[1]=='F')){ // FM or MF
    return FMVariable;
  }else {
    return GlobalVariable;
  }
}
uint8_t Selectindex(String msg){
  if((msg[0]=='C'&&msg[1]=='F')||(msg[0]=='F'&&msg[1]=='C')){ // CF - FC
   return 1;
  }else if((msg[0]=='C'&&msg[1]=='R')||(msg[0]=='R'&&msg[1]=='C')){ // CR or RC
    return 2;
  }else if((msg[0]=='C'&&msg[1]=='M')||(msg[0]=='M'&&msg[1]=='C')){ // CM or MC
    return 3;
  }else if ((msg[0]=='R'&&msg[1]=='F')||(msg[0]=='F'&&msg[1]=='R')){ // RF or FR
    return 4;
  }else if ((msg[0]=='R'&&msg[1]=='M')||(msg[0]=='M'&&msg[1]=='R')){ // RM or MR
    return 5;
  }else if ((msg[0]=='F'&&msg[1]=='M')||(msg[0]=='M'&&msg[1]=='F')){ // FM or MF
    return 6;
  }else {
    return 7;
  }
}
bool Answer_index_check(String msg_str){
  uint16_t Code =  msg_str.substring(2,5).toInt();
  InformationQueue* Queue_ptr = SelectQueue(msg_str);
  if(Queue_ptr->Expected_answer_index==Code){
    return true;
  }else{
    return false;
  }
}
void updatelasttime(Message SelectedMessage){
  InformationQueue* Queue_ptr = SelectQueue(SelectedMessage.Content);
  Queue_ptr->last_sent_time=millis();
}
uint8_t getLoc(String msg){
  uint8_t loc=0;
  for(int i=0;i<=MAX_MESSAGE_LENGTH;i++){
    if(msg[i]=='?'){
      loc=i;
      return loc;
    }
  }
  for(int i=0;i<=MAX_MESSAGE_LENGTH;i++){
    if(msg[i]=='='){
      loc=i;
      return loc;
    }
  }
  
}
// ----- Error Category Messages --------
void RecieveErrOkWrOk(String msg){
  device.no_repeated_Errmessage=0; // reset number of tries for error queue - need to be checked
  if (msg[0]=='E'){
    device.Waiting_for_ErrOk=false;
    Serial3.println("Error Queue length before removing");
    uint8_t len = queueLength(ErrorQueue,1);
    Serial3.println(len);
    removeErr(); // remove error message from queue
    Serial3.println("Error Queue length after removing");
    len = queueLength(ErrorQueue,1);
    Serial3.println(len);
  }else{
    device.Waiting_for_WrOk=false;
    removeWr(); // remove error message from queue
  }
}
void RecieveErr_Wr(uint16_t Code,String msg){
  if(isindex(Code,7)){// check if error code exists
    // fcn to handle and solve the (error-warning) then returns (Errok-WrOk)
    Serial3.println("Index exists");
    if (msg[0]=='E'){
      ErrorHandling(Code);}
    else{
      WarningHandling(Code);}
  }else{
  // return message of (ErrWt-WrWt) as Errcode not exist
  if (msg[0]=='E'){
    Serial3.println("Index doesn't exist");
    SendErrWt();}
  else{SendWrWt();}
  }
}
// ----- Error Messages --------
void removeErr(){
  uint8_t len = queueLength(ErrorQueue,1);
  uint8_t location;
  for(uint8_t i=0;i<=len-1;i++){
    if(ErrorQueue[i].Content[0]=='E'&&ErrorQueue[i].Content[2]!='W'&&ErrorQueue[i].Content[2]!='O'){
      location= i;
    }
  }
  for(uint8_t i= location;i<=len-1;i++){
    ErrorQueue[i]=ErrorQueue[i+1];
  }
}
void ErrorHandling(uint16_t ErrorCode){
  // ------------to be fill by error team
  Serial3.println("Error Handled");
  // ------------
  SendErrOk();// Erok
}
void SendErrOk(){
  String msg = "ErOk";
  build_message(msg,ErrorQueue,4);
  Serial3.println("ErrOk Queued");
}
void SendErrWt(){
  String msg = "ErWt";
  build_message(msg,ErrorQueue,3);
}
void SendErr(uint16_t ErrorCode){
  String msg = "Er" + String(ErrorCode);
  build_message(msg,ErrorQueue,1); 
}
// ------- Warning Messages -------
void removeWr(){
  uint8_t len = queueLength(ErrorQueue,1);
  uint8_t location;
  for(uint8_t i=0;i<=len-1;i++){
    if(ErrorQueue[i].Content[0]=='W'&&ErrorQueue[i].Content[2]!='W'&&ErrorQueue[i].Content[2]!='O'){
      location= i;
    }
  }
  for(uint8_t i= location;i<=len-1;i++){
    ErrorQueue[i]=ErrorQueue[i+1];
  }
}
void WarningHandling(uint16_t WarningCode){
  // ------------to be fill by Warning team
  // ------------
  SendWrOk();
}
void SendWrOk(){
  String msg = "WrOk";
  return build_message(msg,ErrorQueue,6);
}
void SendWrWt(){
  String msg = "WrWt";
  return build_message(msg,ErrorQueue,5);
}
void SendWr(uint16_t WarningCode){
  String msg = "Wr" + String(WarningCode);
  build_message(msg,ErrorQueue,2);
}
// ----- Information Messages --------
void RecieveInfoWt(InformationQueue Queue){
  if(Queue.no_repeated_infomessage>=MAX_RETRY_COUNT){
    Serial3.println("Program stopped");
    stop_program();
  }
  Queue.no_repeated_infomessage++;
  Queue.Waiting_for_ResOk=false;
  Queue.Waiting_for_UpdateOk=false;
}
void RecieveInfoOk(String msg_str){
  if(device.device_id=='M'){
    if(Queue1.Waiting_for_UpdateOk){
    Queue1.Waiting_for_UpdateOk=false;
  }
  Queue1.no_repeated_infomessage=0;
  // remove message from specific Queue
  removemessage(Queue1.MessageQueue,21);
  Serial.println("Update Ok and message Removed");  
  }else{
    InformationQueue* Queue_ptr = SelectQueue(msg_str);
  // disable waiting
  if(Queue_ptr->Waiting_for_UpdateOk){
    Queue_ptr->Waiting_for_UpdateOk=false;
  }
  Queue_ptr->no_repeated_infomessage=0;
  // remove message from specific Queue
  removemessage(Queue_ptr->MessageQueue,21);
  Serial3.println("Update Ok and message Removed");
  }
  
}
void RecieveInfoRes(String msg_str,char* readarr){
  if(device.device_id=='M'){
  if(Answer_index_check(msg_str)){
    // handle
    Variable* arr =SelectVarArr(msg_str);
    uint8_t index = Selectindex(msg_str);
    uint16_t Code =  msg_str.substring(2,5).toInt();//fc012
    if(Code>=500){
        Code=Code-500;
        arr=GlobalVariable;
        index=8;
      }
    UpdateVar(Code,index,msg_str,arr,readarr);
    // disable waiting
    if(Queue1.Waiting_for_ResOk){
      Queue1.Waiting_for_ResOk=false;
    }
    Queue1.no_repeated_infomessage=0;
    // remove message from specific Queue
    removemessage(Queue1.MessageQueue,20);
    Serial3.println("Message Removed");
  }else{
    SendWt(msg_str.substring(0,2),Queue1.MessageQueue);  
  }
  }else{
    InformationQueue* Queue_ptr = SelectQueue(msg_str);
  if(Answer_index_check(msg_str)){
    // handle
    Variable* arr =SelectVarArr(msg_str);
    uint8_t index = Selectindex(msg_str);
    uint16_t Code =  msg_str.substring(2,5).toInt();//fc012
    if(Code>=500){
        Code=Code-500;
        arr=GlobalVariable;
        index=8;
      }
    UpdateVar(Code,index,msg_str,arr,readarr);
    // disable waiting
    if(Queue_ptr->Waiting_for_ResOk){
      Queue_ptr->Waiting_for_ResOk=false;
    }
    Queue_ptr->no_repeated_infomessage=0;
    // remove message from specific Queue
    removemessage(Queue_ptr->MessageQueue,20);
    Serial3.println("Message Removed");
  }else{
    SendWt(msg_str.substring(0,2),Queue_ptr->MessageQueue);  
  }
  }
  
  
}
void RecieveInfoUpdate(uint16_t Code, String msg,String msg_str,char* readarr){
  Variable* arr =SelectVarArr(msg);
  uint8_t index = Selectindex(msg);
  if(Code>=500){
        Code=Code-500;
        arr=GlobalVariable;
        index=8;
      }
  UpdateVar(Code,index,msg_str,arr,readarr);
}
void RecieveInfoQuestion(uint16_t Code, String msg){ // determine which Variables to use CF/CR/CM/etc..
  uint8_t index = Selectindex(msg);
  if(Code>=500){
        Code=Code-500;
        index=8;
      }
  SendQesReshandleing(msg,Code,index);
}
void SendOkhandleing(String msg_str){
  if(device.device_id=='M'){
  Serial.println("Sending Ok");
  SendOk(msg_str.substring(0,2),Queue1.MessageQueue);
  }else{
    InformationQueue* Queue_ptr = SelectQueue(msg_str);
  SendOk(msg_str.substring(0,2),Queue_ptr->MessageQueue);
  }
  
}
void SendQesReshandleing(String msg_str,uint16_t Code,uint8_t select){ // check for index existsence and determine Queue to use -> Queue 1 ,etc..
  if(isindex(Code,select)){
    if(device.device_id=='M'){
    SendQesRes(msg_str,Queue1.MessageQueue);  
    }
    else{
      InformationQueue* Queue_ptr = SelectQueue(msg_str);
    SendQesRes(msg_str,Queue_ptr->MessageQueue);
    }
    
  }else{
    SendWthandleing(msg_str.substring(0,2));
  }
}
void SendWthandleing(String msg_str){
  if(device.device_id=='M'){
  SendWt(msg_str.substring(0,2),Queue1.MessageQueue);
  }
  else{
    InformationQueue* Queue_ptr = SelectQueue(msg_str);
  SendWt(msg_str.substring(0,2),Queue_ptr->MessageQueue);
  }
  
 }
void UpdateVar(uint16_t Code,uint8_t select,String msg_str,Variable* Vararr,char* readarr){
  if(isindex(Code,select)){
    if(Code>=500){Code=Code-500;}
    uint8_t len = Getlength(readarr);
    switch(Vararr[Code].datatype){
        case 'b':
        case 'c':
          for(uint8_t i=0;i<1;i++){
            Vararr[Code].value.byte_array[i]=readarr[len-6+i];
          }
          Serial3.println("Value after update: ");
          Serial3.println(Vararr[Code].value.boolvalue);  
        break;
        case 'i':
          for(uint8_t i=0;i<4;i++){
            Vararr[Code].value.byte_array[i]=readarr[len-6+i];
          }  
          Serial3.println("Value after update: ");
          Serial3.println(Vararr[Code].value.intvalue); 
        break;
        case 'f':
          for(uint8_t i=0;i<4;i++){
            Vararr[Code].value.byte_array[i]=readarr[len-6+i];
          }  
          Serial3.println("Value after update: ");
          Serial3.println(Vararr[Code].value.floatvalue);
        break;
        case 'l'://FC101=!!!!!!x
          for(uint8_t i=0;i<4;i++){
            Vararr[Code].value.byte_array[i]=readarr[len-6+i];
          }  
          Serial3.println("Value after update: ");
          Serial3.println(Vararr[Code].value.longvalue);
        break;
        case 'S'://CR101=AHmed||x
          *(Vararr[Code].value.stringPtr)=String(readarr).substring(getLoc(msg_str)+1,len-2);
          Serial3.println("Value after update: ");
          Serial3.println(*(Vararr[Code].value.stringPtr));
        break;
    }
    if(msg_str[getLoc(msg_str)]!='?'){
      SendOkhandleing(msg_str);
    }
}else{
  SendWthandleing(msg_str);
}
}
char GetVar(Variable* Vararr, uint16_t var_index) {
  return Vararr[var_index].datatype;
}
void SendQesRes(String msg_str,Message* messageQueue){ // Send the Response to be queued
  String msg;
  msg = String(msg_str[1])+String(msg_str[0])+msg_str.substring(2);
  build_message(msg,messageQueue,15);    
}
void SendOk(String msg_str,Message* Vararr){
  String msg = String(msg_str[1])+String(msg_str[0])+"Ok";
  build_message(msg,Vararr,11);
}
void SendWt(String msg_str,Message* Vararr){
  String msg = String(msg_str[1])+String(msg_str[0])+"Wt";
  build_message(msg,Vararr,10);
}
void AskVar(char destination_device, uint16_t var_index){
     // Create message string: "DeviceIndex=Value"
    String msg = String(device.device_id) + String(destination_device) + String(var_index) + "?";
    InformationQueue* Queue_ptr = SelectQueue(msg);
    Queue_ptr->Expected_answer_index=var_index;
    build_message(msg,Queue_ptr->MessageQueue,20);
}
void SendVar(char destination_device, uint16_t var_index){
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

// ----- Queuing ---------
uint8_t queueLength(Message* QueueList,uint8_t select){
  switch(select){
    case 1: // error queue
      for(uint8_t i=0;i<=ERROR_QUEUE_SIZE-1;i++){ 
        if(QueueList[i].Content==""){
          return i;
        }
      }
      return ERROR_QUEUE_SIZE;
    break;
    case 2: // normal queue
    for(uint8_t i=0;i<=QUEUE_SIZE_PER_DEVICE-1;i++){   
      if(QueueList[i].Content==""){
          return i;
        }
      }
      return QUEUE_SIZE_PER_DEVICE;
    break;
  }
   
}
void queuing(Message message,Message* messages){
    uint8_t select;
    uint8_t Queue_size;
    uint8_t Queuing_index;
    switch(message.Content[0]){
      case 'E':
      case 'W':
        select = 1;
        Queue_size = ERROR_QUEUE_SIZE;
        Queuing_index = queueLength(messages,1);
      break;
      default :
        select = 2;
        Queue_size = QUEUE_SIZE_PER_DEVICE;
        Queuing_index = queueLength(messages,2);
      break;
    }
    if (Queuing_index>=(Queue_size)){
      // what to do       
    }else{
      messages[Queuing_index]=message;
      }
}
Message SelectMessage(Message* QueueList,uint8_t select,InformationQueue Queue){
  uint8_t len;
  struct Message msg;
  switch(select){
    case 1: 
      len = queueLength(QueueList,select);
      if(len>=1){ // errorqueue has message
        // 1. Err message check
        for(uint8_t i=0;i<=len-1;i++){
          if(QueueList[i].Priority==1){
            return msg = QueueList[i];
          }
        }
        // 2. Wr message check
        for(uint8_t i=0;i<=len-1;i++){
          if(QueueList[i].Priority==2){
            return msg = QueueList[i];
          }
        }
        // 3. ErrWt message check
        for(uint8_t i=0;i<=len-1;i++){
          if(QueueList[i].Priority==3){
            return msg = QueueList[i];
          }
        }
        // 4. WrWt message check
        for(uint8_t i=0;i<=len-1;i++){
          if(QueueList[i].Priority==4){
            return msg = QueueList[i];
          }
        }
        // 5. ErrOk message check
        for(uint8_t i=0;i<=len-1;i++){
          if(QueueList[i].Priority==5){
            return msg = QueueList[i];
          }
        }
        // 6. WtOk message check
        for(uint8_t i=0;i<=len-1;i++){
          if(QueueList[i].Priority==6){
            return msg = QueueList[i];
          }
        }
      }
      msg.Priority=30;
      return msg;
    break;
    case 2:
      len = queueLength(Queue.MessageQueue,select);
      if(len>=1){ // queue has message
        // 1. Err message check
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==1){
            return msg = Queue.MessageQueue[i];
          }
        }
        // 2. Wr message check
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==2){
            return msg = Queue.MessageQueue[i];
          }
        }
        // 3. ErrWt message check
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==3){
            return msg = Queue.MessageQueue[i];
          }
        }
        // 4. WrWt message check
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==4){
            return msg = Queue.MessageQueue[i];
          }
        }
        // 5. ErrOk message check
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==5){
            return msg = Queue.MessageQueue[i];
          }
        }
        // 6. WtOk message check
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==6){
            return msg = Queue.MessageQueue[i];
          }
        }
        // 1. information What
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==10){
            return msg = Queue.MessageQueue[i];
          }
        }
        // 2. Information Ok
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==11){
            return msg = Queue.MessageQueue[i];
          }
        }
        // 3. Response to Question
        for(uint8_t i=0;i<=len-1;i++){
          if(Queue.MessageQueue[i].Priority==15){
            return msg = Queue.MessageQueue[i];
          }
        }
          // 4. Question
          for(uint8_t i=0;i<=len-1;i++){
            if(Queue.MessageQueue[i].Priority==20&&(Queue.MessageQueue[i].Content[5]=='?'||Queue.MessageQueue[i].Content[4]=='?'||Queue.MessageQueue[i].Content[3]=='?')){
              return msg = Queue.MessageQueue[i];
            }
          }
          // 5. Update
          for(uint8_t i=0;i<=len-1;i++){
            if(Queue.MessageQueue[i].Priority==20&&(Queue.MessageQueue[i].Content[5]=='='||Queue.MessageQueue[i].Content[4]=='='||Queue.MessageQueue[i].Content[3]=='=')){
              return msg = Queue.MessageQueue[i];
            } 
          }
        
      }
      msg.Priority=30;
      return msg;
    break;
  }
  return msg;
}
Message SelectedMessagefromQueues(){
  struct Message msg2 = SelectMessage(ErrorQueue,2,Queue1);
  unsigned long msg2_spend_time=millis()-(Queue1.last_sent_time);
  struct Message msg3 = SelectMessage(ErrorQueue,2,Queue2);
  unsigned long msg3_spend_time=millis()-(Queue2.last_sent_time);
  struct Message msg4 = SelectMessage(ErrorQueue,2,Queue3);
  unsigned long msg4_spend_time=millis()-(Queue3.last_sent_time);
  struct Message msg;
  unsigned long msg_spend_time;
  if(msg2.Priority<msg3.Priority){
    msg = msg2;
    msg_spend_time = msg2_spend_time;
  }
  else if(msg2.Priority>msg3.Priority){
    msg = msg3;
    msg_spend_time = msg3_spend_time;
  }
  else if(msg2.Priority==msg3.Priority){
    if(msg2_spend_time>msg3_spend_time){
      msg = msg2;
      msg_spend_time = msg2_spend_time;
    }else{
      msg = msg3;
      msg_spend_time = msg3_spend_time;
    }
  }
  if(msg.Priority<msg4.Priority){
    return msg;
  }else if(msg.Priority>msg4.Priority){
    return msg4;
  }
  else if(msg.Priority==msg4.Priority){
    if(msg_spend_time>msg4_spend_time){
      return msg;
    }else{
      return msg4;
    }
  }
}
// ---- stop_program --------
void stop_program(){
  while(true){

  }
}