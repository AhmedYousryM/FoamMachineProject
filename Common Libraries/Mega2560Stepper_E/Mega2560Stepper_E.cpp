#include "Mega2560Stepper_E.h"

// This code controls atepper motors by generating pulse width modulation output using 16bit-timers in Arduino Mega 2560, Timer for each stepper motor
// The code uses FAST PWM Mode defined in the Atmega_2560 DataSheet
// The timer register TCNTn counts from 0 until it reaches the TOP value (OCRA) , to reset the counter TCNTn and start again from zero
// The output pin is OCnB:          OCnB is HIGH when ( TCNTn = OCRA )         ,         OCnB is LOW when ( TCNTn = OCRB )          
//                                  ________                                                                                                   ________
//                                 |        |                                                                                                 |        |
//                                 |        |                                                                                                 |        |
//                                 |        |                                                                                                 |        |
//_________________________________          __________________________________________________________________________________________________         ______________________________________________
//                                OCRA     OCRB                                                                                               OCRA    OCRB



#define SetPrescalar( timer , PRESCALAR ) \
( TCCR##timer##B = (TCCR##timer##B & 0b11111000) | PRESCALAR )
#define StopTimer(timer)  ( TCCR##timer##B = (TCCR##timer##B & 0b11111000) )
#define StartTimer(timer)  ( SetPrescalar( timer , presc##timer ) )
#define ZeroPrescalar(timer)  ( presc##timer = 0 )
#define CheckZeroPrescaler(timer) \
 ( ( bitRead(TCCR##timer##B,CS##timer##0) || bitRead(TCCR##timer##B,CS##timer##1) || bitRead(TCCR##timer##B,CS##timer##2) )==0 )


#define SetOCR( timer , A , ocr )  ( OCR##timer##A = ocr )
#define ActivateTimerINT( timer , A )     bitSet(TIMSK##timer , OCIE##timer##A) 
#define DeactivateTimerINT( timer , A )   bitClear(TIMSK##timer , OCIE##timer##A) 
#define ActivateTimerNextINT( timer , B )   \
bitSet( TIFR##timer , OCF##timer##B );     \
bitSet( TIMSK##timer , OCIE##timer##B );   


#define CalculateDirection()    ( _direction = ( (_targetPos-_currentPos) > 0 )  )


const int Clock_prescaler[] = { 0, 1, 8, 64, 256, 1024};
const uint32_t F_CPU_over_Clock_prescaler[] = { 0, F_CPU, F_CPU/8, F_CPU/64, F_CPU/256, F_CPU/1024};
const float micros20_for_each_clock[] = { 0, 20*16, 20*16/8, 20*16/64, 1.0, 1.0};
const uint8_t  StepPin[] = {255, 12, 255, 2, 7, 45};         
// Timer variables
volatile uint16_t step1, step3, step4, step5;
volatile uint16_t endStep1, endStep3, endStep4, endStep5;
volatile uint16_t ocrA1, ocrA3, ocrA4, ocrA5;
volatile unsigned int ocrB1, ocrB3, ocrB4, ocrB5;
volatile unsigned int presc1, presc3, presc4, presc5; 
float *_speed1, *_speed3, *_speed4, *_speed5;
volatile Mega2560Stepper::ModesOfOperation *myMode1,*myMode3 ,*myMode4 ,*myMode5 ;
// Timer Interrupts
#define StepCounterInt(timer) \
    ISR(TIMER##timer##_COMPA_vect){\
      step##timer++;\
    }
        StepCounterInt(1)
        StepCounterInt(3)
        StepCounterInt(4)
        StepCounterInt(5)

#define ChangePulseSpecs( timer ) \
if(CheckZeroPrescaler(timer)){            /* if (timer is stopped) */ \ 
          digitalWrite(13,1);                   \
presc##timer = prescalarbits;                                         \
ocrA##timer = ocr;                                                    \
ocrB##timer = ocr/2;/*_minPulseWidth * micros_for_each_clock[ prescalarbits ];*/\
ApplyPulseSpecs(timer);            /*apply this speed now*/            \   
}else{                         /*No speed at all and no pulses*/      \
presc##timer = prescalarbits;                                         \
ocrA##timer = ocr;                                                    \
ocrB##timer = ocr/2;/*_minPulseWidth * micros_for_each_clock[ prescalarbits ];*/\
ActivateTimerNextINT( timer , B ); /* wait fot the next pulse end*/   \ 
}

#define ApplyPulseSpecs(timer) \
      StopTimer(timer);   \
      /*TCNT##timer += micros20_for_each_clock[ presc##timer ];*/\
      SetOCR( timer , A , ocrA##timer );\
      SetOCR( timer , B , ocrB##timer );\
      StartTimer(timer)    ;  // start with the new prescaler

#define motionCahngeINT(timer)           \
    ISR(TIMER##timer##_COMPB_vect){      \
      switch (*myMode##timer){                   \
        case Mega2560Stepper::accelerating :              \
        case Mega2560Stepper::Stopping :                  \
        case Mega2560Stepper::motionCut :                 \
          digitalWrite(13,0);                  \
          ApplyPulseSpecs(timer);         \
          DeactivateTimerINT( timer , B ); \
        break;                           \
        case Mega2560Stepper::approachingTarget1stTime:        \
          ApplyPulseSpecs(timer);           \
          *myMode##timer = Mega2560Stepper::approachingTarget ;     \
        case Mega2560Stepper::approachingTarget :            \
          if(step##timer>=endStep##timer){   \
            StopTimer(timer) ;           \
            *myMode##timer = Mega2560Stepper::atTarget1stTime ; \
          }                                  \
        break;                               \
        case Mega2560Stepper::stopped1stTime :                \
          StopTimer(timer) ;                 \
          *myMode##timer = Mega2560Stepper::stopped2ndTime ;          \
        break;                               \
      }                                      \
    }
       motionCahngeINT(1)
       motionCahngeINT(3)
       motionCahngeINT(4)
       motionCahngeINT(5)

#define StopNow( timer )              \   
myMode = stopped1stTime ;             \        
ActivateTimerNextINT( timer , B ); /* wait for the pulse end  */ 


#define afterStop( timer )       \
UpdateStep();                   \
presc##timer = 0;                \
DeactivateTimerINT( timer , B ); \
_speed = 0;            

#define startApproaching( timer )    \
myMode = approachingTarget1stTime ;   \
UpdateStep();                          \
ActivateTimerNextINT( timer , B );      /* wait for the pulse end  */ 



// Important code lines
#define APPLY( code )            \
      switch (_timer){           \
                case 1:          \
                    code(1);     \
                break;           \
              case 3:            \
                    code(3);     \
                break;           \
              case 4:            \
                    code(4);     \
                break;           \
              case 5:            \
                    code(5);     \
                break;           \
      }


                                
        Mega2560Stepper::Mega2560Stepper(){}

#define Initiate_REG( timer )      \
TCCR##timer##A = bit(COM##timer##B1) | bit(WGM##timer##1) | bit(WGM##timer##0) ;         \
TCCR##timer##B = bit(WGM##timer##3)  | bit(WGM##timer##2) ;    \
TIMSK##timer = 0;                \
bitSet( TCCR##timer##C ,  FOC##timer##B );  \
ActivateTimerINT( timer , A );          \
pinMode(StepPin[timer],OUTPUT);   \
_speed##timer = &_speed;       \
myMode##timer = &myMode ;         
       
        void Mega2560Stepper::Begin(uint8_t timer , uint8_t pin1 , uint8_t pin2){

            _timer = timer;
            _DirPin = pin1;
            _EnablePin = pin2;
            
            // set direction pin as output
            pinMode(_DirPin,OUTPUT);
            
            // set denable pin as output
            pinMode(_EnablePin,OUTPUT);

            // set timer pin and registers
            APPLY( Initiate_REG )

        }



        void    Mega2560Stepper::moveTo(long absolute){
            if (_targetPos != absolute){
              _targetPos = absolute;
            }
            run();
        }

        void    Mega2560Stepper::move(long relative){
            moveTo(_currentPos + relative);
        }

        bool Mega2560Stepper::run(){
         /* Serial.print(_currentPos);
          Serial.print("   ");
          Serial.print(step1);
          Serial.print("   ");
          Serial.println(endStep1);*/
          bool motionEnd = reachTarget() ;

          switch (myMode){
            case accelerating1stTime :
              if (!motionEnd){
                CalculateDirection() ;
                enableOutputs() ;
                myMode = accelerating ;
              }
              else{
                myMode = atTarget1stTime ;
              }
            break;                  
            case accelerating :              
                if ( FutureUpdate() )
                computeNewSpeed();
            break;
            case Stopping :
              myMode = accelerating ;
            break;
            case atTarget1stTime :
              APPLY( afterStop )
              disableOutputs() ;
              myMode = atTarget ;
            break;
            case atTarget :
            case stopped2ndTime :
            case stopped :
              if (!motionEnd) {myMode = accelerating1stTime;} 
            break;
          }
          return !motionEnd ;
        }

        void    Mega2560Stepper::setMaxSpeed(long speed){
            if (speed < 0)
               speed = -speed;
            _maxSpeed=speed;

        }

        void    Mega2560Stepper::setAcceleration(int acceleration){
            if (_acceleration_per_s_ms == acceleration/1000.0){}
            else{
              if (acceleration < 0)
                acceleration = -acceleration;
                _acceleration_per_s_ms = acceleration/1000.0;
                compute_dec_const();
            }
        }

        float   Mega2560Stepper::speed(){
            return _speed;
        }

        long    Mega2560Stepper::distanceToGo(){
            return _targetPos - _currentPos;
        }

        long    Mega2560Stepper::targetPosition(){
            return _targetPos;
        }

        void    Mega2560Stepper::setCurrentPosition(long position){
            _targetPos = _currentPos = position;
        }

 
     
        void Mega2560Stepper::StoppingDo(){
            float speedChange = _acceleration_per_s_ms * DeltaTime;
            if ( _speed < speedChange ){
              //Serial.println("stop now");
              APPLY( StopNow )
            }else{
              //Serial.println("decelerate");
              _speed -= speedChange;
              ExecuteSpeed();
            }
        }

        void Mega2560Stepper::stop(){
          switch (myMode){
            case accelerating1stTime :
            case accelerating :                           
            case approachingTarget1stTime :
            case approachingTarget :
              myMode = Stopping ;
            case Stopping :
              if (FutureUpdate())    
                StoppingDo();
            break;
            case stopped2ndTime :
              UpdateStep();
              APPLY( afterStop )
              disableOutputs();
              myMode = stopped ;
            break;
            case stopped :
              // do nothing 
            break;
        }
            
        }



        void    Mega2560Stepper::disableOutputs(){
            digitalWrite(_EnablePin,HIGH);
            enabled = 0;
        }

        void    Mega2560Stepper::enableOutputs(){
            digitalWrite(_EnablePin,LOW);
            enabled = 1;
        }


        bool    Mega2560Stepper::reachTarget(){
            UpdateStep();
            return (_speed == 0.0 && _targetPos == _currentPos);
        }

#define SafetyFactor 4   // this factor should get higher as the run() function called less frequent
                             // to decrease the probability of missing the target point to stop at.


        void Mega2560Stepper::compute_dec_const(){
          C1 = _acceleration_per_s_ms * MinDeltaTime / 2 ; //a * dt /2
          C2 = 2 / (C1*C1) * _acceleration_per_s_ms * 1000;
          C3 = _acceleration_per_s_ms * MaxDeltaTime ;
          C4 = max( _acceleration_per_s_ms * MaxDeltaTime * MaxDeltaTime /1000 /2 * SafetyFactor , 4 );
        }

        void    Mega2560Stepper::computeNewSpeed(){
          long distanceTo = distanceToGo();
          if ( _direction == ( distanceTo > 0 ) ){ // right moving direction
          distanceTo = abs(distanceTo);

            long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration_per_s_ms*1000));
            if ( stepsToStop > distanceTo ){
              _speed = C1 * (-1+sqrt(1+C2*distanceTo)) ;
              ExecuteSpeed();
              if ( distanceTo<C4  ){               
                APPLY( startApproaching ) ;
              }             
              
            }else if( stepsToStop > distanceTo + C4 ){
              // do nothing
              if ( distanceTo<C4  ){
                APPLY( startApproaching ) ; 
              }   
            }else if(_speed<_maxSpeed){  // the target point is far and the speed can get faster
              // accelerate              
              float speedChange = _acceleration_per_s_ms * DeltaTime;
              _speed += speedChange ;
              
              if ( _speed > _maxSpeed )
                _speed = _maxSpeed ;

              ExecuteSpeed();  
            }else if(_speed>_maxSpeed){
              _speed -= _acceleration_per_s_ms * DeltaTime;
            }
          
                      
          }else{  // rotate in the oppisite direction
              //Serial.println("stop()");
              StoppingDo();
              
          }

        }





        bool Mega2560Stepper::ExecuteSpeed(){
          uint8_t prescalarbits;
          if (_speed==0){
              prescalarbits = 0b000;
              APPLY( StartTimer );
              APPLY( ZeroPrescalar );
          }else{
              prescalarbits = 0b001;
              uint32_t ocr ;
              for(int i = 1 ; i<=5 ; i++){
                ocr = F_CPU_over_Clock_prescaler[ i ] / _speed   - 1 ;
                if (ocr > 0xffff){
                  prescalarbits+=1;
                  continue ;
                }else{
                  break ;
                }
              }
          APPLY( ChangePulseSpecs );
          }                    
        }
        


        void Mega2560Stepper::InvertDirectionPin(){
          digitalWrite( _DirPin , digitalRead(_DirPin)^1 );
        }


        bool Mega2560Stepper::FutureUpdate(){
          uint32_t CurrentTime = millis();
          DeltaTime = CurrentTime - Prev_Update_Time;
          if ( DeltaTime > MinDeltaTime ){
            AveragedDeltaTime = AveragedDeltaTime*0.9 + DeltaTime*0.1;
            if(DeltaTime>MaxDeltaTime)  (DeltaTime=MaxDeltaTime);
            Prev_Update_Time = CurrentTime;
            return 1;
          }else{
            return 0;
          }
        }



 





#define UPDATESTEP( timer )         \
  switch (myMode){                      \
    case approachingTarget1stTime:         \
      DeactivateTimerINT( timer , A );      \
      if(_direction)                         \
        _currentPos += step##timer;          \
      else                                   \
        _currentPos -= step##timer;          \
      step##timer = 0;                       \
      endStep##timer = abs( distanceToGo() );\
      ActivateTimerINT( timer , A );         \
      ActivateTimerNextINT( timer , B );     \
    break;                                   \
    case approachingTarget :                 \
      DeactivateTimerINT( timer , A );       \
      if(_direction)                         \
        _currentPos += step##timer;          \
      else                                   \
        _currentPos -= step##timer;          \
      endStep##timer -= step##timer;         \
      step##timer = 0;                       \
      ActivateTimerINT( timer , A );         \
    break;                                   \
    default:                                 \
      DeactivateTimerINT( timer , A );       \
      if(_direction)                         \
        _currentPos += step##timer;          \
      else                                   \
        _currentPos -= step##timer;          \
      step##timer = 0;                       \
      ActivateTimerINT( timer , A );         \              
    break;                                   \
  }

  


        void Mega2560Stepper::UpdateStep(){
          APPLY( UPDATESTEP );
        }

        long    Mega2560Stepper::currentPosition(){
          return _currentPos;
        }


                                          \

        void    Mega2560Stepper::CutMotion(){
          APPLY( StopTimer );
          _speed = 0;
        }

        void Mega2560Stepper::SetMinMaxDeltaTime(unsigned int N1 , unsigned int N2){
          MinDeltaTime = (N1<N2? N1:N2) ;
          MaxDeltaTime = (N1<N2? N2:N1) ;
        }