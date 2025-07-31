#include <Arduino.h>
#define LED 25
#define POT 33
int value =0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(POT, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(POT);
  Serial.println("value -> " + String(value));
  analogWrite(LED, value);
  delay(50);
  
}
