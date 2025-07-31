#include <Arduino.h>
#include <ESP32Servo.h>
#define LED 13
#define LDR 14
#define buzz 12
#define butt 27
#define Ser 2

int threshold = 2000;
int light = 0;
int butt_st = 0;
int cbutt = 0;

Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(butt, INPUT);
  pinMode(buzz, OUTPUT);
  
  servo.attach(Ser);
  servo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  light = analogRead(LDR);
  cbutt = digitalRead(butt);
  Serial.print("light -> " + String(light));

  if (cbutt) {butt_st = !butt_st; delay(150);}

  if (light < threshold && !butt_st) {
    digitalWrite(LED, HIGH);
    digitalWrite(buzz, HIGH);
    servo.write(180);
    Serial.println(" (Alarm Activated)");
  }
  else {
    digitalWrite(LED, LOW);
    digitalWrite(buzz, LOW);
    servo.write(0);
    Serial.println(butt_st ? " (manually silenced)" : " (Safe situation)");
  }

  delay(100); 
}