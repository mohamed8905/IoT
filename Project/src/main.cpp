#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>
#include <ESPSupabase.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "cred.h"

#define LED 25
#define IR 27
#define BZ 14
#define HT 4   
#define MQ 35   
#define SER 18   
#define FAN 32   
#define FAN2 33   

int heatThreshold = 65;  
int gasThreshold = 2000;  
int irValue = 0;
int mqValue = 0;
float temperatureC = 0;
bool alarmActive = false;

WiFiClientSecure espClient;
PubSubClient client(espClient);

Servo myServo;
Supabase db;
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(HT);
DallasTemperature DS18B20(&oneWire);

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
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

  if (String(topic) == "SERVO") {
    int angle = message.toInt();
    if (angle >= 0 && angle <= 90) {
      myServo.write(angle + 90);
      Serial.print("Servo moved to: ");
      Serial.println(angle + 90);
      delay(1000);
    }
  }
  else if (String(topic) == "BUZZ") {
    int ON = message.toInt();
    digitalWrite(BZ, ON);
    delay(1000);
  }
  else if (String(topic) == "FAN") {
    int ON = message.toInt();
    digitalWrite(FAN, ON);
    delay(2000);
  }
  else if (String(topic) == "FAN2") {
    int ON = message.toInt();
    digitalWrite(FAN2, ON);
    delay(2000);
  }
  else if (String(topic) == "TEMP-threshold") {
    int threshold = message.toInt();
    if (threshold >= 0 && threshold <= 100) heatThreshold = threshold;
  }
  else if (String(topic) == "GAS-threshold") {
    int threshold = message.toInt();
    if (threshold >= 0 && threshold <= 4095) gasThreshold = threshold;
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

    lcd.setCursor(1, 0);
    lcd.print("Connecting to");
    lcd.setCursor(5, 1);
    lcd.print("MQTT");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("SERVO");
      client.subscribe("BUZZ");
      client.subscribe("TEMP-threshold");
      client.subscribe("GAS-threshold");
      client.subscribe("FAN");
      client.subscribe("FAN2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 sec");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Failed,Try again");
      for (int i = 3 ; i > 0 ; i--) {
        lcd.setCursor(4, 1);
        lcd.print("in ");
        lcd.print(i);
        lcd.print(" sec");
        delay(1000);
      }
    }
  }
}

void publishData() {
  StaticJsonDocument<200> doc;
  doc["sensor"] = "IR";
  doc["value"] = irValue;

  StaticJsonDocument<200> doc2;
  doc2["sensor"] = "HEAT";
  doc2["value"] = temperatureC;

  StaticJsonDocument<200> doc3;
  doc3["sensor"] = "Co2";
  doc3["value"] = mqValue;
  
  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);
  
  char jsonBuffer2[256];
  serializeJson(doc2, jsonBuffer2);
  
  char jsonBuffer3[256];
  serializeJson(doc3, jsonBuffer3);
  
  client.publish("esp32/IR", jsonBuffer);
  client.publish("esp32/HEAT", jsonBuffer2);
  client.publish("esp32/Co2", jsonBuffer3);
}

void setup() {
  Serial.begin(115200);

  lcd.init();        
  lcd.backlight();   

  lcd.setCursor(1, 0);
  lcd.print("Connecting to");
  lcd.setCursor(5, 1);
  lcd.print("WiFi");
  setupWiFi();
  setupMQTT();
  db.begin(supabase_url, anon_key);
  
  DS18B20.begin();
  pinMode(LED, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(FAN2, OUTPUT);
  pinMode(BZ, OUTPUT);
  pinMode(IR, INPUT);
  pinMode(MQ, INPUT);
  
  myServo.attach(SER);
  myServo.write(180);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HELLO");
  delay(1500);
  lcd.clear();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
    lcd.clear();
  }
  client.loop();
  
  irValue = digitalRead(IR);
  mqValue = analogRead(MQ);
  DS18B20.requestTemperatures();
  temperatureC = DS18B20.getTempCByIndex(0);
  delay(100);
  
  publishData();
  
  Serial.print("IR -> ");
  Serial.print(irValue);
  Serial.print("    HT -> ");
  Serial.print(temperatureC);
  Serial.print("    MQ -> ");
  Serial.print(mqValue);
  
  if (temperatureC > heatThreshold || mqValue > gasThreshold) {
    alarmActive = true;
    digitalWrite(BZ, HIGH);
    digitalWrite(FAN, HIGH);
    // digitalWrite(FAN2, HIGH);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALARM ACTIVATED");
    lcd.setCursor(0, 1);
    lcd.print("Cause:");
    lcd.setCursor(6, 1);
    lcd.print(temperatureC > heatThreshold ? "High TEMP" : "Gas Leak");
    
    myServo.write(90);
    digitalWrite(LED, HIGH);
    delay(1750);
  }
  else {
    digitalWrite(BZ, LOW);
    digitalWrite(FAN, LOW);
    // digitalWrite(FAN2, LOW);
    alarmActive = false;
  }

  if (!alarmActive) {
    if (!irValue) {
      digitalWrite(LED, HIGH);
      myServo.write(90);

      lcd.setCursor(0, 0);
      lcd.print("Motion Detected!");
      delay(1500);
    } else {
      lcd.clear();
      digitalWrite(LED, LOW);
      myServo.write(180);

      lcd.setCursor(0, 0);
      lcd.print("All Under CTRL");
    }
    
    lcd.setCursor(1, 1);
    lcd.print("HT -> ");
    
    if (temperatureC == DEVICE_DISCONNECTED_C) {
      lcd.print("Error   ");
    } else {
      lcd.print(temperatureC, 1);
      lcd.print((char)223); 
      lcd.print("C ");
    }
  }

  String JSON = "{\"on-door?\": " + String(!irValue) +
  ", \"temp\": " + String(temperatureC) +
  ", \"gas\": " + String(mqValue) +
  ", \"alarm-on?\": " + String(alarmActive) + "}";

  int code = db.insert(table, JSON, true);

  Serial.print("    HTTP Response Code: ");
  Serial.println(code);

  lcd.setCursor(15, 1);
  if (code == 201) {
    lcd.write('^');
  } else {
    lcd.write('!');
  }

  delay(150);
}