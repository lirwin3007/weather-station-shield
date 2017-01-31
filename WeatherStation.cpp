/*
WeatherStation.h - Library for working with the arduino weather station shield.
Created by Louis Irwin, September 19, 2016.
Released into the public domain.
*/

#include "Arduino.h"
#include "WeatherStation.h"

WeatherStation::WeatherStation(){}

float WeatherStation::readLight() {
	return((1023-analogRead(A0))/10.23);
}

bool WeatherStation::isRaindrop() {
	int reading = analogRead(A3);
	if (reading > 1000) { return false; } else { return true; }
}

float WeatherStation::readRaindrop() {
	return((1023-analogRead(A3))/10.23);
}

float WeatherStation::readHumidity() {
	float Vin = 5;
	float R = 10000;

	int reading = analogRead(A2);
	float Vout = (Vin/1023.0) * reading;

	float resistance = ( Vout * R ) / ( Vin - Vout );
	float humidity = pow(resistance, -0.182);
	return(421.14 * humidity);
}

float WeatherStation::readSoil() {
	return((1023-analogRead(A1))/10.23);
}