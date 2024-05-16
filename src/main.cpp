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

int MOSI_PIN = PIN_PA1;
int MISO_PIN =PIN_PA2;
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
const int GAUGE_TIMEOUT = 10; // Timeout in ms for waiting for strain gauge data

long channel_A;
long channel_B;

HX711 scale;

//Servo
#include <Servo.h>

Servo servo1;
Servo servo2;
const int SERVO1_PIN = PIN_PC2;
const int SERVO2_PIN = PIN_PC3;

// LED pins
const int pinLED1 = PIN_PB0;
const int pinLED2 = PIN_PB1;
const int pinLED3 = PIN_PB5;

// Table lookup
const float CONTROL_ANGLES[]      = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
const unsigned int SERVO_ANGLES[] = {  15,  17,  19,  21,  23,  25,  27,  29,  31,  33};
const int NUMBER_OF_ANGLES = sizeof(CONTROL_ANGLES) / sizeof(CONTROL_ANGLES[0]);

unsigned int getServoAngle(float desiredControlAngle) {
  // Ensure it's valid and clamped
  if (isnan(desiredControlAngle)) desiredControlAngle = 0;
  if (desiredControlAngle <= CONTROL_ANGLES[0]) return SERVO_ANGLES[0];
  if (desiredControlAngle >= CONTROL_ANGLES[NUMBER_OF_ANGLES - 1]) return SERVO_ANGLES[NUMBER_OF_ANGLES - 1];

  unsigned int interpolated = 90; // Value to interpolate from lookup table, defaults to mid-range
  
  // Interpolate
  for (int i = 1; i < (NUMBER_OF_ANGLES - 1); i++) {
    if (CONTROL_ANGLES[i] > desiredControlAngle) {
      // Interpolate in this bracket
      const float dx = CONTROL_ANGLES[i] - CONTROL_ANGLES[i - 1];
      const float dy = SERVO_ANGLES[i] - SERVO_ANGLES[i - 1];
      const float portion = (desiredControlAngle - CONTROL_ANGLES[i - 1]) / dx;

      interpolated = SERVO_ANGLES[i - 1] + (dy * portion);
      break;
    }
  }

  return interpolated;
}

void setup() {

  //UART and HX711
  Serial.begin(9600);
  delay(1000);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  //LED tests
  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);
  pinMode(pinLED3, OUTPUT);

  //Do you like flashing LEDs? Then UNCOMMENT THIS LED TEST!
  digitalWrite(pinLED1, HIGH);
  delay(20);
  digitalWrite(pinLED2, HIGH);
  delay(20);
  digitalWrite(pinLED3, HIGH);
  delay(20);
  digitalWrite(pinLED1, LOW);
  delay(20);
  digitalWrite(pinLED2, LOW);
  delay(20);
  digitalWrite(pinLED3, LOW);
  delay(20);

  //Servo
  servo1.attach(SERVO1_PIN); 
  servo2.attach(SERVO2_PIN); 

  // RF 24

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
  else Serial.println("Radio okay");


  radio.setPALevel(RF24_PA_HIGH);  // RF24_PA_MAX is default.

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

  radio.writeAckPayload(1, &response, sizeof(response)); // Initial dummy response to serve

  digitalWrite(pinLED1, HIGH);
}

void loop() {

  // UART and HX711

  scale.set_gain(32); // Set HX711 to read channel B after this read
  Serial.println("Channel A");
  if (scale.wait_ready_timeout(GAUGE_TIMEOUT)) {
    channel_A = scale.read();
    Serial.print("HX711 reading Channel A: ");
    Serial.println(channel_A);
  } else {
    Serial.println("HX711 Channel A not found.");
  }
  // Read on Channel B
  scale.set_gain(128); // Set HX711 to read channel A after this read
  if (scale.wait_ready_timeout(GAUGE_TIMEOUT)) {
    channel_B = scale.read();
    Serial.print("HX711 reading Channel B: ");
    Serial.println(channel_B);
  } else {
    Serial.println("HX711 Channel B not found.");
  }

  // This device is a RX node
  // Set ack payload response to channel A and channel B readings
 
  response.readings[0] = channel_A;
  response.readings[1] = channel_B;

  uint8_t pipe;
  if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
    radio.read(&receivedFloat, bytes);             // fetch payload from FIFO
    Serial.println(receivedFloat);  // print the payload's value

    Serial.println(response.readings[0]);    // print outgoing message
    // load the payload for the first received transmission on pipe 0
    radio.writeAckPayload(1, &response, sizeof(response));

    // Update servos with newest angle
    unsigned int angle = getServoAngle(receivedFloat);
    servo1.write(angle);
    servo2.write(angle);
  }
}
