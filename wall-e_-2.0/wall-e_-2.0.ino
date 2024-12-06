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

AF_DCMotor motor1(1);
AF_DCMotor motor2(4);

const int trigPin = A1;
const int echoPin = A2;

long duration;
int distance;

void setup() {
  
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  
  Serial.begin(9600);
  
  motor1.setSpeed(200);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(RELEASE);
  
}

void loop() {

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
  distance = duration * 0.034 / 2;
  // Print the distance:
  Serial.print("Distance: ");
  Serial.println(distance);

  // Move forward if no obstacle is present:
  if (distance > 30) {
    motor1.setSpeed(125);
    motor2.setSpeed(125);
  }

  // Turn right if obstacle is present
  else {
    // Motor 1 forward
    motor1.setSpeed(0);
    motor1.setSpeed(125);
    // Motor 2 backward
    motor2.setSpeed(0);
    motor2.run(BACKWARD);
    motor2.setSpeed(125);
  }
  
  // Wait 0.5 seconds
  delay(500);

}
