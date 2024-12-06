/* Motor configuration:
 * 
 *    FRONT
 * 
 * (1)      (2)
 * 
 *     BACK
 * 
 */

// Import libraries:
#include <AFMotor.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Temporary way to switch between automatic and manual modes:
const int mode = 1; // 0 -> automatic, 1 -> manual

// Create RF24 object:
RF24 radio(12,11); // CE, CSN pins

// Assign address for module communication:
const byte address[6] = "00001";

AF_DCMotor motor1(1);
AF_DCMotor motor2(4);

const int trigPin = A1;
const int echoPin = A2;

long duration;

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
  
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  
  Serial.begin(9600);
  
  motor1.setSpeed(200);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(RELEASE);

  radio.begin();

  // Set the address:
  radio.openReadingPipe(0, address);

  // Set the module as receiver:
  radio.startListening();
  
}

void loop() {

  // MANUAL MODE:
  if (mode == 1)
  {
    
    // Read RC data:
    if (radio.available())
    {
      radio.read(&data, sizeof(Data_Package));
    }

    // Set to defaults if no radio connection:
    else{
      data.X1 = 127;
      data.Y1 = 127;
      data.X2 = 127;
      data.Y2 = 127;
    }

    Serial.print(" X1: ");
    Serial.print(data.X1);
    Serial.print(" Y1: ");
    Serial.print(data.Y1);
    Serial.print(" X2: ");
    Serial.print(data.X2);
    Serial.print(" Y2: ");
    Serial.print(data.Y2);

    // Define speed values:
    // Max speed 127 for now -- map to 0 to 255 later if not too fast
    int M1speed = data.Y1 - 127;
    int M2speed = data.Y2 - 127;

    // Joystick 1 Y values --> Motor 1
    // Joystick 2 Y values --> Motor 2
    
    // Motor 1 controls:
    if (data.Y1 < 0) // backward
    {
      motor1.run(BACKWARD);
      motor1.setSpeed(abs(M1speed));
    }
    else if (data.Y1 > 0) // forward
    {
      motor1.run(FORWARD);
      motor1.setSpeed(M1speed);
    }
    else if (data.Y1 == 0) // stay still
    {
      motor1.setSpeed(M1speed);
    }

    // Motor 2 controls:
    if (data.Y2 < 0) // backward
    {
      motor2.run(BACKWARD);
      motor2.setSpeed(abs(M2speed));
    }
    else if (data.Y2 > 0) // forward
    {
      motor2.run(FORWARD);
      motor2.setSpeed(M2speed);
    }
    else if (data.Y2 == 0) // stay still
    {
      motor2.setSpeed(M2speed);
    }

    // Wait 0.5 seconds:
    delay(500);
    
  }


  // AUTOMATIC MODE:
  if (mode == 0)
  {
    
    // Assign motor direction:
    // Adjust according to wiring such that motors run forward by default
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    
    // Clear the trigPin:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Set the trigPin on HIGH state for 10 ms:
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Read the echoPin and return the sound wave travel time in ms:
    duration = pulseIn(echoPin, HIGH);
    // Calculate the distance in cm:
    int distance = duration * 0.034 / 2;
    // Print the distance:
    Serial.print("Distance: ");
    Serial.println(distance);
  
    // Move forward if no obstacle is present:
    if (distance > 45) {
      motor1.setSpeed(125);
      motor2.setSpeed(125);
    }
    
  //  // Go backwards if obstacle is present:
  //  else {
  //    // Motor 1 forward
  //    motor1.setSpeed(0);
  //    motor1.run(BACKWARD);
  //    motor1.setSpeed(125);
  //    // Motor 2 backward
  //    motor2.setSpeed(0);
  //    motor2.run(BACKWARD);
  //    motor2.setSpeed(125);
    // Turn right if obstacle is present
    
    else {
      // Motor 1 forward:
      motor1.setSpeed(0);
      motor1.run(FORWARD);
      motor1.setSpeed(125);
      // Motor 2 backward:
      motor2.setSpeed(0);
      motor2.run(BACKWARD);
      motor2.setSpeed(125);
    }
    
    // Wait 0.5 seconds:
    delay(500);

  }

}
