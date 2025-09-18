#include "GRBL_Control.h"

GRBL_Control::GRBL_Control(HardwareSerial &grblSerial)
  : _grblSerial(&grblSerial) {
  _joggingActive = false;
  _jogAxis = 'X';
  _jogSpeed = 100.0;
  _jogDistance = 0.1;
  _lastJogTime = 0;
  _jogInterval = 0;
  
  _lastStatus = "Unknown";
  _lastStatusRequest = 0;
  _statusInterval = 150; // ms between status requests
  _isMoving = false;
}

void GRBL_Control::begin(unsigned long baudrate) {
  _grblSerial->begin(baudrate);
  
  // Send wake-up commands to clear buffer
  delay(500);
  _grblSerial->write('\r');
  _grblSerial->write('\n');
  _grblSerial->write(0x18); // Soft reset
  delay(1000);
  while (_grblSerial->available()) _grblSerial->read(); // Clear buffer
  
  // Wait for GRBL initialization message
  delay(1000);
  while (_grblSerial->available()) {
    String response = _grblSerial->readStringUntil('\n');
    if (response.indexOf("Grbl") >= 0) break;
  }
}

void GRBL_Control::startJog(char axis, char dir, float speed, float distance) {
  axis = toupper(axis);
  if (axis != 'X' && axis != 'Y' && axis != 'Z') return;
  
  if (dir != '+' && dir != '-') return;

  _jogDir=dir;
  _joggingActive = true;
  _jogAxis = axis;
  _jogSpeed = speed;
  _jogDistance = abs(distance);
  
  // Calculate interval between jog commands
  _jogInterval = static_cast<unsigned long>((_jogDistance / _jogSpeed) * 60000);
  sendJogCommand();
}

void GRBL_Control::stopJog() {
  if (_joggingActive) {
    _grblSerial->write(0x85);  // Jog cancel
    _joggingActive = false;
  }
}

void GRBL_Control::doHoming() {
  sendCommand("$H");  // GRBL homing command
  _isMoving = true;
}

void GRBL_Control::setMachinePosition(float x, float y, float z) {
  String command = "G92";
  if (!isnan(x)) command += " X" + String(x, 3);
  if (!isnan(y)) command += " Y" + String(y, 3);
  if (!isnan(z)) command += " Z" + String(z, 3);
  sendCommand(command);
}

void GRBL_Control::moveTo(float x, float y, float z, float feedRate) {
  String command = "G90 G1";  // Use G1 for controlled feed move
  if (!isnan(x)) command += " X" + String(x, 3);
  if (!isnan(y)) command += " Y" + String(y, 3);
  if (!isnan(z)) command += " Z" + String(z, 3);
  command += " F" + String(feedRate);
  sendCommand(command);
  _isMoving = true;
}

void GRBL_Control::moveToZ(float z, float feedRate){
  String command = "G90 G1";  // Use G1 for controlled feed move
  if (!isnan(z)) command += " Z" + String(z, 3);
  command += " F" + String(feedRate);
  sendCommand(command);
  _isMoving = true;
}
void GRBL_Control::moveToXY(float x, float y, float feedRate){
  String command = "G90 G1";  // Use G1 for controlled feed move
  if (!isnan(x)) command += " X" + String(x, 3);
  if (!isnan(y)) command += " Y" + String(y, 3);
  command += " F" + String(feedRate);
  sendCommand(command);
  _isMoving = true;
}

void GRBL_Control::feedHold() {
  _grblSerial->write('!');
}

void GRBL_Control::cycleStart() {
  _grblSerial->write('~');
}

void GRBL_Control::softReset() {
  _grblSerial->write(0x18);
}

bool GRBL_Control::IsMoving() {
  // Send status request if enough time has passed
  unsigned long currentTime = millis();
  if (currentTime - _lastStatusRequest > _statusInterval) {
    _grblSerial->write('?');
    _lastStatusRequest = currentTime;
  }
  
  // Return the current movement state
  return _isMoving;
}

bool GRBL_Control::isJogging() const {
  return _joggingActive;
}

String GRBL_Control::getLastStatus() const {
  return _lastStatus;
}

void GRBL_Control::sendJogCommand() {
  String jogCommand = "$J=G91 ";
  jogCommand += _jogAxis;
  jogCommand += (_jogDir == '+') ? String(_jogDistance, 3) : String(-_jogDistance, 3);
  jogCommand += " F";
  jogCommand += String(abs(_jogSpeed));
  
  _grblSerial->println(jogCommand);
  _lastJogTime = millis();
  _isMoving = true;
}

void GRBL_Control::sendCommand(const String &command) {
  _grblSerial->println(command);
}

void GRBL_Control::Control_run() {
  // Process incoming responses
  while (_grblSerial->available()) {
    String response = _grblSerial->readStringUntil('\n');
    response.trim();
    processResponse(response);
  }
  
  // Handle continuous jogging
  if (_joggingActive && millis() - _lastJogTime > _jogInterval) {
    sendJogCommand();
  }
}

void GRBL_Control::processResponse(const String &response) {
  // Handle status reports
  if (response.startsWith("<")) {
    _lastStatus = response;
    
    // Update movement state
    if (response.indexOf("Idle") >= 0) {
      _isMoving = false;
    } else if (response.indexOf("Run") >= 0 || 
               response.indexOf("Jog") >= 0 || 
               response.indexOf("Home") >= 0) {
      _isMoving = true;
    }else{
      _isMoving = true;
    }
  }
  // Handle homing responses
  else if (response.startsWith("[MSG:]")) {
    if (response.indexOf("Homing") >= 0) {
      _lastStatus = response;
      _isMoving = true;
    }
  }
  // Handle command responses
  else if (response.startsWith("ok")) {
    // Commands like G92 will get 'ok' but don't affect movement state
  }
  else if (response.startsWith("error")) {
    // Handle errors if needed
    /*
    send error
    */
  }
}

bool GRBL_Control::isOkReceived(){
    if ( _grblSerial->available() >= 3 ) {
    String response = _grblSerial->readStringUntil('\n');
    response.trim();
    return response.startsWith("ok");
  }else{
    return false;
  }

}

float GRBL_Control::getZAxisMachinePos(){
  // Request status
  GRBL.write("?\n");  

  // Read reply
  String response = readGrblStatus;

  if (status.length() > 0) {
    // Example reply: <Idle|MPos:5.000,10.000,-2.000|WPos:0.000,5.000,-7.000>
    int mposIndex = response.indexOf("MPos:");
    if (mposIndex != -1) {
      String mposData = response.substring(mposIndex + 5, response.indexOf("|WPos")); 
      // mposData = "5.000,10.000,-2.000"

      // split by commas
      int firstComma = mposData.indexOf(',');
      int secondComma = mposData.indexOf(',', firstComma + 1);

      String zStr = mposData.substring(secondComma + 1);
      float zValue = zStr.toFloat();

      return zValue;
    }else{
      return NAN;
    }
  }else{
    return NAN;
  }

}

String readGrblStatus(unsigned long timeout = 200) {
  String response = "";
  unsigned long start = millis();

  while (millis() - start < timeout) {
    while (GRBL.available()) {
      char c = GRBL.read();
      if (c == '<') {  // Start of a status report
        response = "<";
      } else if (response.length() > 0) {
        response += c;
        if (c == '>') {  // End of status line
          return response;
        }
      }
    }
  }
  return ""; // timeout or no valid response
}

