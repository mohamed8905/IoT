#include <Arduino.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#define LED 4
#define LDR 14
#define buzz 12
#define butt 27
#define Ser 18
#define IR 5

int threshold = 2000;
int light = 0;
int butt_st = 1;
int cbutt = 0;
int ir_d = 0;
int lcdColumns = 16;
int lcdRows = 2;

Servo servo;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


void setup() {
  // put your setup code here, to run once:

  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(butt, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(IR, INPUT);

  servo.attach(Ser);
  servo.write(0);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  light = analogRead(LDR);
  cbutt = digitalRead(butt);
  ir_d = digitalRead(IR);
  
  if (cbutt) {butt_st = !butt_st; delay(150);}
  
  if (!butt_st) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Manual Override Activated");
    servo.write(0);
  }
  else if (!ir_d) {
    digitalWrite(buzz, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alert:");
    lcd.setCursor(0, 1);
    lcd.print("Motion Detected!");
    delay(5000);
    digitalWrite(buzz, LOW);
  }
  else if (light < threshold) {
    digitalWrite(LED, HIGH);
    servo.write(180);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Night Mode:");
    lcd.setCursor(0, 1);
    lcd.print("Door Locked");
  }
  else {
    digitalWrite(LED, LOW);
    servo.write(0);
    lcd.clear();
  }

  delay(100); 
}