#include "Communication.h"

Communication::Communication(Stream& s){
  mystream = &s;
}


const char Communication::TypeOf(const bool*)   { static const char type = 'b';     return type; }
const char Communication::TypeOf(const double*) { static const char type = 'd';     return type; }
const char Communication::TypeOf(const float*)  { static const char type = 'f';     return type; }
const char Communication::TypeOf(const int*)    { static const char type = 'i';     return type; }
const char Communication::TypeOf(const long*)   { static const char type = 'l';     return type; }



template <typename T>
bool Communication::check(variable var,T val){
  switch(var.dif_type){
    case 'r':
    T* upr = var.Up ;
    T* low = var.Low;
    if(val<*upr && val>*low){
      return true;
    }
    else{
      To_screen("variable value "+var.var_str +" is out of the range");
      return false;
    }
      break;
    case 'd':
    T* num  = var.Up ;
    T* me = var.var_pointer;
    if( abs( *me -val ) < *num ){
      return true;
    }
    else{
      To_screen("variable value "+var.var_str +" is changes rapidly");
      return false;
    }
      break;
      case 'n':
        return true;
      break;
      case 'f':
        var.dif_type='d';
        return true;
      break;
    default:
      To_screen("variable check has problem");
      return false;
      break;
  }
}
/*
template <typename T>
Communication::variable Communication::myList_search(T* var){
  Var_List.from_begining();
  int n = Var_List.size_;
  bool b = false;
   for(int i=0;i<n;i++){
    Var_List.next();
      if(var==Var_List.get_element().var_pointer){
        b=true;
        break;
      }
   }
   if (b=true){
    return Var_List.get_element();
   }
   else{
    To_screen(Var_List.get_element().var_str+" variable is not found in the list");
    return no_var;
   }
   
}*/
Communication::variable Communication::myList_search(String str){
  Var_List.from_begining();
  int n = Var_List.size_;
  bool b = false;
   for(int i=0;i<n;i++){
     //To_screen(String(Var_List.get_element().var_type));
      if(str.equals(Var_List.get_element().var_str)){
        b=true;
        
        break;
      }
      Var_List.next();
   }
   if (b=true){
    return Var_List.get_element();
   }
   else{
    To_screen(str+" variable is not found in the list");
    return no_var;
   }
   
}
Communication::function Communication::myFunc_search(String mystr){
  Func_List.from_begining();
  int n = Func_List.size_;
  bool b = false;
   for(int i=0;i<n;i++){
     //To_screen(String(Var_List.get_element().var_type));
      if(mystr.equals(Func_List.get_element().str)){
        b=true;
        
        break;
      }
      Var_List.next();
   }
   if (b=true){
    return Func_List.get_element();
   }
   else{
    To_screen(mystr+" fuction is not found in the list");
    return no_func;
   }
   
}

Communication::variable Communication::myList_search_pointer(void* pointer){
  Var_List.from_begining();
  int n = Var_List.size_;
  bool b = false;
   for(int i=0;i<n;i++){
     //To_screen(String(Var_List.get_element().var_type));
      if(pointer==Var_List.get_element().var_pointer){
        b=true;
        
        break;
      }
      Var_List.next();
   }
   if (b=true){
    return Var_List.get_element();
   }
   else{
    To_screen("Variable pointer is not found in the list");
    return no_var;
   }
   
}

template <typename T> String Value_to_String(T value){
  // create union to convert the data type to char array
   union convert_union{
    T myval;
    char char_array[sizeof(T)+1];
    } my_union;
  // assign value to the union.
    my_union.myval=value;
  // convert to string
    String my_string = my_union.char_array;
    return my_string;
}
/*
template  String Value_to_String(double);
template  String Value_to_String(float);
template  String Value_to_String(int);
template  String Value_to_String(long);*/
 template <typename T> T String_to_vlaue(String str){
  // create union to convert the data type to char array
   union convert_union{
    T myval;
    char char_array[sizeof(T)+1];
    } my_union;
    str.toCharArray(my_union.char_array,sizeof(T)+1);
    
        
    return  my_union.myval;
}
/*
template  float String_to_vlaue<float>(String);
template  double String_to_vlaue<double>(String);
template  int String_to_vlaue<int>(String);
template  long String_to_vlaue<long>(String);*/
template <typename T>
bool Communication::Check_and_Assing(variable var,String val_str){
  T val_num =  String_to_vlaue<T>(val_str);
        if( check(var,val_num) ){
          T* d = var.var_pointer;
          *d = val_num;
          var.responded=true;
          return true;
        }
        else{
          return false;
        }
}
void Communication::recieve(String recieved_string){
     ///// foam 1 width = 12.1234
     ///// foam 1 available is true
    if( recieved_string.startsWith("SC:") ){
      To_screen( recieved_string.substring(3) );
    }
    else if ( recieved_string.indexOf('=') != -1 ){
      // check CRC 
      byte len = recieved_string.length();
      char c = recieved_string.charAt(len-1);
      recieved_string.remove(len-1);
      CRC8 crc;
      len = recieved_string.length();
      char buf[len];
      svar_str.toCharArray(buf,len);
      crc.add( (uint8_t*)buf , len );
      if (c==crc.calc()){
      int ind = recieved_string.indexOf('=');
      String var_str=recieved_string.substring(0,ind-2);
      variable myvar = myList_search(var_str);
      if ( &myvar != &no_var ){
        String var_value=recieved_string.substring(ind+2);
        switch (myvar.var_type){
          case 'b':
            bool* b=myvar.var_pointer;
            if (var_value.length()<=2){
              switch(var_value.charAt(0)){
                case '0':
                *b = 0;
                myvar.responded=true;
                  break;
                case '1':
                *b = 1;
                myvar.responded=true;
                  break;
              }
            }
            else{
              To_screen("wrong bool value");
            }
            break;
          case 'l':
            Check_and_Assing<long>(myvar,var_value);
            break;
          case 'f':
            Check_and_Assing<float>(myvar,var_value);
            break;
          case 'i':
            Check_and_Assing<int>(myvar,var_value);
            break;
          case 'd':
            Check_and_Assing<double>(myvar,var_value);
            break;
        }
      }
      }else{
        To_screen("wrong assignment statement");
      }
   }
   else if(recieved_string.endsWith("?") != -1){
    int ind = recieved_string.indexOf('?');
    String var_str=recieved_string.substring(0,ind-1);
    variable var = myList_search(var_str);
      if ( &var != &no_var){
        var_str.concat("?");
        send_variable(var_str);
      }
      else{
        To_screen("wrong question for undefined variable "+var_str);
      }
   }
   else{
    if(){
      function myfun = myFunc_search(recieved_string);
      myfun.func();
    }else{
      To_screen("wrong recieved sentence: "+recieved_string);
    }
   }
}
void Communication::save_message(String str){
  myBuffer.Write(str);
  Serial.println(str);
}

void Communication::send_String(String str){
  save_message(str);
}

bool Communication::send_variable(String var_str){
  variable myvar = myList_search( var_str );
  if ( &myvar != &no_var){
  var_str.concat(" = ");     
  switch( myvar.var_type ){
          case 'b':
            bool* b = myvar.var_pointer;
              if(*b){
                var_str.concat("1");
              }else{
                var_str.concat("0");
              }
            break;
          case 'l':
            long* l=myvar.var_pointer;
            var_str.concat(Value_to_String(*l));
            break;
          case 'f':
            float* f=myvar.var_pointer;
            var_str.concat(Value_to_String(*f));
            break;
          case 'i':
            int* i=myvar.var_pointer;
            var_str.concat(Value_to_String(*i));
            break;
          case 'd':
            double* d=myvar.var_pointer;
            var_str.concat(Value_to_String(*d));
            break;
          default:
            To_screen("variable type is not one of the premissable types");
            Serial.print("oo");
            break;
        }
  // Add CRC byte to this message
  CRC8 crc;
  byte len = svar_str.length();
  char buf[len];
  svar_str.toCharArray(buf,len);
  crc.add( (uint8_t*)buf , len );
  var_str.concat( crc.calc() );
  // send message
  save_message(var_str);
  return true;
  }
  else{
    To_screen(var_str+" is undefined variable name to send");
    return false;
  }
}
bool Communication::Variable_is_answered(String var_str){
  variable var = myList_search(var_str);
  if ( &var != &no_var){
    if (var.responded==true){
      return true;
    }else{
      return false;
    }
  }
  else{
    To_screen("undefined variable to check written in code: "+var_str);
    return false;
  }
}
bool Communication::All_is_answered(){
  Var_List.from_begining();
  variable var;
  int n = Var_List.size_;
  bool b = true;
   for(int i=0;i<n;i++){
    Var_List.next();
    var=Var_List.get_element();
      if(var.asked==true && var.responded==false){
        b=false;
        break;
      }
   }
   return b;
}
bool Communication::Ask_for_variable(String var_str){
  // ask once for each variable per second
  variable var = myList_search(var_str);
  if ( &var != &no_var){
    if (var.asked == false){
        var.asked=true;
        var.ask_time=millis();
        var.responded=false;
        var_str.concat("?");
        save_message(var_str);
        return true;
    }
    else{
      // wait one second then ask again
      if (var.ask_time+1000>millis()){
        To_screen("undefined variable written in code: "+var_str);
        var.asked=false;
        return false;
        if(var.responded==false){
          To_screen("Question is not answered for var: "+var_str);
        }
      }
    }
  }
  else{
    To_screen("undefined variable written in code: "+var_str);
    return false;
  }
}

void Communication::To_screen(String str){
  str=String("SC:"+str);
  //save_message(str);
  recieved_on_screen(str);
  Serial.println(str);
}
void Communication::send_message(){
  String str = myBuffer.Read();
  if (str.equals("None")){
    // do nothing
  }else{
    mystream->println(str);
    if(str.length()>64){
      To_screen("message length is more than buffer length (64 char)");
    }
  }
}
bool Communication::recieve(){
  static char char_array[64];
  static byte i;
  char c;
  bool rr=false;
  while ( (i<=64) && ( mystream->available() ) )  
  {  
    c = mystream->read();
    char_array[i]=c;
    i++;
    if(c=='\0'){
      String str = String(char_array);
      recieve(str);
      Serial.println("rec:"+str);
      i=0;
      rr=true;
    }
    
  }  
 return rr; 
}

void Communication::List_Update(){
  int n = Var_List.size_;
  To_screen(String(n));
  Var_List.from_begining();
   for(int i=0;i<n;i++){
     To_screen(Var_List.get_element().var_str);
      Var_List.next();
   }

}

void Communication::add_func(String str,void (*func) ()){
  function *new_fun= new function();
  new_fun->str=str;
  new_fun->func=func;
}