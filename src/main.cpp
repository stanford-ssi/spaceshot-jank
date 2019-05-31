#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include <TinyGPS++.h>

#define BMP_SCK   (14)
#define BMP_MISO  (12)
#define BMP_MOSI  (11)
#define BMP_CS    (10)
#define SerialGPS Serial5

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
Adafruit_BNO055 bno = Adafruit_BNO055();
TinyGPSPlus gps;

const int chipSelect = BUILTIN_SDCARD;

int num_samples = 1;
int setup_timestamp = 0;

void error(const char* msg) {
  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    Serial.println(msg);
  }
}

void setup() {

  Serial.begin(9600);
  SerialGPS.begin(9600);
  
  pinMode(39, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  for (size_t i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

  digitalWrite(39, LOW);
  digitalWrite(36, LOW);

  Wire.setSDA(38);
  Wire.setSCL(37);

  if (!bno.begin()) {
    Serial.println("ALACK! I have no BNOs!");
    error("No BNO");
  }

  /*
  if (!SD.begin(chipSelect)) {
    Serial.println("ALACK! I have no SD cards!");
    error("No SD card");
  }
  */

  if (!bmp.begin()) {
    Serial.println("ALACK! I have no bmps!");
    error("No BMP");
  }

  setup_timestamp = millis();
}


struct datalog {
  unsigned int time;

  float ax;
  float ay;
  float az;

  float rx;
  float ry;
  float rz;

  float temp;
  float pres;
  float alt;

  double gps_alt;   // meters 
  double gps_lat;   // degrees
  double gps_long;  // degrees
};

#define BUF_SIZE 2048 
struct datalog data[BUF_SIZE];

void loop() {
  /*
  if (num_samples % BUF_SIZE == 0) {
    File dataFile = SD.open("DATALOG.CSV", FILE_WRITE);
    if (!dataFile) {
      Serial.println("ALACK! I have no files!!");
      return;
    } else {
      Serial.println("** datalog event");
    }
    size_t amount_to_write = sizeof(struct datalog) * BUF_SIZE;
    dataFile.write( (char *) data, amount_to_write);
    dataFile.flush();
    Serial.println(amount_to_write);
  }
  */

  int i = num_samples % BUF_SIZE;
  data[i].time = micros();

  sensors_event_t event; 

  bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);
  data[i].ax = event.acceleration.x;
  data[i].ay = event.acceleration.y;
  data[i].az = event.acceleration.z;

  bno.getEvent(&event, Adafruit_BNO055::VECTOR_EULER);
  data[i].rx = event.orientation.x;
  data[i].ry = event.orientation.y;
  data[i].rz = event.orientation.z;

  data[i].temp = bmp.readTemperature();
  data[i].pres = bmp.readPressure();
  data[i].alt  = bmp.readAltitude(1013.25);

  while (SerialGPS.available() > 0) {
    Serial.print(SerialGPS.read());
    // gps.encode(SerialGPS.read());
  }

  data[i].gps_alt = gps.altitude.meters();
  data[i].gps_lat = gps.location.lat();
  data[i].gps_long = gps.location.lng();
  /* 
  Serial.println("Lat:");
  Serial.println(gps.location.lat(), 6);
  Serial.println("Lon:");
  Serial.println(gps.location.lng(), 6);
  */
  
  if (i == 0)
    digitalWrite(LED_BUILTIN, HIGH);
  if (i == BUF_SIZE / 2)
    digitalWrite(LED_BUILTIN, LOW);
  if (i == 0)
    Serial.println((float) num_samples / (millis() - setup_timestamp));

  num_samples++;
}
