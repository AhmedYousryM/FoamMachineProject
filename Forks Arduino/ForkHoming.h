#ifndef FORK_HOMING_H
#define FORK_HOMING_H

#include "ForkMotors.h"
#include "ForkSensors.h"

class ForkHoming {
public:
    ForkHoming(Motor& motor, SensorFilter& sensor,
               float fastSpeed, float slowSpeed,
               long longMove, long shortMove_1, long shortMove_2);

    void reset();

    // Returns true when homing has finished (DONE or ERROR)
    bool run();

    // Query result
    bool hasError() const { return _error; }
    bool isDone()   const { return _state == DONE; }

private:
    enum State : uint8_t {
        MOVE_FAST,
        SEEK_SENSOR_FAST,
        BACK_OFF_1,
        SEEK_SENSOR_SLOW,
        FINAL_BACKOFF,
        ERROR,
        DONE
    };

    State _state = MOVE_FAST;

    bool _firstTime = true;
    bool _error     = false;

    Motor& _motor;
    SensorFilter& _sensor;

    float _fastSpeed;
    float _slowSpeed;
    long  _longMove;
    long  _shortMove_1;
    long  _shortMove_2;
};

#endif