#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "time.h"


const char* ssid = "AT8";
const char* password = "ATW$@8888";


const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600; 
const int daylightOffset_sec = 0;     

LiquidCrystal_I2C lcd(0x27, 16, 2);

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    lcd.setCursor(0, 1);
    lcd.print("Time fetch error");
    return;
  }

  char buffer[20];
  strftime(buffer, sizeof(buffer), "%I:%M:%S %p", &timeinfo);
  
  Serial.print("Current time: ");
  Serial.println(buffer);

  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(5, 1);
  lcd.print(buffer);
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  connectToWiFi();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("Waiting for NTP time...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.print(".");
    delay(1000);
  }
  
  lcd.clear();
}

void loop() {
  printLocalTime();
  delay(1000);
}
