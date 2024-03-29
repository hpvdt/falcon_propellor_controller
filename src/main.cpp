#include <Arduino.h>

//HX711
#include <HX711.h>

//RF24
#include <RF24.h>
#include <SPI.h>


#define CE_PIN 3 //set based on chip
#define CSN_PIN 8 //set based on chip

// Let these addresses be used for the pair
uint8_t address[][6] = { "1Node", "2Node" };
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 0;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
float receivedFloat = 0.0;

int MOSI_Pin = PIN_PA1;
int MISO_Pin =PIN_PA2;
int CLOCK = PIN_PA3;
// instantiate an object for the nRF24L01 transceiver
RF24 radio(PIN_PA6, PIN_PA4);

// Make a data structure to store the entire payload of different datatypes
struct PayloadStruct {
  long readings[2];  // only using 6 characters for TX & ACK payloads
};
PayloadStruct response;

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

  // RF 24

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
  else Serial.println("Radio okay");


  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_HIGH);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(receivedFloat));  // float datatype occupies 4 bytes


  // to use ACK payloads, we need to enable dynamic payload lengths (for all nodes)
  radio.enableDynamicPayloads();  // ACK payloads are dynamically sized

  // Acknowledgement packets have no payloads by default. We need to enable
  // this feature for all nodes (TX & RX) to use ACK payloads.
  radio.enableAckPayload();

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0

  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1
  radio.startListening();  // put radio in RX mode

  digitalWrite(pinLED1, HIGH);
}

void loop() {

  //UART and HX711

  // Test on Channel A
  gain = 128;
  // scale.set_gain(gain);  

  //   Serial.println("Channel A");

  // if (scale.wait_ready_timeout(10)) {
  //   channel_A = scale.read();
  //   Serial.print("HX711 reading Channel A: ");
  //   Serial.println(channel_A);
  // } else {
  //   Serial.println("HX711 Channel A not found.");
  // }

  // delay(1.5);

  // Test on Channel B
  // gain = 32;
  // scale.set_gain(gain);  

  // if (scale.wait_ready_timeout(10)) {
  //   channel_B = scale.read();
  //   Serial.print("HX711 reading Channel B: ");
  //   Serial.println(channel_B);
  // } else {
  //   Serial.println("HX711 Channel B not found.");
  // }

 // This device is a RX node

    uint8_t pipe;
    if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
      radio.read(&receivedFloat, bytes);             // fetch payload from FIFO
      Serial.println(receivedFloat);  // print the payload's value
      //Serial.println("Hello");
      //delay(1000);

      //Recieved 

      Serial.println(response.readings[0]);    // print outgoing message
      // load the payload for the first received transmission on pipe 0
      radio.writeAckPayload(1, &response, sizeof(response));

    }
    // role
  
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
//}
