//--------------------------------------------------------------
//-- Oscillator.pde
//-- Generate sinusoidal oscillations in the servos
//--------------------------------------------------------------
//-- (c) Juan Gonzalez-Gomez (Obijuan), Dec 2011
//-- GPL license
//--------------------------------------------------------------
#ifndef Oscillator_h
#define Oscillator_h

#ifdef ARDUINO_ARCH_ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif

//-- Macro for converting from degrees to radians
#ifndef DEG2RAD
  #define DEG2RAD(g) ((g)*M_PI)/180
#endif

class Oscillator
{
  public:
    Oscillator(int trim=0) {_trim=trim;};
    void attach(int pin, bool rev =false);
    void detach();
    
    void SetA(unsigned int amplitude) {_amplitude=amplitude;};
    void SetO(int offset) {_offset=offset;};
    void SetPh(double Ph) {_phase0=Ph;};
    void SetT(unsigned int period);
    void SetTrim(int trim){_trim=trim;};
    int getTrim() {return _trim;};
    void SetPosition(int position); 
    void Stop() {_stop=true;};
    void Play() {_stop=false;};
    void Reset() {_phase=0;};
    void refresh();
    
  private:
    bool next_sample();  
    
  private:
    //-- Servo that is attached to the oscillator
    Servo _servo;
    
    //-- Oscillators parameters
    unsigned int _amplitude;  //-- Amplitude (degrees)
    int _offset;  //-- Offset (degrees)
    unsigned int _period;  //-- Period (miliseconds)
    double _phase0;   //-- Phase (radians)  
    
    //-- Internal variables
    int _pos;         //-- Current servo pos
    int _trim;        //-- Calibration offset
    double _phase;    //-- Current phase
    double _inc;      //-- Increment of phase
    double _numberSamples;        //-- Number of samples
    unsigned int _samplingPeriod; //-- sampling period (ms)
    
    long _previousMillis; 
    long _currentMillis;
    
    //-- Oscillation mode. If true, the servo is stopped
    bool _stop;

    //-- Reverse mode
    bool _rev;
};

#endif