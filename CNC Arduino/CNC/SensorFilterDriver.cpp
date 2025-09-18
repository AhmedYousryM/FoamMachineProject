#include "SensorFilterDriver.h"

SensorFilter::SensorFilter(uint8_t pin, bool reverse) {
  _pin = pin;
  _reverse = reverse;
  pinMode(_pin, INPUT);
  
  // Initialize states
  _currentState = digitalRead(_pin);
  if (_reverse) _currentState = !_currentState;
  _filteredState = _currentState;
  _lastState = _currentState;
  
  // Default filter parameters
  _minCount = 2;
  _minDuration = 1000; // 1ms
  
  // Initialize filter counters
  _count = 0;
  _firstChangeTime = 0;
}

bool SensorFilter::readCurrent() {
  _currentState = digitalRead(_pin);
  if (_reverse) _currentState = !_currentState;
  return _currentState;
}

bool SensorFilter::readFiltered() {
  update();
  return _filteredState;
}

void SensorFilter::update() {
  // Read current state
  bool newState = digitalRead(_pin);
  if (_reverse) newState = !newState;
  
  // Update current state
  _currentState = newState;
  
  // Update filter logic
  _updateFilter(newState);
}

void SensorFilter::_updateFilter(bool newState) {
  // Check for state change
  if (newState != _lastState) {
    // State changed, reset counters
    _count = 1;
    _firstChangeTime = micros();
    _lastState = newState;
  } else {
    // State is the same, increment counter if needed
    if (_count > 0 && _count < _minCount) {
      _count++;
    }
  }
  
  // Check if we should update filtered state
  if (_count >= _minCount) {
    unsigned long duration = micros() - _firstChangeTime;
    if (duration >= _minDuration) {
      _filteredState = newState;
      _count = 0; // Reset counter after confirming change
    }
  }
}

void SensorFilter::setFilterParameters(uint16_t minCount, unsigned long minDuration) {
  _minCount = minCount;
  _minDuration = minDuration;
}
