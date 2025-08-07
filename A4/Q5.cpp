#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

const char* ssid = "AT8";
const char* password = "ATW$@8888";

const char* mqtt_server = "278db5193f6c4151a3b4fe36175cba56.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "Mossssssssssad";
const char* mqtt_password = "Bolbol1212";
const char* mqtt_topic = "iot/ir";

#define IR_PIN 35
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastSendTime = 0;
const long sendInterval = 90;

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String payload = "";
  for (unsigned int i = 0; i < length; i++) {
    payload += (char)message[i];
  }

  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) return;

  int irValue = doc["value"];

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IR Read -> ");
  lcd.print(irValue);
  lcd.setCursor(4, 1);
  lcd.print("A7la Msa");
}

void reconnect() {
  while (!client.connected()) {
    client.connect("ESP32Client", mqtt_user, mqtt_password);
    client.subscribe(mqtt_topic);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  setup_wifi();

  espClient.setInsecure();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastSendTime > sendInterval) {
    lastSendTime = now;

    int irValue = analogRead(IR_PIN);

    StaticJsonDocument<128> doc;
    doc["sensor"] = "IR";
    doc["value"] = irValue;

    char buffer[128];
    serializeJson(doc, buffer);

    client.publish(mqtt_topic, buffer);
  }
}
