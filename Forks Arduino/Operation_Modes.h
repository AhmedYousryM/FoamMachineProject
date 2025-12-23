#ifndef Forks_Modes_h
#define Forks_Modes_h

#include <Arduino.h>
#include <NonBlockingSequence.h>
#include "Shared_Variables.h"
#include "Shared_Functions.h" 
#include "ForkInitialSetup.h"
#include "ForkPreProductionMode.h"
#include "ForkProductionOperationMode.h"

/*void Do_Lubrication(){
  Move_Fork(1200, F_1);
  Move_Fork(1300, F_2);
  Move_Fork(3400, F_3);
  Move_Fork(200, F_1);
  Move_Fork(3000, F_2);
}*/

class Operation_Modes {

  public:
    void Loop();
    bool IsModeFinished();

  private :
    uint8_t lastMode;
    bool Mode_finished=false;
};

#endif