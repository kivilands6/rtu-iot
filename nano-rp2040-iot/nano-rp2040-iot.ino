#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

char ssid[] = "BALTICOM2G_7826283";
char pass[] = "esccyhwzcss10";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char broker[] = "miot.ditf.rtu.lv";
const int port = 1883;

const char topic1[] = "NTNU/Ard2/out/LightSen";
const char topic2[] = "NTNU/Web/LED";
const char topic3[] = "NTNU/Ard2/out/LedState";
const char topic4[] = "NTNU/Ard2/out/ButtonState";

void setup() {
  pinMode(13, OUTPUT); // LED
  pinMode(A5, INPUT_PULLUP); // Button
  pinMode(A3, INPUT); // Light Sensor
  Serial.begin(9600);

  Serial.print("Connecting to WiFi: ");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("Connected to WiFi");

  mqttClient.setServer(broker, port);
  mqttClient.setCallback(onMqttMessage);

  Serial.print("Connecting to MQTT broker: ");
  while (!mqttClient.connect("RP2040Client")) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("Connected to MQTT broker");

  mqttClient.subscribe(topic2);
}

void loop() {
  mqttClient.loop();

  int lightSen = analogRead(A3);
  bool button = !digitalRead(A5);

  Serial.print("Publishing to topic: ");
  Serial.println(topic1);
  Serial.println(lightSen);
  mqttClient.publish(topic1, String(lightSen).c_str());

  Serial.print("Publishing to topic: ");
  Serial.println(topic3);
  Serial.println(button);
  mqttClient.publish(topic3, String(button).c_str());

  Serial.print("Publishing to topic: ");
  Serial.println(topic4);
  Serial.println(lightSen);
  mqttClient.publish(topic4, String(lightSen).c_str());

  delay(500);
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message on topic: ");
  Serial.println(topic);

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Message: " + message);

  if (strcmp(topic, topic2) == 0) {
    if (message == "on") {
      digitalWrite(13, HIGH);
    } else if (message == "off") {
      digitalWrite(13, LOW);
    } else {
      Serial.println("Invalid message!");
    }
  }
}
