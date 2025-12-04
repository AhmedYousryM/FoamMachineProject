#include "GRBL_Sender.h"

GRBL_Sender::GRBL_Sender(HardwareSerial &grblSerial)
  : _grblSerial(&grblSerial) {
  _filePosition = 0;
  _sendingActive = false;
  _fileFinished = false;
  _gcodeLineSent = false;
  _lastSentLine = "";
}

bool GRBL_Sender::Sender_begin(const String &filename) {
  _filename=filename;
  if (!SD.begin(53)) { // Mega2560 fixed SD CS pin
    return false;
  }
  return true;
}

bool GRBL_Sender::openFile() {
  closeFile(); // Close any open file first
  
  _gcodeFile = SD.open(_filename, FILE_READ);
  if (!_gcodeFile) {
    // try twice
          /*
      *****
      send warning message
      *****
      */
    _gcodeFile = SD.open(_filename, FILE_READ);
    if (!_gcodeFile) {
      /*
      *****
      send error message
      *****
      */
      _filePosition = 0;
      _fileFinished = false;
      return false;
    }
  }
  
  if (_filePosition > 0) {
    //_gcodeFile.seek(_filePosition);
    _filePosition=0;
  }
  _fileFinished = false;
  return true;
}

void GRBL_Sender::closeFile() {
  if (_gcodeFile) {
    _gcodeFile.close();
  }
}

bool GRBL_Sender::start() {
  if (!openFile()) {
    /*
    *****
    send error message
    *****
    */
    return false;
  }
  
  _sendingActive = true;
  _fileFinished = false;
  _gcodeLineSent = false;
  
  // Send the first line immediately
  return sendNextLine();
}

bool GRBL_Sender::pause() {
  if (!_sendingActive) {
    return false;
  }
  
  _grblSerial->write('!');  // Send GRBL feed hold
  _sendingActive = false;
  return true;
}

bool GRBL_Sender::continueSend() {
  if (_sendingActive || _fileFinished) {
    return false;
  }
  
  _grblSerial->write('~');  // Send GRBL cycle start
  _sendingActive = true;
  return true;
}

bool GRBL_Sender::Sender_run() {
  // Check if we've already finished
  if (_fileFinished) {
    return true;
  }

  // Process incoming GRBL responses
  while (_grblSerial->available()) {
    String response = _grblSerial->readStringUntil('\n');
    response.trim();
    processResponse(response);
  }
  
  // Send next line if ready
  if (_sendingActive && !_gcodeLineSent) {
    if (!sendNextLine()) {
      // End of file with no pending responses
      if (!_gcodeLineSent) {
        _fileFinished = true;
        return true;
      }
    }
  }
  
  return false;
}

bool GRBL_Sender::sendNextLine() {
  if (!_gcodeFile.available()) {
    return false;
  }
  
  char lineBuffer[128];
  int bytesRead = _gcodeFile.readBytesUntil('\n', lineBuffer, sizeof(lineBuffer) - 1);
  
  if (bytesRead > 0) {
    lineBuffer[bytesRead] = '\0';
    String gcodeLine(lineBuffer);
    gcodeLine.trim();
    
    // Remove comments
    int commentIndex = gcodeLine.indexOf(';');
    if (commentIndex >= 0) {
      gcodeLine = gcodeLine.substring(0, commentIndex);
      gcodeLine.trim();
    }
    
    // Skip empty lines
    if (gcodeLine.length() == 0) {
      _filePosition = _gcodeFile.position();
      return sendNextLine(); // Recursively try next line
    }
    
    // Send valid G-code line
    _lastSentLine = gcodeLine;
    _grblSerial->println(gcodeLine);
    _gcodeLineSent = true; // Mark as sent and waiting
    _filePosition = _gcodeFile.position();
    return true;
  }
  
  return false;
}

void GRBL_Sender::processResponse(const String &response) {
  if (response.startsWith("ok")) {
    _gcodeLineSent = false; // Ready to send next line
  } else if (response.startsWith("error")) {
    // Resend last line on error
    if (_lastSentLine.length() > 0) {
      _grblSerial->println(_lastSentLine);
      /*
      warning
      */
    }
  }else{
    /*
    error
    */
  }
}

bool GRBL_Sender::isSending() const {
  return _sendingActive;
}

bool GRBL_Sender::fileIsOpen() const {
  return _gcodeFile && _gcodeFile.available();
}

bool GRBL_Sender::isFinished() const {
  return _fileFinished;
}


    // spindle motion
    void SetSpindleSpeed(float s){
      _grblSerial->println("S"+String(s, 3));
    }
    void GRBL_Sender::SpindleStart(){
      _grblSerial->println("M3");

      // for Counter-Clockwise rotation
      // _grblSerial->println("M4");
    }
    void GRBL_Sender::SpindleStop(){
      _grblSerial->println("M5");
    }

    // 2nd coordinates
    void GRBL_Sender::UseMachineCoordinates(){
      _grblSerial->println("G53");
    }

    void GRBL_Sender::UseWorkpieceCoordinates(){
      _grblSerial->println("G54");
    }

    void GRBL_Sender::SetWorkpieceCoordinates(float x, float y, float z){
      String command = "G10 L3 P1";  
      if (!isnan(x)) command += " X" + String(x, 3);
      if (!isnan(y)) command += " Y" + String(y, 3);
      if (!isnan(z)) command += " Z" + String(z, 3);
      _grblSerial->println(command);
    }

    void GRBL_Sender::SetWorkpieceCoordinatesZ( float z){
      String command = "G10 L3 P1";  
      if (!isnan(z)) command += " Z" + String(z, 3);
      _grblSerial->println(command);
    }

    void GRBL_Sender::SetWorkpieceCoordinatesXY(float x, float y ){
      String command = "G10 L3 P1";  
      if (!isnan(x)) command += " X" + String(x, 3);
      if (!isnan(y)) command += " Y" + String(y, 3);
      _grblSerial->println(command);
    }