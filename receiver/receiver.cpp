#include <SPI.h>
#include <LoRa.h>

#define BUZZER_PIN 8

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed");
    while (1);
  }
  Serial.println("LoRa Receiver Ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }
    Serial.print("Received: ");
    Serial.println(incoming);

    if (incoming.startsWith("ALERT:")) {
      int ttl = incoming.substring(6).toInt();
      if (ttl > 0) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(2000);
        digitalWrite(BUZZER_PIN, LOW);

        // Relay alert
        ttl -= 1;
        LoRa.beginPacket();
        LoRa.print("ALERT:");
        LoRa.print(ttl);
        LoRa.endPacket();
        Serial.println("Alert relayed");
      }
    }
  }
}
