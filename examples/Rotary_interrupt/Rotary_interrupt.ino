// Uncomment for additional settings:
// Enable DigitalWriteFast:
#define DWF

// If you want to Enable internal pullup resistors (useful for KY-040 Encoder):
//#define ENABLE_PULLUPS

// Try this if your Encoder does not work:
// Change type of Encoding table:
//#define ENCODER2

// Enable this to emit codes twice per step:
//#define HALF_STEP

#include <Rotary2.h> 
Rotary2 enc = Rotary2(0, 1);
volatile int16_t counter = 0;
void setup() {
  Serial.begin(115200);
  // Interrupt pins:
  //  0, 1, 2, 3 (2,3 is SDA and SCL, not recommend to use) on Leonardo or Pro Micro;
  //  2, 3 on Uno or Nano, Mega etc.
  attachInterrupt(digitalPinToInterrupt(0),rotate,CHANGE);
  attachInterrupt(digitalPinToInterrupt(1),rotate,CHANGE);
  Serial.println(counter);
}
void loop() {
}
// rotate() is called anytime the rotary inputs change state.
void rotate() {
  unsigned char result = enc.process();
  if (result == DIR_CW){counter++;}
  else if (result == DIR_CCW){counter--;}
}
