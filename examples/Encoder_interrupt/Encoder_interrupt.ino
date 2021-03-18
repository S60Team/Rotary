// Example using the Rotary library, dumping integers to the serial
// port. The integers increment or decrement depending on the direction
// of rotation.
// This example uses interrupts rather than polling.
#include <Rotary2.h>
// Rotary encoder is wired with the common to ground and the two
// outputs to pins 2 and 3.
Rotary2 rotary = Rotary2(2, 3);
// Counter that will be incremented or decremented by rotation.
int counter = 0;
void setup() {
  Serial.begin(115200);
  // Interrupt pins:
  //  0, 1, 2, 3 (2,3 is SDA and SCL, not recommend to use) on Leonardo or Pro Micro;
  //  2, 3 on Uno or Nano, Mega etc.
  attachInterrupt(digitalPinToInterrupt(2),rotate,CHANGE);
  attachInterrupt(digitalPinToInterrupt(3),rotate,CHANGE);
}
void loop() {
}
// rotate is called anytime the rotary inputs change state.
void rotate() {
  unsigned char result = rotary.process();
  if (result == DIR_CW) {
    counter++;
    Serial.println(counter);
  } else if (result == DIR_CCW) {
    counter--;
    Serial.println(counter);
  }
}
