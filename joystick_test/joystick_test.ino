void setup() {

  Serial.begin(9600)

}

void loop() {
  
  float X = 0;
  float Y = 0;

  X = analogRead(A3);
  Y = analogRead(A5);

  Serial.print("X: ");
  Serial.print(X);
  Serial.print("Y: ");
  Serial.print(Y);

  delay(1000);

}

// Center: X,Y = 509
// Up: Y = 0 (X = 509)
// Down: Y = 1023 (X = 509)
// Left: X = 0 (Y = 509)
// Right: X = 1023 (Y = 509)