#ifndef GRBL_Control_h
#define GRBL_Control_h

#include <Arduino.h>

class GRBL_Control {
  public:
    // Constructor
    GRBL_Control(HardwareSerial &grblSerial);
    
    // Initialize communication
    void Control_begin(unsigned long baudrate = 115200);
    
    // Motion control functions
    void startJog(char axis, char dir, float speed, float distance = 0.1);
    void stopJog();
    void doHoming();
    void setMachinePosition(float x , float y , float z );
    void setMachineZ(float z );
    void setMachineXY(float x , float y );
    void moveTo(float x, float y, float z, float feedRate = 500);
    void moveToZ(float z, float feedRate = 500);
    void moveToXY(float x, float y, float feedRate = 500);

    // used to know the maximum upper limit in Z direction
    float getZAxisMachinePos();
    
    // Real-time commands
    void feedHold();   // stop the motion
    void cycleStart(); // resume the motion
    void softReset();  // erease the homing
    
    // Status checking
    bool IsMoving();
    bool isJogging() const;
    String getLastStatus() const;
    
    // State management
    void Control_run();  // Call regularly in loop()
    bool isOkReceived();
    
  private:
    HardwareSerial *_grblSerial;
    bool _joggingActive;
    char _jogAxis;
    char _jogDir;
    float _jogSpeed;
    float _jogDistance;
    unsigned long _lastJogTime;
    unsigned long _jogInterval;
    
    // Status monitoring
    String _lastStatus;
    unsigned long _lastStatusRequest;
    unsigned long _statusInterval;
    bool _isMoving;
    
    void sendJogCommand();
    void sendCommand(const String &command);
    void processResponse(const String &response);
};

#endif