#include "ForkHoming.h"

ForkHoming::ForkHoming(Motor& motor, SensorFilter& Sensor, float fastSpeed, float slowSpeed, 
                        long longMove, long shortMove_1, long shortMove_2)
    : _motor(motor),
      _sensor(Sensor),
      _fastSpeed(fastSpeed),
      _slowSpeed(slowSpeed),
      _longMove(longMove),
      _shortMove_1(shortMove_1),
      _shortMove_2(shortMove_2),
      _state(MOVE_FAST)
{}

void ForkHoming::reset() {
    _state = MOVE_FAST;
    _firstTime = true;
    _error = false;
}

bool ForkHoming::run() {
    switch (_state) {

        case MOVE_FAST:
            _motor.enableDriver();
            _motor.setMaxSpeed(_fastSpeed);
            _motor.move(_longMove);          // toward sensor
            _state = SEEK_SENSOR_FAST;
            break;

        case SEEK_SENSOR_FAST:
            _motor.run();
            if (_sensor.readFiltered()) {
                _motor.CutMotion();               // or CutMotion() if you really want hard stop
                _firstTime = true;
                _state = BACK_OFF_1;
            }
            break;

        case BACK_OFF_1:
            if (_firstTime) {
                _motor.setMaxSpeed(_slowSpeed);
                _motor.move(_shortMove_1);     // max backoff distance
                _firstTime = false;
            }
            _motor.run();
            if (_motor.reachTarget()) {        // backoff finished
                _motor.stop();
                if (!_sensor.readFiltered()) { // cleared
                    _firstTime = true;
                    _state = SEEK_SENSOR_SLOW;
                } else {
                    // error: still pressed after full backoff
                    _state = ERROR; // better: ERROR
                }
            }
            break;

        case SEEK_SENSOR_SLOW:
            // Approach slowly until sensor triggers again
            if (_firstTime) {
                _motor.setMaxSpeed(_slowSpeed);
                _motor.move(_shortMove_2);   // toward sensor (small travel is usually enough)
                _firstTime = false;
            }
            _motor.run();
            if (_sensor.readFiltered()) {
                _motor.CutMotion();
                _motor.setCurrentPosition(0);
                _firstTime = true;
                _state = FINAL_BACKOFF;
            }
            break;

        case FINAL_BACKOFF:
            // Move away from sensor to leave it unpressed
            if (_firstTime) {
                _motor.setMaxSpeed(_slowSpeed);
                _motor.move(_shortMove_1);   // away from sensor
                _firstTime = false;
            }
            _motor.run();
            if (_motor.reachTarget()) {
                _motor.stop();
                if (!_sensor.readFiltered()) {
                    _firstTime = true;
                    _state = DONE;
                } else {
                    // error: still pressed after final backoff
                    _state = ERROR; // better: ERROR
                }
            }
            break;

        case ERROR:
            // send Error
            _motor.disableDriver();
            _error = true;
            _motor.CutMotion();
            return true;   // terminal (failed)

        case DONE:
            _motor.disableDriver();
            return true;

    }

    return false;
}