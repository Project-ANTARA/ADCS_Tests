#include <Wire.h>

#define AS5600_ADDR 0x36
#define STATUS_REG  0x0B
#define RAW_ANG_REG 0x0C

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA = GPIO 21, SCL = GPIO 22
  delay(500);

  // 1. Check I2C communication
  Wire.beginTransmission(AS5600_ADDR);
  if (Wire.endTransmission() == 0) {
    Serial.println("AS5600 detected successfully!");
  } else {
    Serial.println("Error: AS5600 not found. Check wiring (3V3, GND, SDA->21, SCL->22).");
    while (1);
  }
}

void loop() {
  // 2. Check Magnet Detection Status
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(STATUS_REG);
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDR, 1);
  
  if (Wire.available()) {
    uint8_t status = Wire.read();
    bool magnetDetected = status & 0x20; // Bit 5 (MD)
    bool magnetTooWeak  = status & 0x10; // Bit 4 (ML)
    bool magnetTooStrong = status & 0x08; // Bit 3 (MH)

    if (!magnetDetected) {
      Serial.println("No magnet detected! Hold a magnet 1-3mm above the chip.");
      delay(500);
      return;
    } else if (magnetTooWeak) {
      Serial.print("[Warning: Magnet too far] ");
    } else if (magnetTooStrong) {
      Serial.print("[Warning: Magnet too close] ");
    }
  }

  // 3. Read 12-bit Raw Angle (0 - 4095)
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(RAW_ANG_REG);
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDR, 2);

  if (Wire.available() == 2) {
    uint16_t highByte = Wire.read();
    uint16_t lowByte  = Wire.read();
    uint16_t rawAngle = ((highByte & 0x0F) << 8) | lowByte;
    
    // Convert 12-bit value (0-4095) to degrees (0-360)
    float degrees = (rawAngle * 360.0) / 4096.0;

    Serial.print("Raw: ");
    Serial.print(rawAngle);
    Serial.print(" | Angle: ");
    Serial.print(degrees, 2);
    Serial.println(" deg");
  }

  delay(200);
}