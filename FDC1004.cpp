/*
  FDC1004.h - Arduino library for communicating with the Texas Instrument FDC1004 
  Capacitance-to-Digital Converter.
  Created by Derek Evans, January 2017.
*/

#include "Arduino.h"
#include "FDC1004.h"

void FDC1004::init() {
  	Wire.begin(FDC_ADDRESS);
}

void FDC1004::write(byte pointer, unsigned int value) {
	byte v[2];
	v[0] = ((value>>8) & 0xFF);//MSB First
	v[1] = ((value>>0) & 0xFF);
	Wire.beginTransmission(FDC_ADDRESS);
	Wire.write(pointer);
	Wire.write(v,2);
	Wire.endTransmission();
}

unsigned int FDC1004::read(byte pointer) {
	union {
		byte b[2];
		unsigned int ival;
	} fdc_read;

	Wire.beginTransmission(FDC_ADDRESS);
	Wire.write(pointer);
	Wire.endTransmission();
	Wire.requestFrom(FDC_ADDRESS,2);
	if(Wire.available()) {
		fdc_read.b[1] = Wire.read();
		fdc_read.b[0] = Wire.read();
		return fdc_read.ival;
	}  
}

unsigned int FDC1004::manufacturerID() {
	return(read(MANUFACTURER_ID));
}

unsigned int FDC1004::deviceID() {
	return(read(DEVICE_ID));
}

void FDC1004::configMeasure(unsigned int meas1, unsigned int meas2, unsigned int meas3, unsigned int meas4) {
	write(CONF_MEAS1,meas1);
	write(CONF_MEAS2,meas2);
	write(CONF_MEAS3,meas3);
	write(CONF_MEAS4,meas4);
}

void FDC1004::readMeasurement() {

	unsigned int readRegister[4][2] = {
		{0x00,0x01},
		{0x02,0x03},
		{0x04,0x05},
		{0x06,0x07}
	};

	union {
		byte b[2];
		unsigned int ival;
	} conf;

	union {
		byte b[3];
		unsigned long lval;
	} cap;

	write(CONF_REGISTER,0); //reset config register
	conf.ival = 0; 
	bitSet(conf.b[1],2);//100S/s
	float capRead[4];

	for(int i = 0; i<4; i++) {
		bitSet(conf.b[0],7-i);//Initiate measurement on cin i
		write(CONF_REGISTER,conf.ival);
		conf.ival = read(CONF_REGISTER);
		int readyBit = bitRead(conf.b[0],3-i);
		while(readyBit==0) {
			conf.ival = read(CONF_REGISTER);
			readyBit = bitRead(conf.b[0],3-i);
		}
		unsigned int msb = read(readRegister[i][0]);
		unsigned int lsb = read(readRegister[i][1]);
		cap.b[2] = (msb >> 8) & 0xFF;
		cap.b[1] = (msb >> 0) & 0xFF;
		cap.b[0] = (lsb >> 8) & 0xFF;

		capRead[i] = cap.lval/pow(2,19);
		bitClear(conf.b[0],7-i);
		write(CONF_REGISTER,conf.ival);
	}

	meas[0] = capRead[0];
	meas[1] = capRead[1];
	meas[2] = capRead[2];
	meas[3] = capRead[3];

	delay(10);
}

void FDC1004::reset() {
	union {
		byte b[2];
		unsigned int ival;
	} reset;

	reset.ival = read(CONF_REGISTER);
	bitSet(reset.b[1],7);
	write(CONF_REGISTER,reset.ival);
}

void FDC1004::avgMeasurements(unsigned int n) {
   
   float sum1 = 0;
   float sum2 = 0;
   float sum3 = 0;
   float sum4 = 0;
   
   for (int i=0; i<n; i++) {
     readMeasurement();
     sum1 = sum1 + meas[0];
     sum2 = sum2 + meas[1];
     sum3 = sum3 + meas[2];
     sum4 = sum4 + meas[3];
   }
   
   avgMeas[0] = sum1/(float)n;
   avgMeas[1] = sum2/(float)n;
   avgMeas[2] = sum3/(float)n;
   avgMeas[3] = sum4/(float)n;
}

void FDC1004::readAllRegistersBIN() {
	byte reg = 0;
	Serial.print("MEAS_1_MSB: ");
	Serial.println(read(reg++),BIN); 
	Serial.print("MEAS_1_LSB: ");
	Serial.println(read(reg++),BIN);
	Serial.print("MEAS_2_MSB: ");
	Serial.println(read(reg++),BIN);
	Serial.print("MEAS_2_LSB: ");
	Serial.println(read(reg++),BIN);
	Serial.print("MEAS_3_MSB: ");
	Serial.println(read(reg++),BIN);
	Serial.print("MEAS_3_LSB: ");
	Serial.println(read(reg++),BIN);
	Serial.print("MEAS_4_MSB: ");
	Serial.println(read(reg++),BIN);
	Serial.print("MEAS_4_LSB: ");
	Serial.println(read(reg++),BIN);
	Serial.print("CONF_MEAS_1: ");
	Serial.println(read(reg++),BIN);
	Serial.print("CONF_MEAS_2: ");
	Serial.println(read(reg++),BIN);
	Serial.print("CONF_MEAS_3: ");
	Serial.println(read(reg++),BIN);
	Serial.print("CONF_MEAS_4: ");
	Serial.println(read(reg++),BIN); 
	Serial.print("FDC_CONF: ");
	Serial.println(read(reg++),BIN);
	Serial.print("OFFSET_CAL_CIN1: ");
	Serial.println(read(reg++),BIN);
	Serial.print("OFFSET_CAL_CIN2: ");
	Serial.println(read(reg++),BIN);
	Serial.print("OFFSET_CAL_CIN3: ");
	Serial.println(read(reg++),BIN);
	Serial.print("OFFSET_CAL_CIN4: ");
	Serial.println(read(reg++),BIN);
	Serial.print("GAIN_CAL_CIN1: ");
	Serial.println(read(reg++),BIN);
	Serial.print("GAIN_CAL_CIN2: ");
	Serial.println(read(reg++),BIN);
	Serial.print("GAIN_CAL_CIN3: ");
	Serial.println(read(reg++),BIN);
	Serial.print("GAIN_CAL_CIN4: ");
	Serial.println(read(reg++),BIN);
}

void FDC1004::readAllRegistersHEX() {
	byte reg = 0;
	Serial.print("MEAS_1_MSB: ");
	Serial.println(read(reg++),HEX); 
	Serial.print("MEAS_1_LSB: ");
	Serial.println(read(reg++),HEX);
	Serial.print("MEAS_2_MSB: ");
	Serial.println(read(reg++),HEX);
	Serial.print("MEAS_2_LSB: ");
	Serial.println(read(reg++),HEX);
	Serial.print("MEAS_3_MSB: ");
	Serial.println(read(reg++),HEX);
	Serial.print("MEAS_3_LSB: ");
	Serial.println(read(reg++),HEX);
	Serial.print("MEAS_4_MSB: ");
	Serial.println(read(reg++),HEX);
	Serial.print("MEAS_4_LSB: ");
	Serial.println(read(reg++),HEX);
	Serial.print("CONF_MEAS_1: ");
	Serial.println(read(reg++),HEX);
	Serial.print("CONF_MEAS_2: ");
	Serial.println(read(reg++),HEX);
	Serial.print("CONF_MEAS_3: ");
	Serial.println(read(reg++),HEX);
	Serial.print("CONF_MEAS_4: ");
	Serial.println(read(reg++),HEX); 
	Serial.print("FDC_CONF: ");
	Serial.println(read(reg++),HEX);
	Serial.print("OFFSET_CAL_CIN1: ");
	Serial.println(read(reg++),HEX);
	Serial.print("OFFSET_CAL_CIN2: ");
	Serial.println(read(reg++),HEX);
	Serial.print("OFFSET_CAL_CIN3: ");
	Serial.println(read(reg++),HEX);
	Serial.print("OFFSET_CAL_CIN4: ");
	Serial.println(read(reg++),HEX);
	Serial.print("GAIN_CAL_CIN1: ");
	Serial.println(read(reg++),HEX);
	Serial.print("GAIN_CAL_CIN2: ");
	Serial.println(read(reg++),HEX);
	Serial.print("GAIN_CAL_CIN3: ");
	Serial.println(read(reg++),HEX);
	Serial.print("GAIN_CAL_CIN4: ");
	Serial.println(read(reg++),HEX);
}