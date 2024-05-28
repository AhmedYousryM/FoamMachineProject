#include "Communication.h"

Communication com(Serial1);
long llll=2;
float f123=1;
bool b___;
void setup() {
  // put your setup code here, to run once:

Serial1.begin(9600);
Serial.begin(9600);

com.recieved_on_screen=print_on_computer;
com.add_var<long>("l value",&llll);
com.add_var<float>("this float",&f123);
com.add_var("true or false",&b___);

}
size_t print_on_computer(String str){
  Serial1.println(str);
}
void loop() {
  // put your main code here, to run repeatedly:
llll+=2;
Serial.print(llll);
com.send_variable("l value");
com.send_message();
com.recieve();
Serial.print("  ");

delay(10);
f123*=1.01;
Serial.print(f123);
com.send_variable("this float");
com.send_message();
com.recieve();
delay(10);
Serial.print("  ");

b___!=b___;
Serial.print(b___);
com.send_message();
com.recieve();
delay(10);
Serial.println("  ");
com.send_message();
com.recieve();
delay(10);

com.send_message();
com.recieve();
delay(10);
com.send_message();
com.recieve();
delay(10);
}
