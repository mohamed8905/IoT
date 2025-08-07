#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "AT8";
const char* password = "ATW$@8888";


const char* mqtt_server = "278db5193f6c4151a3b4fe36175cba56.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "Mossssssssssad";
const char* mqtt_password = "Bolbol1212";

#define SENSOR_PIN 33

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void setupMQTT() {
  espClient.setInsecure(); 
  client.setServer(mqtt_server, mqtt_port);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 sec");
      delay(5000);
    }
  }
}

void publishSensorData() {
  int sensorValue = analogRead(SENSOR_PIN);

  // Create JSON
  StaticJsonDocument<200> doc;
  doc["sensor"] = "potentiometer";
  doc["value"] = sensorValue;
  
  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);

  client.publish("esp32/sensor", jsonBuffer);
  Serial.println("Published: ");
  Serial.println(jsonBuffer);
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  publishSensorData();
  delay(900);
}
