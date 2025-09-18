void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
switch (RC_variables(Mode)){
  case 4: // Sensor Test
     SensorTest.loop();
  break;
  case 3: // Move Motors
  
  break;
  case 0: // Move Motors
  
  break;
  default:
  
  break;
  }
}
