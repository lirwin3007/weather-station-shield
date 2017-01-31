/*
WeatherStation.h - Library for working with the arduino weather station shield.
Created by Louis Irwin, September 19, 2016.
Released into the public domain.
*/

#ifndef WeatherStation_h
#define WeatherStation_h

#include "Arduino.h"

class WeatherStation
{
public:
	WeatherStation();
	float readLight();
	bool isRaindrop();
	float readRaindrop();
	float readHumidity();
	float readSoil();
};

#endif