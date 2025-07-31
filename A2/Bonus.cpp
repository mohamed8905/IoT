#include <Arduino.h>
#define LED 25
#define LDR 32
#define POT 33
int value =0;
int threshold =0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(POT, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(LDR);
  threshold = analogRead(POT);
  Serial.println("value -> " + String(value) + " threshold -> " + String(threshold));
  if (value >= threshold) digitalWrite(LED, LOW);
  else digitalWrite(LED, HIGH);
  // delay(50);
  
}

