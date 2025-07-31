#include <Arduino.h>
#define IR 34
int value = 0;
int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
  pinMode(IR, INPUT);
  lcd.init();
  lcd.backlight();
}

void loop(){
  value = analogRead(IR);

  lcd.setCursor(0, 0);
  lcd.print("IR read : ");
  lcd.print(value);
  if (value < 10) lcd.print("   "); 
  else if (value < 100) lcd.print("  "); 
  else if (value < 1000) lcd.print(" "); 
  lcd.print("        ");
  
  lcd.setCursor(0, 1);
  lcd.print("Distance : ");
  lcd.print(0.00255 * value);
  lcd.print(" cm");
  lcd.print("    ");  
  
  delay(200);
}