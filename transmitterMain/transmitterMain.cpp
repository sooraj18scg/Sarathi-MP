#include <SPI.h>
#include <LoRa.h>
#include <TensorFlowLite.h>

const int trigPin = D1; // Ultrasonic sensor trig pin
const int echoPin = D2; // Ultrasonic sensor echo pin
const int ledPin = D3;  // LED pin for dashboard lights

TfLiteModel* model; // TensorFlow Lite model
TfLiteInterpreter* interpreter; // TensorFlow Lite interpreter

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Load the TensorFlow Lite model
  model = tflite::GetModel("model.tflite");
  if (model == nullptr) {
    Serial.println("Error loading TensorFlow Lite model");
    while (1);
  }

  // Create an interpreter to run the model
  interpreter = new TfLiteInterpreter(model);
  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("Error allocating tensors");
    while (1);
  }

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

  float threshold = predictThreshold(distance);
  
  if (distance < threshold) {
    digitalWrite(ledPin, HIGH);
    sendMessage(distance); // Transmit distance to nearby receiver modules
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(1000); // Delay for stability
}

float predictThreshold(float distance) {
  // Run the TensorFlow Lite model to predict the threshold
  TfLiteTensor* input = interpreter->input(0);
  input->data.f[0] = distance;

  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Error running TensorFlow Lite model");
    while (1);
  }

  TfLiteTensor* output = interpreter->output(0);
  return output->data.f[0];
}

void sendMessage(long distance) {
  LoRa.beginPacket();
  LoRa.print("Distance: ");
  LoRa.print(distance);
  LoRa.endPacket();
}
