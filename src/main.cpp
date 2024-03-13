#include <Arduino.h>

//HX711
#include <HX711.h>

//RF24
#include <RF24.h>


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = PIN_PC0;
const int LOADCELL_SCK_PIN = PIN_PC1;
long channel_A;
long channel_B;

HX711 scale;

byte gain;

//Servo
#include <Servo.h>

Servo servo1;
Servo servo2;
int pos1 = 0;    // variable to store the servo position
int pos2 = 0;


//LED test
const int pinLED1 = PIN_PB0;
const int pinLED2 = PIN_PB1;
const int pinLED3 = PIN_PB5;

void setup() {

  //UART and HX711
  Serial.begin(9600);

 

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  //LED tests
  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);
  pinMode(pinLED3, OUTPUT);

  //Servo
  servo1.attach(PIN_PC2);  // Servo 2 
  servo2.attach(PIN_PC3); // Servo 3 
}

void loop() {

  //UART and HX711

  // Test on Channel A
  gain = 128;
  scale.set_gain(gain);  

    Serial.println("Channel A");

  if (scale.wait_ready_timeout(10)) {
    channel_A = scale.read();
    Serial.print("HX711 reading Channel A: ");
    Serial.println(channel_A);
  } else {
    Serial.println("HX711 Channel A not found.");
  }

  delay(1.5);

  // Test on Channel B
  gain = 32;
  scale.set_gain(gain);  

  if (scale.wait_ready_timeout(10)) {
    channel_B = scale.read();
    Serial.print("HX711 reading Channel B: ");
    Serial.println(channel_B);
  } else {
    Serial.println("HX711 Channel B not found.");
  }


  //LED Tests
 // digitalWrite(pinLED1, HIGH);
 // delay(20);
 // digitalWrite(pinLED2, HIGH);
 // delay(20);
 // digitalWrite(pinLED3, HIGH);
 // delay(20);
 // digitalWrite(pinLED1, LOW);
 // delay(20);
 // digitalWrite(pinLED2, LOW);
 // delay(20);
  //digitalWrite(pinLED3, LOW);
 // delay(20);

  // Servo
//   for (pos1 = 0; pos1 <= 180; pos1 += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     servo1.write(pos1);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15ms for the servo to reach the position
//   }
//   for (pos1 = 180; pos1 >= 0; pos1 -= 1) { // goes from 180 degrees to 0 degrees
//     servo1.write(pos1);              // tell servo to go to position in variable 'pos'
//     delay(15);                            // waits for the servo to get there
//   }

//   for (pos2 = 0; pos2 <= 180; pos2 += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     servo2.write(pos2);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15ms for the servo to reach the position
//   }
//   for (pos2 = 180; pos2 >= 0; pos2 -= 1) { // goes from 180 degrees to 0 degrees
//     servo2.write(pos2);              // tell servo to go to position in variable 'pos'
//    delay(15);                            // waits for the servo to get there
//  }
}
