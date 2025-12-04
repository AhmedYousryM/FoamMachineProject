#include "mainArduino.h"
void mainsendInfo(Message msg){
  InformationQueue* Queue_ptr = SelectQueue(msg.Content);
  if(!Queue1.Waiting_for_ResOk||!Queue1.Waiting_for_UpdateOk){
    uint8_t lengt =msg.Content.length();
    uint16_t Code;
    Variable* arr;  
    if(msg.Priority==15||(msg.Priority==20&&msg.Content[getLoc(msg.Content)]=='=')){
      arr =SelectVarArr(msg.Content);
      Code = msg.Content.substring(2,getLoc(msg.Content)).toInt();
      if(Code>=500){
        Code = Code-500;
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
    HardwareSerial*  mystream;
        if(msg.Content[1]=='R'){
          mystream=&Serial1;
          Queue5.last_sent_time=millis();
        }else if(msg.Content[1]=='C'){
          mystream=&Serial2;
          Queue2.last_sent_time=millis();
        }else{
          mystream=&Serial3;
          Queue3.last_sent_time=millis();
        }
    Serial.println("expected len: ");
    Serial.println(lengt);
    mystream->print("b");
    mystream->write(lengt);
  
    mystream->print(msg.Content);
    if(msg.Priority==15||(msg.Priority==20&&msg.Content[getLoc(msg.Content)]=='=')){
      arr =SelectVarArr(msg.Content);
      Code = msg.Content.substring(2,getLoc(msg.Content)).toInt();
      if(Code>=500){
        Code = Code-500;
        arr=GlobalVariable;
      }
      switch(arr[Code].datatype){
        case 'b':
        case 'c':
          for(uint8_t i=0;i<1;i++){
          mystream->write(arr[Code].value.byte_array[i]);    
          }  
        break;
        case 'i':
          for(uint8_t i=0;i<4;i++){
          mystream->write(arr[Code].value.byte_array[i]);    
          }  
        break;
        case 'f':
          for(uint8_t i=0;i<4;i++){
          mystream->write(arr[Code].value.byte_array[i]);    
          }  
        break;
        case 'l':
          for(uint8_t i=0;i<4;i++){
          mystream->write(arr[Code].value.byte_array[i]);    
          }  
        break;
        case 'S':
          String perm =*(arr[Code].value.stringPtr);
          for(int i=0;i<perm.length();i++){
            mystream->write(perm[i]);
          }
          
        break;
      }//CR98=AhmedHesham||x
      if(msg.Priority==20&&msg.Content[getLoc(msg.Content)]=='='){
        Serial.println("true trigered");
        Queue1.Waiting_for_UpdateOk=true;  
      }else if(msg.Priority==20){
        Queue1.Waiting_for_ResOk=true;
      }
    }
    mystream->write(msg.XorCheck);
    mystream->write(msg.SumCheck);
    mystream->write('x');
    updatelasttime(msg);      
  }else{
    if(millis()-Queue_ptr->last_sent_time>ACK_TIMEOUT_MS){
      Queue_ptr->Waiting_for_ResOk=false;
      Queue_ptr->Waiting_for_UpdateOk=false;
    }
  }
}
String mainReading(HardwareSerial& comserial,char* readarr){
    // Method 1: Use readBytesUntil (RECOMMENDED)
    for(uint8_t i=0;i<=MAX_MESSAGE_LENGTH;i++){
      readarr[i]={};
    }
    byte incoming = comserial.read();
    if(incoming=='b'){
      incoming = comserial.read();
      byte l = incoming;
      Serial.println("Recieved len:");
      Serial.println(l);
      for(uint8_t i=0;i<l;i++){
         // wait until the next byte arrives
        while (comserial.available() == 0) { 
        // do nothing (or add timeout)
        }
        incoming = comserial.read();
        readarr[i] = incoming;    
      }
    }
    return "ss";
}
void main_set_serial_for_queue(){
  Queue2.stream=&Serial2;
  Queue5.stream=&Serial1;
  Queue3.stream=&Serial3;
  Queue2.stream->begin(SERIAL_BAUD_RATE);
  Queue3.stream->begin(SERIAL_BAUD_RATE);
  Queue5.stream->begin(SERIAL_BAUD_RATE);
}
Message mainSelectedMessagefromQueues(){
  struct Message msg1 = SelectMessage(ErrorQueue,2,Queue1);
  if(Queue1.Waiting_for_ResOk||Queue1.Waiting_for_UpdateOk){
    msg1.Priority=30;
  }
  unsigned long msg1_spend_time=millis()-(Queue1.last_sent_time);
  struct Message msg2 = SelectMessage(ErrorQueue,2,Queue5);
  unsigned long msg2_spend_time=millis()-(Queue5.last_sent_time);
  struct Message msg3 = SelectMessage(ErrorQueue,2,Queue2);
  unsigned long msg3_spend_time=millis()-(Queue2.last_sent_time);
  struct Message msg4 = SelectMessage(ErrorQueue,2,Queue3);
  unsigned long msg4_spend_time=millis()-(Queue3.last_sent_time);
  
  struct Message msg;
  unsigned long msg_spend_time;
  if(msg1.Priority<msg2.Priority){
    queuepointer=&Queue1;
    msg = msg1;
    msg_spend_time = msg1_spend_time;
  }
  else if(msg1.Priority>msg2.Priority){
    queuepointer=&Queue5;
    msg = msg2;
    msg_spend_time = msg2_spend_time;
  }
  else if(msg1.Priority==msg2.Priority){
    if(msg1_spend_time>msg2_spend_time){
      queuepointer=&Queue1;
      msg = msg1;
      msg_spend_time = msg1_spend_time;
    }else{
      queuepointer=&Queue5;
      msg = msg2;
      msg_spend_time = msg2_spend_time;
    }
  }
  if(msg.Priority<msg3.Priority){
  }
  else if(msg.Priority>msg3.Priority){
    queuepointer=&Queue2;
    msg = msg3;
    msg_spend_time = msg3_spend_time;
  }
  else if(msg2.Priority==msg3.Priority){
    if(msg_spend_time>msg3_spend_time){
    }else{
      queuepointer=&Queue2;
      msg = msg3;
      msg_spend_time = msg3_spend_time;
    }
  }
  if(msg.Priority<msg4.Priority){
    return msg;
  }else if(msg.Priority>msg4.Priority){
    queuepointer=&Queue3;
    return msg4;
  }
  else if(msg.Priority==msg4.Priority){
    if(msg_spend_time>msg4_spend_time){
      return msg;
    }else{
      queuepointer=&Queue3;
      return msg4;
    }
  }
}
void main_stop_waiting(uint8_t select){
  if(millis()-device.last_sent_time>=ACK_TIMEOUT_MS){
    if(device.no_repeated_Errmessage>=MAX_RETRY_COUNT){
      Serial.println("Stop Programme for max retry reached");
      stop_program();
    }else{
      device.no_repeated_Errmessage++;
    }
    if(select==1){
      device.Waiting_for_ErrOk=false;
    }else if (select==2){
      device.Waiting_for_WrOk=false;
    }else if (select==3){
      queuepointer->Waiting_for_UpdateOk=false;
      queuepointer->Waiting_for_ResOk=false;
    }
  }  
}
void main_send(Message msg,uint8_t select){
    uint8_t lengt;
    if(msg.Priority==1||msg.Priority==2){
      lengt =msg.Content.length()+3;
      Serial.println("expected len: ");
      Serial.println(lengt);
      Serial1.print("b");
      Serial1.write(lengt);
      Serial1.print(msg.Content);
      Serial1.write(msg.XorCheck);
      Serial1.write(msg.SumCheck);
      Serial1.write('x');
      if(msg.Priority==1){
        device.Waiting_for_ErrOk=true;  
      }else{
        device.Waiting_for_WrOk=true;
      }
      device.last_sent_time=millis();
    }else if (msg.Priority>2&&msg.Priority<=6){
      lengt =msg.Content.length()+3;
      Serial.println("expected len: ");
      Serial.println(lengt);
      queuepointer->stream->print("b");
      queuepointer->stream->write(lengt);
      queuepointer->stream->print(msg.Content);
      queuepointer->stream->write(msg.XorCheck);
      queuepointer->stream->write(msg.SumCheck);
      queuepointer->stream->write('x');
      device.last_sent_time=millis();
    }else{
      if(msg.Priority==15||(msg.Priority==20&&msg.Content[getLoc(msg.Content)]=='=')){
        
        if(msg.Content[1]=='R'){
          Serial.println("expected len: ");
          Serial.println(Getlength(Myarr1)+1);
          Serial1.print("b");
          Serial1.write(Getlength(Myarr1)+1);
          for(uint8_t i=0;i<=Getlength(Myarr1);i++){
            Serial1.write(Myarr1[i]);    
        }
        for(uint8_t i=0;i<=Getlength(Myarr1);i++){
          Myarr1[i]={};
        }
        Queue5.last_sent_time=millis();

        }else if(msg.Content[1]=='C'){
          Serial.println("expected len: ");
          Serial.println(Getlength(Myarr2)+1);
          Serial2.print("b");
          Serial2.write(Getlength(Myarr2)+1);
          for(uint8_t i=0;i<=Getlength(Myarr2);i++){
          Serial2.write(Myarr2[i]);
        }
        for(uint8_t i=0;i<=Getlength(Myarr2);i++){
          Myarr2[i]={};
        }
        Queue2.last_sent_time=millis();
        }else{
          Serial.println("expected len: ");
          Serial.println(Getlength(Myarr3)+1);
          Serial3.print("b");
          Serial3.write(Getlength(Myarr3)+1);
          for(uint8_t i=0;i<=Getlength(Myarr3);i++){
          Serial3.write(Myarr3[i]);
        }
        for(uint8_t i=0;i<=Getlength(Myarr3);i++){
          Myarr3[i]={};
        }
        Queue3.last_sent_time=millis();
      }  
      }else{
        HardwareSerial*  mystream;
        if(msg.Content[1]=='R'){
          mystream=&Serial1;
          Queue5.last_sent_time=millis();
        }else if(msg.Content[1]=='C'){
          mystream=&Serial2;
          Queue2.last_sent_time=millis();
        }else{
          mystream=&Serial3;
          Queue3.last_sent_time=millis();
        }
        lengt =msg.Content.length()+3;
        Serial.println("expected len: ");
        Serial.println(lengt);
        mystream->print("b");
        mystream->write(lengt);
        mystream->print(msg.Content);
        mystream->write(msg.XorCheck);
        mystream->write(msg.SumCheck);
        mystream->write('x');
        
      }
  
}
    
    
  switch (select){
    case 1:
    device.Waiting_for_ErrOk = true;
    break;
    case 2:
    if(msg.Priority==2){
      device.Waiting_for_WrOk = true;
    }
    break;
    case 3:
    if(msg.Priority==20&&msg.Content[getLoc(msg.Content)]=='='){
      queuepointer->Waiting_for_UpdateOk=true;
    }else{
      queuepointer->Waiting_for_ResOk=true;
    }
    break;
    default:
    break;
  }
}
void mainSendMessage(){
  struct Message SelectedMessage= mainSelectedMessagefromQueues();
  if(device.Waiting_for_ErrOk){
    main_stop_waiting(1);
  }else{
    if (SelectedMessage.Priority==30){}
    else{
      Serial.println("Message Selected :");
      Serial.println(SelectedMessage.Content);
      if (SelectedMessage.Priority==1){
        main_send(SelectedMessage,1);
        removemessage(queuepointer->MessageQueue,1);
        }
      else if(device.Waiting_for_WrOk){main_stop_waiting(2);}
      else{
        if (SelectedMessage.Priority>1&&SelectedMessage.Priority<=6){
          main_send(SelectedMessage,2);
            removemessage(queuepointer->MessageQueue,SelectedMessage.Priority);
          }
        else{
          InformationQueue* Queue_ptr=SelectQueue(SelectedMessage.Content);
        if(SelectedMessage.Content[0]=='M'){
            mainsendInfo(SelectedMessage);
            InformationQueue* Queue_ptr=SelectQueue(SelectedMessage.Content);
            if(SelectedMessage.Priority!=20){
              Serial.println("before remove update message :");
              Serial.println(Queue1.MessageQueue[0].Content);
              removemessage(Queue1.MessageQueue,SelectedMessage.Priority);
              Serial.println("after remove update message :");
              Serial.println(Queue1.MessageQueue[0].Content);
            }
        }else{
          main_send(SelectedMessage,3);
          if(SelectedMessage.Priority==20&&SelectedMessage.Content[getLoc(SelectedMessage.Content)]=='='){
            removemessage(queuepointer->MessageQueue,21);
            }else{
              removemessage(queuepointer->MessageQueue,SelectedMessage.Priority);
          }
        }
        }
      }    
    }
  }
  }
void SendingWhat(Message msg,InformationQueue Queue){
  String Msg;
  if (msg.Priority==1||msg.Priority==3||msg.Priority==4){
    Msg = "ErWt";
    build_message(Msg,Queue.MessageQueue,3);
  }else if (msg.Priority==2||msg.Priority==5||msg.Priority==6){
    Msg = "WrWt";
    build_message(Msg,Queue.MessageQueue,5);
  }else{
    Msg = msg.Content[1]+msg.Content[0]+"Wt";
    build_message(Msg,Queue.MessageQueue,10);
  }
}
InformationQueue* SelectQueuefromSerial(HardwareSerial& comserial){
  if(&comserial==&Serial1){
    return &Queue5;
  }else if(&comserial==&Serial2){
    return &Queue2;
  }else if(&comserial==&Serial3){
    return &Queue3;
  }
}
InformationQueue* SelectQueueforOk(uint8_t select){
  switch (select){
    case 1:
      if(Queue2.Waiting_for_ErrOK&&!Queue3.Waiting_for_ErrOK){
        return &Queue2;
      }else if(!Queue2.Waiting_for_ErrOK&&Queue3.Waiting_for_ErrOK){
        return &Queue3;
      }else if (Queue2.last_sent_time<Queue3.last_sent_time){
        return &Queue2;
      }else{
        return &Queue3;
      }
    break;
    case 2:
      if(Queue2.Waiting_for_WrOK&&!Queue3.Waiting_for_WrOK){
        return &Queue2;
      }else if(!Queue2.Waiting_for_WrOK&&Queue3.Waiting_for_WrOK){
        return &Queue3;
      }else if (Queue2.last_sent_time<Queue3.last_sent_time){
        return &Queue2;
      }else{
        return &Queue3;
      }
    break;
  
  default:
    break;
  }

}
void mainRecieveMessage(HardwareSerial& comserial){
  if(comserial.available()>4){
    char readArray[MAX_MESSAGE_LENGTH]={};
    String msg_str=mainReading(comserial,readArray);
    struct Message msg;
    msg = Rebuildmsg(readArray);
    Serial.println(msg.Content);
    msg_str=msg.Content;  
    if(Checksum_Validation(readArray,msg)){
      Serial.println("CheckSum valid");
      Serial.println("Message Recieved :");
      Serial.println(msg.Content);
      uint16_t Code;
      if(msg.Content[1]=='M'){
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
        case 10: //FCWt 
          RecieveInfoWt(Queue1);
        break;
      }
      }
      else{
        switch(msg.Priority){
        case 1:// Er102
        case 2:// Wr102
          queuepointer = SelectQueuefromSerial(comserial);
          if(msg.Priority==1){queuepointer->Waiting_for_ErrOK=true;}
          else{queuepointer->Waiting_for_WrOK=true;}
          queuing(msg,queuepointer->MessageQueue);
        break;
        case 4://ErOk
          device.Waiting_for_ErrOk=false;
          queuepointer = SelectQueueforOk(1);
          queuing(msg,queuepointer->MessageQueue);
        break;
        case 6://WrOk
          device.Waiting_for_WrOk=false;
          queuepointer = SelectQueueforOk(2);
          queuing(msg,queuepointer->MessageQueue);
        break;
        default:
        queuepointer = SelectQueuefromSerial(comserial);
        if(msg.Content[1]=='R'){
          for (int i = 0; i <= Getlength(readArray); i++) {
          Myarr1[i] = readArray[i];
          }
          queuing(msg,queuepointer->MessageQueue);
        }else if(msg.Content[1]=='C'){
          for (int i = 0; i <= Getlength(readArray); i++) {
          Myarr2[i] = readArray[i];
          }
          queuing(msg,queuepointer->MessageQueue);
        }else if(msg.Content[1]=='F'){
          for (int i = 0; i <= Getlength(readArray); i++) {
          Myarr3[i] = readArray[i];
          }
          queuing(msg,queuepointer->MessageQueue);
        }else{

        }
        
        break;
        }
      }
      
    }else{
      Serial.println("CheckSum falied");
      queuepointer = SelectQueuefromSerial(comserial);
      SendingWhat(msg,*queuepointer);
    }
  }
    
  }