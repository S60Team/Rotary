/*
Now you can define setting in the Rotary2.h file:
This is ONLY shows settings, not sets them!
#define DWF // Enable Faster Digital Write.
#define ENABLE_PULLUPS // Enable internal pull-up resistor.
#define ENCODER2 // Enable other type of encoder.
#define HALF_STEP // Enable Half-step encoder type.
*/
//
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
