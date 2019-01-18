#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (14)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
Adafruit_BNO055 bno = Adafruit_BNO055(55);

const int chipSelect = BUILTIN_SDCARD;

void error() {
  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

void setup() {
  
  pinMode(39, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(39, LOW);
  digitalWrite(36, LOW);

  Wire.setSDA(38);
  Wire.setSCL(37);

  if (!bno.begin()) {
    Serial.println("ALACK! I have no BNOs!");
    error();
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("ALACK! I have no SD cards!");
    error();
  }

  if (!bmp.begin()) {
    Serial.println("ALACK! I have no bmps!");
    error();
  }

}

void loop() {

  File dataFile = SD.open("LAUNCH_DATA.csv", FILE_WRITE);
  if (!dataFile) {
    Serial.println("ALACK! I have no files!!");
    error();
  }

  sensors_event_t event; 
  bno.getEvent(&event);

  dataFile.print(event.orientation.x, 16);
  dataFile.print(", ");
  dataFile.print(event.orientation.y, 16);
  dataFile.print(", ");
  dataFile.print(event.orientation.z, 16);
  dataFile.print(", ");
  dataFile.print(bmp.readTemperature(), 16);
  dataFile.print(", ");
  dataFile.print(bmp.readPressure(), 16);
  dataFile.print(", ");
  dataFile.print(bmp.readAltitude(1013.25), 16);
  dataFile.println("");
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
}
