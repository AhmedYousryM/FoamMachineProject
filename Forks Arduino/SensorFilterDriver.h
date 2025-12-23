#ifndef SENSOR_FILTER_DRIVER_H
#define SENSOR_FILTER_DRIVER_H

#include "Arduino.h"

class SensorFilter {
  public:
    // Constructor with pin number and reverse flag
    SensorFilter(uint8_t pin, bool reverse = false);
    
    // Read current sensor value (immediate, no filtering)
    bool readCurrent();
    
    // Read filtered sensor value (with debounce logic)
    bool readFiltered();
    
    // Set filter parameters
    void setFilterParameters(uint16_t minCount, unsigned long minDuration);
    
    // Must be called frequently to update filter state
    void update();
    
  private:
    uint8_t _pin;
    bool _reverse;
    bool _currentState;
    bool _filteredState;
    
    // Filter parameters
    uint16_t _minCount;
    unsigned long _minDuration;
    
    // Filter variables
    bool _lastState;
    uint16_t _count;
    unsigned long _firstChangeTime;
    
    // Internal state update
    void _updateFilter(bool newState);
};

#endif