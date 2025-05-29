#include <SPI.h>
#include <LoRa.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define THRESHOLD 100  // Distance in cm

int ttl = 3;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed");
    while (1);
  }
  Serial.println("LoRa Transmitter Ready");
}

void loop() {
  // Measure distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < THRESHOLD) {
    LoRa.beginPacket();
    LoRa.print("ALERT:");
    LoRa.print(ttl);
    LoRa.endPacket();
    Serial.println("Alert sent");
    delay(2000);  // Avoid spamming
  }

  delay(1000);
}
