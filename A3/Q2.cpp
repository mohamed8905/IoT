#include <Arduino.h>
#include <ESP32Servo.h>
#include <Keypad.h>
#define SERVO_PIN 25
bool servoEnabled = true;
Servo Ser;

const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

uint8_t rowPins[ROWS] = { 23, 22, 21, 19 };
uint8_t colPins[COLS] = { 18, 5, 17, 16 }; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  Ser.attach(SERVO_PIN); 
  Ser.write(0);
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY && (servoEnabled || key == '#')) {
    Serial.println(key);

    switch (key) {
      case '0':
        Ser.write(0);
        break;
      case '1':
        Ser.write(30);
        break;
      case '2':
        Ser.write(45);
        break;
      case '3':
        Ser.write(60);
        break;
      case '4':
        Ser.write(75);
        break;
      case '5':
        Ser.write(90);
        break;
      case '6':
        Ser.write(120);
        break;
      case '7':
        Ser.write(150);
        break;
      case '8':
        Ser.write(180);
        break;
      case '9':
        Ser.write(0);
        delay(500);
        Ser.write(180);
        delay(500);
        Ser.write(45);
        delay(500);
        Ser.write(135);
        delay(500);
        Ser.write(90);
        break;
      case 'A':
        Ser.write(0);
        delay(500);
        Ser.write(180);
        delay(500);
        Ser.write(0);
        break;
      case 'B':
        Ser.write(180);
        delay(500);
        Ser.write(0);
        delay(500);
        Ser.write(180);
        break;
      case 'C':
        Ser.write(0);
        delay(500);
        Ser.write(90);
        delay(500);
        Ser.write(180);
        delay(500);
        Ser.write(90);
        delay(500);
        Ser.write(0);
        break;
      case 'D':
        Ser.write(180);
        delay(500);
        Ser.write(90);
        delay(500);
        Ser.write(0);
        break;
      case '*':
        servoEnabled = false;
        break;
      case '#':
        servoEnabled = true;
        break;
    }
  }
}
