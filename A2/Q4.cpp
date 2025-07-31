#include <Arduino.h>
#define LED_BUTT 12
#define LED_LDR 14
#define LED_POT 25
#define BUTT 13
#define LDR 27
#define POT 26

int st =0;
int cst =1;
int value_LDR =0;
int value_POT =0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUTT, OUTPUT);
  pinMode(LED_LDR, OUTPUT);
  pinMode(LED_POT, OUTPUT);
  pinMode(BUTT, INPUT_PULLUP);
  pinMode(LDR, INPUT);
  pinMode(POT, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  st = digitalRead(BUTT);
  value_LDR = analogRead(LDR);
  value_POT = analogRead(POT);

  digitalWrite(LED_LDR, value_LDR < 2000 ? 1 : 0);
  digitalWrite(LED_POT, value_POT < 2000 ? 1 : 0);

  Serial.printf((st == LOW ? "Pressed" : "Released"));
  Serial.printf(" value 1 -> %d" , value_LDR);
  Serial.printf(" value2 -> %d\n" , value_POT);

  if (st == LOW) {
    cst=!cst;
    digitalWrite(LED_BUTT, cst);
    delay(500);
  }


}