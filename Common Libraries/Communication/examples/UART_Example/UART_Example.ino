#include "Communication.h"

Communication com(Serial1);
long l;
float f;
bool b;

MyLinkedList<int> Var_List=MyLinkedList<int>();

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
Serial1.begin(9600);
com.recieved_on_screen=print_on_computer;
com.add_var<long>("l value",&l,0,200);
com.add_var<float>("this float",&f,15.2);
com.add_var("true or false",&b);

Var_List.add_element(100);
}
size_t print_on_computer(String str){
  Serial.println(str);
}
void loop() {
  // put your main code here, to run repeatedly:
com.send_message();
com.recieve();

  Serial.print(f,5);
  Serial.print("  ");
  Serial.print(l);
  Serial.print("  ");
  if(b==true){
    Serial.print("t");
  }else{
    Serial.print("f");
  }
  delay(1);
  Serial.println();

  com.Ask_for_variable("true or false");
  com.List_Update();
  /*bool is_it_rec = com.Variable_is_answered("true or false");
  if( is_it_rec ){
    //Serial.print(b);
  }*/
  //Serial.print(is_it_rec);
com.send_message();
com.recieve();  
}
