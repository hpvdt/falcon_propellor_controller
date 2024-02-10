#include <Arduino.h>

const int pinLED1 = PIN_PB0;
const int pinLED2 = PIN_PB1;
const int pinLED3 = PIN_PB5;

void setup() {
  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);
  pinMode(pinLED3, OUTPUT);
}

void loop() {
  digitalWrite(pinLED1, HIGH);
  delay(200);
  digitalWrite(pinLED2, HIGH);
  delay(200);
  digitalWrite(pinLED3, HIGH);
  delay(200);
  digitalWrite(pinLED1, LOW);
  delay(200);
  digitalWrite(pinLED2, LOW);
  delay(200);
  digitalWrite(pinLED3, LOW);
  delay(200);

  // Servo
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                            // waits for the servo to get there
  }
}
