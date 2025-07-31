#include <Arduino.h>
#include <ESP32Servo.h>
#define IR_ao 15
#define IR_do 25
#define SERVO_PIN 13
int value = 0;
int value2 = 0;

Servo Ser;

void setup() {
  Serial.begin(115200);
  pinMode(IR_ao, INPUT);
  pinMode(IR_do, INPUT);
  Ser.attach(SERVO_PIN); 
}

void loop() {
  value = analogRead(IR_ao);
  value2 = digitalRead(IR_do);
  Serial.printf("%d %d\n", value, value2);
  if (value2==0) {
    for (int pos = 0; pos <=180 ; pos+=30) Ser.write(pos);
    delay(1000);
    for (int pos = 180; pos >=0 ; pos-=30) Ser.write(pos);
  } 
  
}