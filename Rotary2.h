// Rotary encoder library for Arduino.
#ifndef rotary2_h
#define rotary2_h
//
#include "Arduino.h"
//
// Enable this to emit codes twice per step.
//#define HALF_STEP
//
// If you want to Enable internal pullup resistors (useful for KY-040 Encoder):
//#define ENABLE_PULLUPS
//
// Uncomment If your Encoder does not work:
//#define ENCODER2
//
// For testing purpose. Do not use
//#define ENCTEST
//
// Define for Enable Faster Read and Write:
#define DWF
//
// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20
//
class Rotary2 {
  public:
    Rotary2(char, char);
    // Process pin(s)
    unsigned char process();
  private:
    unsigned char state;
    unsigned char pin1;
    unsigned char pin2;
};
#endif
