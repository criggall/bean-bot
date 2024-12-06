// Import libraries:
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Create RF24 object:
RF24 radio(12,11); // CE, CSN pins

// Assign address for module communication:
const byte address[6] = "00001";

// Create structure to contain joystick readings:
struct Data_Package{
  byte X1;
  byte Y1;
  byte X2;
  byte Y2;
};

// Create variable with this structure:
struct Data_Package data;
void setup() {
  Serial.begin(9600);
  radio.begin();
  // Set the address:
  radio.openWritingPipe(address);
  // Set module as transmitter:
  radio.stopListening();
  // Set default values:
  // Middle value (509 analog -> 127 digital) at resting position
  data.X1 = 127;
  data.Y1 = 127;
  data.X2 = 127;
  data.Y2 = 127;
}

void loop() {

  // Read joystick (analog) inputs and map to one Byte value:
  data.X1 = map(analogRead(A0),0,1023,0,255);
  data.Y1 = map(analogRead(A1),0,1023,0,255);
  data.X2 = map(analogRead(A2),0,1023,0,255);
  data.Y2 = map(analogRead(A3),0,1023,0,255);

  //Serial.print(" X1: ");
  //Serial.print(data.X1);
  //Serial.print(" Y1: ");
  //Serial.print(data.Y1);
  //Serial.print(" X2: ");
  //Serial.print(data.X2);
  //Serial.print(" Y2: ");
  //Serial.print(data.Y2);

  // Send message to receiver:
  radio.write(&data, sizeof(Data_Package));
  
  // Wait 0.5 seconds:
  delay(500);
}