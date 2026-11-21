#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

SFE_MMC5983MA myMag;

void setup() {
  Serial.begin(115200);
  delay(500);

  Wire.begin(); // Defaults to SDA = GPIO 21, SCL = GPIO 22

  if (myMag.begin(Wire) == false) {
    Serial.println("MMC5983MA not detected. Check GPIO 21/22 wiring!");
    while (1) delay(10);
  }

  myMag.softReset();
  Serial.println("MMC5983MA connected to ESP32!");
}

void loop() {
  uint32_t rawX = 0, rawY = 0, rawZ = 0;

  if (myMag.getMeasurementXYZ(&rawX, &rawY, &rawZ)) {
    double scaledX = ((double)rawX - 131072.0) / 131072.0 * 8.0;
    double scaledY = ((double)rawY - 131072.0) / 131072.0 * 8.0;
    double scaledZ = ((double)rawZ - 131072.0) / 131072.0 * 8.0;

    Serial.print("X: "); Serial.print(scaledX, 4);
    Serial.print(" G | Y: "); Serial.print(scaledY, 4);
    Serial.print(" G | Z: "); Serial.print(scaledZ, 4);
    Serial.println(" G");
  }
  delay(200);
}