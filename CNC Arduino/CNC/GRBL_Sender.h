#ifndef GRBL_Sender_h
#define GRBL_Sender_h

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "CNCConstantMacros.h"
#include "Communication.h"
class GRBL_Sender {
  public:
    // Constructor with filename
    GRBL_Sender(HardwareSerial &grblSerial);
    
    // Initialize the sender
    bool Sender_begin(const String &filename);
    
    // Control functions
    bool start();       // Start sending
    bool pause();       // Pause sending
    bool continueSend(); // Continue sending (resume)
    
    // Main processing function - returns true when all GCode is sent
    bool Sender_run();
    
    // Status checks
    bool isSending() const;
    bool fileIsOpen() const;
    bool isFinished() const;

    // spindle motion
    void SetSpindleSpeed(float s);
    void SpindleStart();
    void SpindleStop();

    // 2nd coordinates
    void UseMachineCoordinates();
    void UseWorkpieceCoordinates();
    void SetWorkpieceCoordinates(float x, float y, float z);
    void SetWorkpieceCoordinatesZ( float z);
    void SetWorkpieceCoordinatesXY(float x, float y );

  private:
    HardwareSerial *_grblSerial;
    File _gcodeFile;
    String _filename;
    unsigned long _filePosition;
    bool _sendingActive;
    bool _fileFinished;
    bool _gcodeLineSent;  // True when a line has been sent and we're awaiting response
    String _lastSentLine; // Stores last sent G-code for error recovery
    
    // Private file operations
    bool openFile();
    void closeFile();
    
    // Core functions
    bool sendNextLine();
    void processResponse(const String &response);
};

#endif