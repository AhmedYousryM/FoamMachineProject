#include "ForkPreProductionMode.h"

void handlePreProduction(){
  char mode = getUCharValue(&Shared_Vars[MODE]);
  if(mode == 6){
    char PreProduction_SubMode = getUCharValue(&Shared_Vars[PREPRODUCTION_SUBMODES]);
    switch(PreProduction_SubMode) {
        case 0: // None
          break;
        case 1: // start postion 1
          break;
        case 2: // Z Axis Setup
          break;
        case 3: // Sleeping
          break;
    }
  }
}