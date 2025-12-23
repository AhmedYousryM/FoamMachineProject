#include "Operation_Modes.h"

void Operation_Modes::Loop(){

    uint8_t currentMode = getUCharValue(&Shared_Vars[MODE]);
    static uint8_t lastMode = 255;

    if(lastMode != currentMode) {
      Serial.print("Mode transition: ");
      Serial.print(lastMode);
      Serial.print(" -> ");
      Serial.println(currentMode);
    }

    switch(currentMode){
        case 0:  // No Mode
            if (lastMode != currentMode){
              handleInitialSetup();
              lastMode = currentMode;
            }
            handleInitialLoop();
            break;
        case 1:  // Move Motors
            break;
        case 2:  // Sensor Test
            break;
        case 3:  // Machine Calibration
            break;
        case 4:  // pre Simulation
            break;
        case 5:  // Simulation
            break;
        case 6:  // Pre Production
            if (lastMode != currentMode){
              handlePreProductionSetup();
              lastMode = currentMode;
            }
            handlePreProduction();
            break;
        case 7:  // Production
            if (lastMode != currentMode){
              handleProductionSetup();
              lastMode = currentMode;
            }
            handleProduction();
            break;
        case 8:  // Error
            if (lastMode != currentMode){
              //handleErrorSetup();
              lastMode = currentMode;
            }
            //handleError();
            break;
        default:
            // Log and auto-recover
            if (lastMode != currentMode){
              Serial.print("Invalid mode ");
              Serial.print(currentMode);
              Serial.println(", switching to Error mode");
              lastMode = currentMode;
            }
            //setUCharValue("MODE", 8); // Default error mode
            //handleError();
            break;
    }
}