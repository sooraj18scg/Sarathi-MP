#include <SPI.h>
#include <LoRa.h>

const int trigPin = D1; // Ultrasonic sensor trig pin
const int echoPin = D2; // Ultrasonic sensor echo pin
const int ledPin = D3;  // LED pin for dashboard lights

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Initialize LoRa module
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (1);
  }
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < 50) { 
// Adjust the threshold distance as needed
    digitalWrite(ledPin, HIGH);
    sendMessage(distance); // Transmit distance to nearby receiver modules
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(1000); // Delay for stability
}

void sendMessage(long distance) {
  LoRa.beginPacket();
  LoRa.print("Distance: ");
  LoRa.print(distance);
  LoRa.endPacket();
}
