#include <SPI.h>
#include <LoRa.h>

// Define the LoRa module pins
#define LORA_SCK 5
#define LORA_CS 18
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_RST 14
#define LORA_IRQ 26

// Define the node's address
#define NODE_ADDRESS 1

// Define the number of nodes behind this node
#define NUM_BEHIND_NODES 50

void setup() {
  Serial.begin(9600);

  // Initialize LoRa module
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (1);
  }
}

void loop() {
  if (LoRa.parsePacket()) {
    Message message = receiveMessage();
    processMessage(message);
  }
}

struct Message {
  int senderAddress;
  int receiverAddress;
  String content;
};

Message receiveMessage() {
  String messageString = "";
  while (LoRa.available()) {
    messageString += (char)LoRa.read();
  }
  // Parse message string into Message struct
  Message message;
  message.senderAddress = ...; // Extract sender address from messageString
  message.receiverAddress = ...; // Extract receiver address from messageString
  message.content = ...; // Extract message content from messageString
  return message;
}

void processMessage(Message message) {
  if (message.receiverAddress == NODE_ADDRESS) {
    // Message is for this node
    triggerWarning();
    forwardMessage(message);
  }
}

void triggerWarning() {
 

}

void forwardMessage(Message message) {
  // Forward message to the next node in the bus
  int nextNode = (NODE_ADDRESS % NUM_BEHIND_NODES) + 1; // Assuming nodes are numbered sequentially
  sendMessage(nextNode, message);
}

void sendMessage(int nextNode, Message message) {
  
}
