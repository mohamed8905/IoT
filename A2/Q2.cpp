#include <Arduino.h>
#define LED 25
#define LDR 34
int value =0;


void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = digitalRead(LDR);
  digitalWrite(LED, !value);
}
