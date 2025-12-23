#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>
#include <math.h>

#define LIBRARY_NAME Mega2560Stepper
#include <Mega2560Stepper.h>

class Motor : public LIBRARY_NAME {
protected:
    float pulse_per_mm = 1.0f;

    byte motor_timer  = 0;
    byte motor_dirPin = 255;
    byte motor_enPin  = 255;

    static constexpr bool ENABLE_ACTIVE_HIGH = true; // your wiring

    // Map STEP pin -> Timer number used by Mega2560Stepper (1,3,4,5)
    static byte stepPinToTimer(byte stepPin) {
        switch (stepPin) {
            case 12: return 1; // Timer1  -> STEP pin 12
            case 2:  return 3; // Timer3  -> STEP pin 2
            case 7:  return 4; // Timer4  -> STEP pin 7
            case 45: return 5; // Timer5  -> STEP pin 45
            default: return 0; // invalid STEP pin for this library
        }
    }

public:

    //Motor() = delete;
    Motor() : LIBRARY_NAME(), pulse_per_mm(1.0f) {}

    // User passes STEP PIN number (2 / 7 / 12 / 45), not timer number
    // pin1 = DIR, pin2 = EN
    Motor(byte stepPin, byte pin1, byte pin2, float ppmm)
        : LIBRARY_NAME(stepPinToTimer(stepPin), pin1, pin2),
          pulse_per_mm(ppmm),
          motor_timer(stepPinToTimer(stepPin)),
          motor_dirPin(pin1),
          motor_enPin(pin2)
    {
        // Optional but strongly recommended guard against invalid STEP pins
        // Place it here at the start of the constructor body (exactly here).
        if (motor_timer == 0) {
            // invalid STEP pin -> leave motor disabled safely
            return;
        }

        pinMode(motor_enPin, OUTPUT);
        disableDriver(); // safest default at boot
    }

    void enableDriver() {
        digitalWrite(motor_enPin, ENABLE_ACTIVE_HIGH ? HIGH : LOW);
    }

    void disableDriver() {
        digitalWrite(motor_enPin, ENABLE_ACTIVE_HIGH ? LOW : HIGH);
    }

    // mm wrappers
    void moveTo(float mm_pos) { LIBRARY_NAME::moveTo(lround(mm_pos * pulse_per_mm)); }
    void move(float mm_pos)   { LIBRARY_NAME::move  (lround(mm_pos * pulse_per_mm)); }

    void setMaxSpeed(float mm_per_second) {
        LIBRARY_NAME::setMaxSpeed(lround(mm_per_second * pulse_per_mm));
    }

    void setAcceleration(float mm_per_sec_sq) {
        LIBRARY_NAME::setAcceleration(lround(mm_per_sec_sq * pulse_per_mm));
    }

    float speed()           { return LIBRARY_NAME::speed() / pulse_per_mm; }
    float distanceToGo()    { return LIBRARY_NAME::distanceToGo() / pulse_per_mm; }
    float targetPosition()  { return LIBRARY_NAME::targetPosition() / pulse_per_mm; }
    float currentPosition() { return LIBRARY_NAME::currentPosition() / pulse_per_mm; }

    void setCurrentPosition(float mm_pos) {
        LIBRARY_NAME::setCurrentPosition(lround(mm_pos * pulse_per_mm));
    }
};

#endif