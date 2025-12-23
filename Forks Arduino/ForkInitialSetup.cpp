#include "ForkInitialSetup.h"

void Fork_Initial_Mode::stopAllMotions() {
    CNC_Gate.CutMotion();

    Fork1_Z.CutMotion();
    Fork1_X.CutMotion();

    Fork2_Z.CutMotion();
    Fork2_X.CutMotion();

    Fork3_Z.CutMotion();
    Fork3_X.CutMotion();
}

void Fork_Initial_Mode::handleNoModeSetup() {

    // -------------------------
    // Homing controllers init ONCE
    // -------------------------
    if (!_Homing_initialized) {

        GateZ_Homing  = new ForkHoming(CNC_Gate, CNCGateZUpSensor,
                                       FAST_SPEED, SLOW_SPEED,
                                       +250, -20, +35);

        Fork1N_Homing = new ForkHoming(Fork1_Z, Fork1ZDnSensor,
                                       FAST_SPEED, SLOW_SPEED,
                                       -200, +15, -30);

        Fork3N_Homing = new ForkHoming(Fork3_Z, Fork3ZDnSensor,
                                       FAST_SPEED, SLOW_SPEED,
                                       -200, +15, -30);

        Fork1X_Homing = new ForkHoming(Fork1_X, FrameFork1XSensor,
                                       FAST_SPEED, SLOW_SPEED,
                                       -2000, +30, -45);

        Fork3X_Homing = new ForkHoming(Fork3_X, Fork3FrameXSensor,
                                       FAST_SPEED, SLOW_SPEED,
                                       +2500, -30, +45);

        Fork2X_Homing = new ForkHoming(Fork2_X, Fork2Fork3XSensor,
                                       FAST_SPEED, SLOW_SPEED,
                                       +2000, -30, +45);

        Fork2N_Homing = new ForkHoming(Fork2_Z, Fork2ZDnSensor,
                                       FAST_SPEED, SLOW_SPEED,
                                       -200, +15, -30);

        _Homing_initialized = true;
    }

    // -------------------------
    // Lubrication init ONCE (placeholder)
    // -------------------------
    if (!_Lubricating_initialized) {
        // TODO later
        _Lubricating_initialized = true;
    }
}

void Fork_Initial_Mode::handleNoModeLoop() {
    char mode = getUCharValue(&Shared_Vars[MODE]);

    if (mode != 0) return;

    char current_SubMode = getUCharValue(&Shared_Vars[NOMODE_SUBMODES]);
    static uint8_t last_SubMode = 255;

    if (last_SubMode != current_SubMode) {
        Serial.print("SubMode transition: ");
        Serial.print(last_SubMode);
        Serial.print(" -> ");
        Serial.println(current_SubMode);
    }

    switch (current_SubMode) {

        case 0: // None
            break;

        case 1: // Homing
            if (last_SubMode != current_SubMode) {
                last_SubMode = current_SubMode;
                handleHomingSetup();
            }
            handleHoming(); // caller can check completion via vars if you want
            break;

        case 2: // Lubrication
            if (last_SubMode != current_SubMode) {
                last_SubMode = current_SubMode;
                handleLubricationSetup();
            }
            handleLubrication();
            break;

        case 3: // Sleeping
            break;

        default:
            // unknown submode
            break;
    }
}

void Fork_Initial_Mode::handleHomingSetup() {

    // Safety: ensure controllers exist
    if (!_Homing_initialized || !GateZ_Homing || !Fork1N_Homing || !Fork3N_Homing ||
        !Fork1X_Homing || !Fork3X_Homing || !Fork2X_Homing || !Fork2N_Homing) {
        homingFailed = true;
        return;
    }

    GateZ_Homing->reset();
    Fork1N_Homing->reset();
    Fork3N_Homing->reset();
    Fork1X_Homing->reset();
    Fork3X_Homing->reset();
    Fork2X_Homing->reset();
    Fork2N_Homing->reset();

    homingFailed = false;

    gateZDone  = false;

    fork1NDone = false;
    fork1XDone = false;

    fork3NDone = false;
    fork3XDone = false;

    fork2XDone = false;
    fork2NDone = false;
}

bool Fork_Initial_Mode::handleHoming() {

    if (homingFailed) {
        stopAllMotions();
        return true; // finished (failed)
    }

    auto checkFail = [&](ForkHoming* mechanism) {
        if (mechanism && mechanism->hasError()) {
            homingFailed = true;
        }
    };

    if (!gateZDone) {
        gateZDone = GateZ_Homing->run();
        if (gateZDone) checkFail(GateZ_Homing);
    }

    if (!fork1NDone) {
        fork1NDone = Fork1N_Homing->run();
        if (fork1NDone) checkFail(Fork1N_Homing);
    }

    if (!fork3NDone) {
        fork3NDone = Fork3N_Homing->run();
        if (fork3NDone) checkFail(Fork3N_Homing);
    }

    // Fork 1 X depends ONLY on Fork 1 nails
    if (fork1NDone && !fork1XDone) {
        fork1XDone = Fork1X_Homing->run();
        if (fork1XDone) checkFail(Fork1X_Homing);
    }

    // Fork 3 X depends ONLY on Fork 3 nails
    if (fork3NDone && !fork3XDone) {
        fork3XDone = Fork3X_Homing->run();
        if (fork3XDone) checkFail(Fork3X_Homing);
    }

    // Fork 2 X depends on Fork 3 X and CNC Gate
    if (fork3XDone && gateZDone && !fork2XDone) {
        fork2XDone = Fork2X_Homing->run();
        if (fork2XDone) checkFail(Fork2X_Homing);
    }

    // Fork 2 nails depend ONLY on Fork 2 X
    if (fork2XDone && !fork2NDone) {
        fork2NDone = Fork2N_Homing->run();
        if (fork2NDone) checkFail(Fork2N_Homing);
    }

    return gateZDone &&
           fork1NDone && fork1XDone &&
           fork3NDone && fork3XDone &&
           fork2XDone && fork2NDone;
}

void Fork_Initial_Mode::handleLubricationSetup() {
    // TODO later
}

bool Fork_Initial_Mode::handleLubrication() {
    // TODO later
    return false;
}