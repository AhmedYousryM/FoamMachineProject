#include "ForkProductionOperationMode.h"

void handleProduction(){
  char mode = getUCharValue(&Shared_Vars[MODE]);
  if(mode == 7){
    char Production_SubMode = getUCharValue(&Shared_Vars[PRODUCTION_SUBMODES]);
    switch(Production_SubMode) {
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