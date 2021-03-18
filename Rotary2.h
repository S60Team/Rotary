/*
 * Rotary encoder library for Arduino.
 */

#ifndef rotary_h
#define rotary_h

#include "Arduino.h"

// Enable this to emit codes twice per step.
//#define HALF_STEP

// If you want to Enable internal pullup resistors (useful for KY-040 Encoder):
//#define ENABLE_PULLUPS

// Uncomment If your Encoder does not work:
//#define ENCODER2

// For testing purpose. Do not use
#if defined(ENCTEST)
  //#error "test"
  //#pragma message "Encoder test:"
#endif

// Define for Enable Faster Read and Write:
#if defined(DWF)
  #include "DigitalWriteFast.h"
#endif

// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

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

/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */
#define R_START 0x0
//
#if defined(HALF_STEP)
  // Use the half-step state table (emits a code at 00 and 11)
  #define R_CCW_BEGIN 0x1
  #define R_CW_BEGIN 0x2
  #define R_START_M 0x3
  #define R_CW_BEGIN_M 0x4
  #define R_CCW_BEGIN_M 0x5
  #if defined(ENCODER2)
    // Modified 5/04/2019 by Carlos Siles
    // Modified table to follow sequence 00>10>11>01>01
    const unsigned char ttable[6][4] = {
      // R_START (00)
      {R_START,           R_CCW_BEGIN,  R_CW_BEGIN,    R_START_M},
      // R_CCW_BEGIN
      {R_START,           R_CCW_BEGIN,  R_START,       R_START_M | DIR_CCW},
      // R_CW_BEGIN
      { R_START,          R_START,      R_CW_BEGIN ,   R_START_M | DIR_CW},
      // R_START_M (11)
      {R_START,           R_CW_BEGIN_M, R_CCW_BEGIN_M, R_START_M},
      // R_CW_BEGIN_M
      {R_START | DIR_CW,  R_CW_BEGIN_M, R_START_M,     R_START_M},
      // R_CCW_BEGIN_M
      {R_START | DIR_CCW, R_START_M,    R_CCW_BEGIN_M, R_START_M},
    };
  #else
    const unsigned char ttable[6][4] = {
      // R_START (00)
      {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
      // R_CCW_BEGIN
      {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
      // R_CW_BEGIN
      {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
      // R_START_M (11)
      {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
      // R_CW_BEGIN_M
      {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
      // R_CCW_BEGIN_M
      {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
    };
  #endif
#else
  // Use the full-step state table (emits a code at 00 only)
  #define R_CW_FINAL 0x1
  #define R_CW_BEGIN 0x2
  #define R_CW_NEXT 0x3
  #define R_CCW_BEGIN 0x4
  #define R_CCW_FINAL 0x5
  #define R_CCW_NEXT 0x6
  //
  #if defined(ENCODER2)
    // Modified 5/04/2019 by Carlos Siles
    const unsigned char ttable[7][4] = {
    // R_START
    {R_START,           R_CCW_BEGIN, R_CW_BEGIN,   R_START},
      // R_CW_FINAL
    {R_START | DIR_CW,  R_CW_FINAL,  R_START,      R_CW_NEXT},
      // R_CW_BEGIN
    {R_START,           R_START,     R_CW_BEGIN,   R_CW_NEXT},
      // R_CW_NEXT
    {R_START,           R_CW_FINAL,  R_CW_BEGIN,   R_CW_NEXT},
      // R_CCW_BEGIN
    {R_START,           R_CCW_BEGIN, R_START,      R_CCW_NEXT},
      // R_CCW_FINAL
    {R_START | DIR_CCW, R_START,     R_CCW_FINAL,  R_CCW_NEXT},
      // R_CCW_NEXT
    {R_START,           R_CCW_BEGIN, R_CCW_FINAL,  R_CCW_NEXT},
    };
  #else
    const unsigned char ttable[7][4] = {
      // R_START
      {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
      // R_CW_FINAL
      {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
      // R_CW_BEGIN
      {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
      // R_CW_NEXT
      {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
      // R_CCW_BEGIN
      {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
      // R_CCW_FINAL
      {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
      // R_CCW_NEXT
      {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
    };
  #endif
#endif
// Constructor. Each arg is the pin number for each encoder contact.
Rotary2::Rotary2(char _pin1, char _pin2) {
  // Assign variables.
  pin1 = _pin1;
  pin2 = _pin2;
  // Set pins to input.
  #if defined(DWF)
    pinModeFast(pin1, INPUT);
    pinModeFast(pin2, INPUT);
    #if defined(ENABLE_PULLUPS)
      digitalWriteFast(pin1, HIGH);
      digitalWriteFast(pin2, HIGH);
    #endif
  #else
    pinMode(pin1, INPUT);
    pinMode(pin2, INPUT);
    #if defined(ENABLE_PULLUPS)
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, HIGH);
    #endif
  #endif
  // Initialise state.
  state = R_START;
}
//
unsigned char Rotary2::process() {
  // Grab state of input pins.
  #if defined(DWF)
    unsigned char pinstate = (digitalReadFast(pin2) << 1) | digitalReadFast(pin1);
  #else
    unsigned char pinstate = (digitalRead(pin2) << 1) | digitalRead(pin1);
  #endif
  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];
  // Return emit bits, ie the generated event.
  #if defined(ENCTEST)
    /*int direction = state & 0x30;
    if (direction != 0){
      if (direction == DIR_CW){
        int x = (1 * state);
        return x;
      }
      else if (direction == DIR_CCW){
        int x = (-1 * state);
        return x;
      }
      else{return 0;}
    }*/
    //#pragma message "Process test"
  #else
    return state & 0x30;
  #endif
}
