#include <Rotary.h> // Rotary encoder on Pin 2 and 3.
Rotary rotary = Rotary(0, 1);
volatile int16_t counter = 0;
void setup() {
  Serial.begin(115200);
  //attachInterrupt(0, rotate, CHANGE);
  //attachInterrupt(1, rotate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(0),rotate,CHANGE);
  attachInterrupt(digitalPinToInterrupt(1),rotate,CHANGE);
}
void loop() {
  Serial.println(counter);
  delay(10);
}
// rotate() is called anytime the rotary inputs change state.
void rotate() {
  unsigned char result = rotary.process();
  if (result == DIR_CW){counter++;} //Serial.println(counter);
  else if (result == DIR_CCW){counter--;} //Serial.println(counter);
}
