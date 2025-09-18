#pragma once
#ifndef COMMUNICATION_H
#define COMMUNICATION_H


#include "String_Buffer.h"
#include "MyLinkedList.h"
#include <Arduino.h>
#include <CRC8.h>
#include <CRC.h>


class Communication{
  public:
  Communication(Stream& s);
  
void save_message(String str);

bool send_variable(String var_str);
bool Ask_for_variable(String var_str);
bool Variable_is_answered(String var_str);
bool All_is_answered();
void To_screen(String str);
void send_message();
bool recieve();
void List_Update();  
size_t (*recieved_on_screen)(String str);
template <typename T>
void send_variable(T* var);
template <typename T>
void add_var(String str,T* var);
template <typename T>
void add_var(String str,T* var,T dif);
template <typename T>
void add_var(String str,T* var,T Up,T Low);
void recieve(String recieved_string);




  private:
  Stream* mystream;
  class variable{
    public:
     String var_str;
     void* var_pointer;
     char var_type;
     char dif_type; // 'd' for diffrence, 'r' for range, 'n' no check, 'f' diffrence but still did not entered the first value
     void* Up;
     void* Low;
     bool asked=false;
     unsigned long ask_time;
     bool responded=false;
  };

  class function{
    public:
    String str;
    void (*func) ();
  };

static function no_func;
MyLinkedList<function> Func_List;
function myFunc_search(String str);
void add_func(String str,void (*func) ());
void send_String(String str);

static variable no_var;
MyLinkedList<variable> Var_List;
variable myList_search(String str);
variable myList_search_pointer(void* pointer);
template <typename T>
variable myList_search(T* var);
const char TypeOf(const bool*)  ;  
const char TypeOf(const double*); 
const char TypeOf(const float*) ; 
const char TypeOf(const int*)   ;   
const char TypeOf(const long*)  ; 
template <typename T>
bool check(variable var,T val);
template <typename T>
bool Check_and_Assing(variable var,String val);
String_Buffer myBuffer;
};
/*
template <typename T>
void send_variable(T* var){
  variable myvar = myList_search_pointer(void* var);
  if ( &myvar != &no_var){
  String var_str=myvar.var_str;
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
  save_message(var_str);
  return true;
  }
  else{
    To_screen("No variable send");
    return false;
  }
}
*/
template <typename T>
void Communication::add_var(String str,T* var_pointer){
  variable *new_var= new variable();
  new_var->var_str=str;
  new_var->var_pointer=var_pointer;
  new_var->var_type=TypeOf(var_pointer);
  Var_List.add_element(*new_var);
  new_var->dif_type='n';
}

template <typename T>
void Communication::add_var(String str,T* var_pointer,T dif){
  add_var( str , var_pointer );
  variable var =  Var_List.get_element();
  var.dif_type='f';
  T* p = new T(dif);
  var.Up= p;
}
template <typename T>
void Communication::add_var(String str,T* var_pointer,T n1,T n2){
  add_var( str , var_pointer );
  variable var =  Var_List.get_element();
  var.dif_type='r';
  T* p1 = new T(max(n1,n2));
  T* p2 = new T(min(n1,n2));
  var.Up= p1;
  var.Low= p1;
}


#endif
