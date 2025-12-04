#include "Terminal_Devices.h"
String Reading(HardwareSerial& comserial,char* readarr){
    // Method 1: Use readBytesUntil (RECOMMENDED)
    for(uint8_t i=0;i<=MAX_MESSAGE_LENGTH;i++){
      readarr[i]={};
    }
    byte incoming = comserial.read();
    if(incoming=='b'){
      incoming = comserial.read();
      byte l = incoming;
      Serial3.println("Recieved len:");
      Serial3.println(l);
      for(uint8_t i=0;i<l;i++){
         // wait until the next byte arrives
        while (comserial.available() == 0) { 
        // do nothing (or add timeout)
        }
        incoming = Serial.read();
        readarr[i] = incoming;   
      }
    }
    return "ss";
}
void sendERR(Message msg){
  uint8_t lengt =msg.Content.length()+3;
  Serial3.println("expected len: ");
  Serial3.println(lengt);
  device.stream->print("b");
  device.stream->write(lengt);
  device.stream->print(msg.Content);
  device.stream->write(msg.XorCheck);
  device.stream->write(msg.SumCheck);
  device.stream->write('x');
  device.last_sent_time=millis();
}
void sendInfo(Message msg){
  InformationQueue* Queue_ptr = SelectQueue(msg.Content);
  if(!Queue_ptr->Waiting_for_ResOk||!Queue_ptr->Waiting_for_UpdateOk){
    uint8_t lengt =msg.Content.length();
    uint16_t Code;
    Variable* arr;  
    if(msg.Priority==15||(msg.Priority==20&&msg.Content[getLoc(msg.Content)]=='=')){
      arr =SelectVarArr(msg.Content);
      Code = msg.Content.substring(2,getLoc(msg.Content)).toInt();
      if(Code>=500){
        Code=Code-500;
        arr=GlobalVariable;
      }
      switch(arr[Code].datatype){
      case 'b':
      case 'c':
          lengt+=4;  
      break;
        case 'i':
        case 'f':  
        case 'l':
          lengt+=7;  
        break;
        case 'S':
          String perm =*(arr[Code].value.stringPtr);
          lengt+=perm.length()+3;
        break;
    }
    }else{
      lengt+=3;
    }
    
    Serial3.println("expected len: ");
    Serial3.println(lengt);
    device.stream->print("b");
    device.stream->write(lengt);
  
    device.stream->print(msg.Content);
    if(msg.Priority==15||(msg.Priority==20&&msg.Content[getLoc(msg.Content)]=='=')){
      arr =SelectVarArr(msg.Content);
      Code = msg.Content.substring(2,getLoc(msg.Content)).toInt();
      if(Code>=500){
        Code=Code-500;
        arr=GlobalVariable;
      }
      switch(arr[Code].datatype){
        case 'b':
        case 'c':
          for(uint8_t i=0;i<1;i++){
          device.stream->write(arr[Code].value.byte_array[i]);    
          }  
        break;
        case 'i':
          for(uint8_t i=0;i<4;i++){
          device.stream->write(arr[Code].value.byte_array[i]);    
          }  
        break;
        case 'f':
          for(uint8_t i=0;i<4;i++){
          device.stream->write(arr[Code].value.byte_array[i]);    
          }  
        break;
        case 'l':
          for(uint8_t i=0;i<4;i++){
          device.stream->write(arr[Code].value.byte_array[i]);    
          }  
        break;
        case 'S':
          String perm =*(arr[Code].value.stringPtr);
          for(int i=0;i<perm.length();i++){
            device.stream->write(perm[i]);
          }
          
        break;
      }//CR98=AhmedHesham||x
      if(getLoc(msg.Content)=='='){
        Queue_ptr->Waiting_for_UpdateOk=true;  
      }else{
        Queue_ptr->Waiting_for_ResOk=false;
      }
    }
    device.stream->write(msg.XorCheck);
    device.stream->write(msg.SumCheck);
    device.stream->write('x');
    updatelasttime(msg);      
  }else{
    if(millis()-Queue_ptr->last_sent_time>ACK_TIMEOUT_MS){
      Queue_ptr->Waiting_for_ResOk=false;
      Queue_ptr->Waiting_for_UpdateOk=false;
    }
  }
}
void send(Message msg,uint8_t select){
  if(select==3){ // sending Information type message
    sendInfo(msg);
  }else{ // sending Error type message
    sendERR(msg);
    switch (select){
      case 1:
        device.Waiting_for_ErrOk = true;
      break;
      case 2:
        if(msg.Priority==2){
          device.Waiting_for_WrOk = true;
        }
      break;
    }
  }
  
}
void stop_waiting(uint8_t select,InformationQueue* Queue_pt){
  switch(select){
    case 1:
    case 2:
      if(millis()-device.last_sent_time>=ACK_TIMEOUT_MS){
        if(device.no_repeated_Errmessage>=MAX_RETRY_COUNT){
          Serial3.println("Stop Programme for max retry reached");
          stop_program();
        }else{
          device.no_repeated_Errmessage++;
        }
        if(select==1){
          device.Waiting_for_ErrOk=false;
        }else if (select==2){
          device.Waiting_for_WrOk=false;
        }
      }    
    break;
    case 3:
      if(millis()-Queue_pt->last_sent_time>=ACK_TIMEOUT_MS){
        if(Queue_pt->no_repeated_infomessage>=MAX_RETRY_COUNT){
          Serial3.println("Stop Programme for max retry reached");
          stop_program();
        }else{
          Queue_pt->no_repeated_infomessage++;
          Queue_pt->Waiting_for_UpdateOk=false;
          Queue_pt->Waiting_for_ResOk=false;
        }
      }
    break;
  }
  
}
void SendMessage(){
  // check if error list has message
  struct Message msg1 = SelectMessage(ErrorQueue,1);
  if(msg1.Priority!=30){
    if(!device.Waiting_for_ErrOk){
      if(msg1.Priority==1){//Er102
        Serial3.println("Message selected is: ");
        Serial3.println(msg1.Content);
        send(msg1,1);   
      }else if(device.Waiting_for_WrOk){
        stop_waiting(2);
      }
      else{
        Serial3.println("Message selected is: ");
        Serial3.println(msg1.Content);
        send(msg1,2);
        if(msg1.Priority!=2){
          removemessage(ErrorQueue,msg1.Priority);
        }
      }
    }else{
      stop_waiting(1);
    } 
  }else{
    struct Message SelectedMessage= SelectedMessagefromQueues();
    if(SelectedMessage.Priority==30){}
    else{
      InformationQueue* Queue_ptr=SelectQueue(SelectedMessage.Content);
      if(Queue_ptr->Waiting_for_ResOk||Queue_ptr->Waiting_for_UpdateOk){
        stop_waiting(3,Queue_ptr);
      }
      else{
        Serial3.println("Selected Info Message: ");
        Serial3.println(SelectedMessage.Content);
        send(SelectedMessage,3);
        if(SelectedMessage.Priority==20){
          if(SelectedMessage.Content[getLoc(SelectedMessage.Content)]=='='){
            Queue_ptr->Waiting_for_UpdateOk=true;
          }else{
            Queue_ptr->Waiting_for_ResOk=true;
          }
        }
        if(SelectedMessage.Priority!=20){
          InformationQueue* Queue_ptr=SelectQueue(SelectedMessage.Content);
          removemessage(Queue_ptr->MessageQueue,SelectedMessage.Priority);
        }
      }    
    }
  }
}
void RecieveMessage(HardwareSerial& comserial){
  // Serial - Serial1 -Serial2
  if(comserial.available()>4){
    Serial3.println("begin Receive: ");
    char readArray[MAX_MESSAGE_LENGTH]={};
    String msg_str=Reading(comserial,readArray);
    Serial3.println("End Receive: ");
    struct Message msg;
    msg = Rebuildmsg(readArray);
    msg_str=msg.Content;
    Serial3.println(msg.Content);
    //---------------------
    if(Checksum_Validation(readArray,msg)){
      Serial3.println("Checksum validated");
      uint16_t Code;// index (ex: Er102 -> index=102)
      //CnC - ResPi
      switch (msg.Priority){
        case 20:
          Code =  msg.Content.substring(2,getLoc(msg.Content)).toInt();
          if(msg.Content[getLoc(msg.Content)]=='='){ // FC136=1
            //RF136=10 
            RecieveInfoUpdate(Code,msg.Content,msg_str,readArray); 
          }else{ // CR045?
            RecieveInfoQuestion(Code,msg.Content);
          }
        break;
        case 11: //FCOk
          RecieveInfoOk(msg.Content); 
        break;
        case 15:
          RecieveInfoRes(msg.Content,readArray); // CR045? -- 045
        break;
      }
      switch (msg.Priority){
        case 1: // Er102 
        case 2: // Wr101  
          Serial3.println("Error/Wr Determined");
          Code =  msg.Content.substring(2).toInt(); //Er102 -102 
          RecieveErr_Wr(Code,msg.Content); // Ok message   
        break;
        case 4: //ErOk
        case 6: //WrOk
          Serial3.println("ErrOk/WrOk Determined");
          RecieveErrOkWrOk(msg.Content); // CnC - Er102 - ResPi - Ok
        break;       
        case 3: //ErWt
        case 5: //WrWt
        Serial3.println("case 3&5");
          if(device.no_repeated_Errmessage>=MAX_RETRY_COUNT){ // max tries reached
            stop_program();
          }else{
            device.no_repeated_Errmessage++; // increase a try 
            // disable waiting for resending 
            if (msg.Content[0]=='E'){device.Waiting_for_ErrOk=false;}
            else{device.Waiting_for_WrOk=false;}
            SendMessage();
          }
        break;
        case 10: //FCWt 
          Serial3.println("case 10");
          InformationQueue* Queue_ptr = SelectQueue(msg.Content);
          RecieveInfoWt(*Queue_ptr);
          SendMessage(); // resending
          break;
        
      }
    }else{
     Serial3.println("Checksum Failed");
     if(msg.Priority==1||msg.Priority==3||msg.Priority==4){
      SendErrWt();
     }else if(msg.Priority==1||msg.Priority==3||msg.Priority==4){
      SendWrWt();
     }else{
      InformationQueue* Queue_ptr = SelectQueue(msg.Content);
      SendWt(msg.Content,Queue_ptr->MessageQueue);
     }
    }
  }
}
