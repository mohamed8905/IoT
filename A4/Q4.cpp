#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// WiFi credentials
const char* ssid = "M7Moud";
const char* password = "m12m12m12";

// HiveMQ Cloud credentials
const char* mqtt_server = "278db5193f6c4151a3b4fe36175cba56.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "Mossssssssssad";
const char* mqtt_password = "Bolbol1212";

#define SENSOR_PIN 33
#define SERVO_PIN 32

Servo myServo;

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

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // If the topic is for the servo, set the angle
  if (String(topic) == "esp32/servo") {
    int angle = message.toInt();
    if (angle >= 0 && angle <= 180) {
      myServo.write(angle);
      Serial.print("Servo moved to: ");
      Serial.println(angle);
    }
  }
}

void setupMQTT() {
  espClient.setInsecure(); 
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("esp32/servo");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again");
      delay(2500);
    }
  }
}

void publishSensorData() {
  int sensorValue = analogRead(SENSOR_PIN);

  client.publish("esp32/ir", String(sensorValue).c_str());
  Serial.print("Published: ");
  Serial.println(sensorValue);
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
  myServo.attach(SERVO_PIN);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  publishSensorData();
  delay(900);
}