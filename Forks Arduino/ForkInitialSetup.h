#ifndef FORK_INITIAL_SETUP_H
#define FORK_INITIAL_SETUP_H

#include <Arduino.h>
#include "Shared_Variables.h"
#include "Shared_Functions.h"

#include "ForkMotors.h"
#include "ForkSensors.h"
#include "ForkHoming.h"

class Fork_Initial_Mode {
    
    public:
        void handleNoModeSetup();
        void handleNoModeLoop();

        void handleHomingSetup();
        bool handleHoming();

        void handleLubricationSetup();   // optional
        bool handleLubrication();        // optional

    private:
        // One-time init guards
        bool _Homing_initialized      = false;
        bool _Lubricating_initialized = false;

        // Homing controllers (constructed once)
        ForkHoming* GateZ_Homing  = nullptr;
        ForkHoming* Fork1N_Homing = nullptr;
        ForkHoming* Fork3N_Homing = nullptr;
        ForkHoming* Fork1X_Homing = nullptr;
        ForkHoming* Fork3X_Homing = nullptr;
        ForkHoming* Fork2X_Homing = nullptr;
        ForkHoming* Fork2N_Homing = nullptr;

        // Homing status
        bool homingFailed = false;

        bool gateZDone  = false;

        bool fork1NDone = false;
        bool fork1XDone = false;

        bool fork3NDone = false;
        bool fork3XDone = false;

        bool fork2XDone = false;
        bool fork2NDone = false;

        // helper: stop all motors immediately
        void stopAllMotions();
};

#endif