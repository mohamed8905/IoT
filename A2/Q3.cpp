#include <Arduino.h>
#define LED 25
#define BUTT 33
int st =0;
int cst =1;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUTT, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  st = digitalRead(BUTT);
  Serial.println(st == LOW ? "Pressed" : "Released");
  if (st == LOW) {
    cst=!cst;
    digitalWrite(LED, cst);
    delay(500);
  }
}
