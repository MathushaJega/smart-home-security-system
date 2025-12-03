int relayPin = 7;

void setup() {
  pinMode(relayPin, OUTPUT);
}

void loop() {
  digitalWrite(relayPin, HIGH); // Turn relay ON
  delay(3000);                  // Wait 1 second
  digitalWrite(relayPin, LOW);  // Turn relay OFF
  delay(3000);                  // Wait 1 second
}
