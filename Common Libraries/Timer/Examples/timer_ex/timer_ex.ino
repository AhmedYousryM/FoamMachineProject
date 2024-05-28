#include <Timer.h>

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  Timer.Calling_func(&printTime);
  Timer.SetTime_Micros(10000);
  Timer.Repeat();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void printTime()
{
  Serial.println(millis());
}
