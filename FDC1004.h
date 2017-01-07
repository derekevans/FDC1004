/*
  FDC1004.h - Arduino library for communicating with the Texas Instrument FDC1004 
  Capacitance-to-Digital Converter.
  Created by Derek Evans, January 2017.
*/

#ifndef FDC1004_h
#define FDC1004_h

#define FDC_ADDRESS 0x50
#define MANUFACTURER_ID 0xFE
#define DEVICE_ID 0xFF
#define CONF_MEAS1 0x08
#define CONF_MEAS2 0x09
#define CONF_MEAS3 0x0A
#define CONF_MEAS4 0x0B
#define CONF_REGISTER 0x0C

#include "Arduino.h"
#include "Wire.h"

class FDC1004
{
  public:
  	float meas[4]; 
	float avgMeas[4];
	void init();
    void write(byte pointer, unsigned int value);
    unsigned int read(byte pointer);
    unsigned int manufacturerID(); 
    unsigned int deviceID();
    void configMeasure(unsigned int meas1, unsigned int meas2, unsigned int meas3, unsigned int meas4);
    void readMeasurement();
    void reset();
    void avgMeasurements(unsigned int n);
    void readAllRegistersBIN();
    void readAllRegistersHEX();
};

#endif
