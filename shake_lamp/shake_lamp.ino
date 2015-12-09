/**
 * Src for the shake lamp, that changes colors and patterns when you hit it.
 * 
 * Gyro Info: http://learn.parallax.com/KickStart/27911
 */

#include <Wire.h>

// gyro
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23

#define GYRO_ADDR 105                 // I2C address of gyro
int x, y, z;

int ledPin = 13;

void setup() {
  Wire.begin();

  // gyro config
  writeI2C(CTRL_REG1, 0x1F);    // Turn on all axes, disable power down
  writeI2C(CTRL_REG3, 0x08);    // Enable control ready signal
  writeI2C(CTRL_REG4, 0x80);    // Set scale (500 deg/sec)
  delay(100);      
   
  Serial.begin(9600);            // Wait to synchronize 
  
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);

  
  getGyroValues();              // Get new values
  // In following Dividing by 114 reduces noise
  Serial.print("Raw X:");  Serial.print(x / 114);  
  Serial.print(" Raw Y:"); Serial.print(y / 114);
  Serial.print(" Raw Z:"); Serial.println(z / 114);
  delay(500);                   // Short delay between reads
}

void getGyroValues () {
  byte MSB, LSB;

  MSB = readI2C(0x29);
  LSB = readI2C(0x28);
  x = ((MSB << 8) | LSB);

  MSB = readI2C(0x2B);
  LSB = readI2C(0x2A);
  y = ((MSB << 8) | LSB);

  MSB = readI2C(0x2D);
  LSB = readI2C(0x2C);
  z = ((MSB << 8) | LSB);
}

int readI2C (byte regAddr) {
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(regAddr);                // Register address to read
    Wire.endTransmission();             // Terminate request
    Wire.requestFrom(GYRO_ADDR, 1);          // Read a byte
    while(!Wire.available()) { };       // Wait for receipt
    return(Wire.read());                // Get result
}

void writeI2C (byte regAddr, byte val) {
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(regAddr);
    Wire.write(val);
    Wire.endTransmission();
}
