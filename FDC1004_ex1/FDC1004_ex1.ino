#include <Wire.h>
#include <FDC1004.h>

FDC1004 fdc1004;

void setup() {
  fdc1004.init();
  fdc1004.reset();
  Serial.begin(9600);
  Serial.print("Manufacturer ID: ");
  Serial.println(fdc1004.manufacturerID(), HEX);
  Serial.print("Device ID: ");
  Serial.println(fdc1004.deviceID(), HEX);
  Serial.println();
  
  Serial.println("Register values");
  fdc1004.readAllRegistersHEX();
  //fdc1004.readAllRegistersBIN();
  Serial.println();
  
  fdc1004.configMeasure(0x0800,0x2800,0x5C00,0x7C00);

}

void loop() {
  delay(5000);
  Serial.println();
  Serial.println("Single Measurement Reading: ");
  fdc1004.readMeasurement();
  Serial.print("MEAS1: ");
  Serial.println(fdc1004.meas[0],4);
  Serial.print("MEAS2: ");
  Serial.println(fdc1004.meas[1],4);
  Serial.print("MEAS3: ");
  Serial.println(fdc1004.meas[2],4);
  Serial.print("MEAS4: ");
  Serial.println(fdc1004.meas[3],4);
  Serial.println("Average of 10 Measurement Readings: ");
  fdc1004.avgMeasurements(10);
  Serial.print("MEAS1: ");
  Serial.println(fdc1004.avgMeas[0],4);
  Serial.print("MEAS2: ");
  Serial.println(fdc1004.avgMeas[1],4);
  Serial.print("MEAS3: ");
  Serial.println(fdc1004.avgMeas[2],4);
  Serial.print("MEAS4: ");
  Serial.println(fdc1004.avgMeas[3],4);
}
