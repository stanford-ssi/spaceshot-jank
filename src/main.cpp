#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  
  pinMode(39, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(39, LOW);
  digitalWrite(36, LOW);

  Wire.setSDA(38);
  Wire.setSCL(37);

  while(!bno.begin()){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
