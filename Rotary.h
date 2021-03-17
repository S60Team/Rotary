/*
 * Rotary encoder library for Arduino.
 */

#ifndef rotary_h
#define rotary_h

#include "Arduino.h"

// Enable this to emit codes twice per step.
//#define HALF_STEP

// Enable weak pullups
//#define ENABLE_PULLUPS

// Uncomment If your Encoder does not work:
//#define ENCODER2

// If you want to Enable internal pullup resistors (useful for KY-040 Encoder):
//#define ENABLE_PULLUPS

// For testing. Dont enable this.
//#define ENCTEST

// Define for Enable Faster Read and Write:
#define DWF

// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

class Rotary
{
  public:
    Rotary(char, char);
    // Process pin(s)
    unsigned char process();
  private:
    unsigned char state;
    unsigned char pin1;
    unsigned char pin2;
};

#endif
 
