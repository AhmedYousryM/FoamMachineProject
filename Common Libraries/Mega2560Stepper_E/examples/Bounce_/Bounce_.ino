
#include <Mega2560Stepper.h>

// New Object
Mega2560Stepper stepper; 

void setup()
{  
  Serial.begin(9600);
  // constructor Mega2560Stepper( timer ,DirPin , EnablePin )
  // Possible  timer values are 1  , 3 , 4 & 5
  // with corresponding StepPin 12 , 2 , 7 , 45
  stepper = Mega2560Stepper(1,11,13);
  // step , direction and enable pin are 12 , 11 and 13

  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
  stepper.moveTo(10000);
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());
      
    Serial.print( stepper.currentPosition() );
    Serial.print("   ");
    Serial.print( stepper.speed() );
    Serial.print("   ");
    Serial.println( stepper.targetPosition() );


    
    stepper.run();
}
