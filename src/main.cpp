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
}
