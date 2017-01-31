/*
  Datalog
  Example weatherstation sketch that reads
  the values of all the sensors and then
  stores them on the on-board SD card in csv format

  This example code is in the public domain.
   The following libraries are required:
  - Sparkfun BMP180 Library (https://github.com/sparkfun/BMP180_Breakout_Arduino_Library/archive/master.zip)
  - SDFat Library (https://github.com/greiman/SdFat/archive/master.zip)
*/

#define measurementInterval 1000  //The time interval in which measurements will be taken (ms)

#include <WeatherStation.h> //Import the weatherStation library
#include <SFE_BMP180.h> //Import the Pressure Sensor library
#include <Wire.h> //Import the I2C Library

//#include <SD.h>
//Normally, the SD library would be used, however, this can be temperamental
//Instead, the SDFat libary is used, it has exactly the same commands as the SD library, so - in theory -
//The next 2 lines could be commented out a replaced with the '#include <SD.h>' line above if required
#include "SdFat.h"
SdFat SD;

WeatherStation myStation; //Create an instance of the weatherstation named 'myStation'
SFE_BMP180 pressureSensor;  //Create an instance of the pressure sensor named 'pressureSensor'

const int chipSelect = 10; //The SD card's chip select is connected to pin 10 - Not pin 4 as on the SD examples!

//Declare variables to store the state of the sensors
int light;
float raindrop;
float humidity;
float soilMoisture;
double pressure;
double temperature;

void setup() {
  Serial.begin(9600); // Setup serial port

  if (not pressureSensor.begin()) { //Initialise the pressure sensor and test for failure
    Serial.println("BMP180 init fail\n\n"); //  Print error message if needed
    while (1); // Pause forever
  }

  //Attempt to connect to the SD card, if this failes, print an error message an pause forever
  Serial.println("Verifying SD card connection");
  if (!SD.begin(chipSelect)) {
    Serial.println("Could not find SD card - is it properly inserted?");
    while (1); //Pause forever
  }
  
  Serial.println("Connection Successful!");
  setupHeaders(); //Setup headers in the CSV file
}

void loop() {
  readValues(); //Read the values from the sensor
  logValues(); //Log the values to the SD card
  delay(measurementInterval);  //Delay a defined ammount of time
}

void readValues() {
  light = myStation.readLight();  //Set the variable 'light' to the light intensity (%)
  raindrop = myStation.isRaindrop();  //Set the variable 'raindrop' to 'true' if there is a raindrop on the sensor, and 'false' if there is not
  humidity = myStation.readHumidity();  //Set the variable 'humidity' to the ambient humidity (%)
  soilMoisture = myStation.readSoil();  //Set the variable 'soilMoisture' to the soil moisture value (%)
  readBMP();  //Reads the temperature and pressure from the BMP180 IC
}

void readBMP() {
  //This subroutine reads the temperature and pressure from the BMP180 IC
  //Note: A temperature reading must be taken before a pressure reading
  //It is a much more basic version of the example 'SFE_BMP180_example' from the 'Sparkfun BMP180' Library
  //Go check that code out for features like sea-level-compensated pressure and much more

  //Each function that is used from the BMP180 library returns a value
  //If this is a zero, then something has gone wrong and the subroutine is exited
  //Else, this is the time that the arduino has to wait before it gets a pressure/temperature reading
  char waitTime;

  waitTime = pressureSensor.startTemperature(); //Start the temperature reading
  if (waitTime == 0) {
    return; //Something has gone wrong
  }
  delay(waitTime);  //Pause the allocated amount of time

  waitTime = pressureSensor.getTemperature(temperature);  //Get the temperature reading, and store it in the variable 'temperature'
  if (waitTime == 0) {
    return; //Something has gone wrong
  }

  waitTime = pressureSensor.startPressure(3); //Start the pressure reading, the '3' relates to the oversampling setting - see the example 'SFE_BMP180_example' from the 'Sparkfun BMP180' Library for more details
  if (waitTime == 0) {
    return; //Something has gone wrong
  }
  delay(waitTime);  //Pause the allocated amount of time

  waitTime = pressureSensor.getPressure(pressure, temperature); //Get the pressure reading, and store it in the variable 'pressure'. The temperature is required to be passed into the function
  if (waitTime == 0) {
    return; //Something has gone wrong
  }
}

void logValues() {
  //Open the file
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // If the file is available for writing to, write the data to it
  if (dataFile) {
    dataFile.print(millis() / 1000.00);
    dataFile.print(",");
    dataFile.print(light);
    dataFile.print(",");
    dataFile.print(raindrop);
    dataFile.print(",");
    dataFile.print(humidity);
    dataFile.print(",");
    dataFile.print(soilMoisture);
    dataFile.print(",");
    dataFile.print(pressure);
    dataFile.print(",");
    dataFile.println(temperature);
    dataFile.close();
    Serial.println("Log Success");
  }
  // If the file cannot be written to, print an error
  else {
    Serial.println("Error writing to datalog.csv");
  }
}

void setupHeaders() {
  //Create headers in the CSV file
  //Open the file
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // If the file is available for writing to, write the headers to it
  if (dataFile) {
    dataFile.print("Timestamp / s");
    dataFile.print(",");
    dataFile.print("Light / %");
    dataFile.print(",");
    dataFile.print("Raindrop Present");
    dataFile.print(",");
    dataFile.print("Humidity / %");
    dataFile.print(",");
    dataFile.print("Soil Moisture / %");
    dataFile.print(",");
    dataFile.print("Pressure / mb");
    dataFile.print(",");
    dataFile.println("Temperature / deg C");
    dataFile.close();
  }
  // If the file cannot be written to, print an error
  else {
    Serial.println("Error writing to datalog.csv");
  }
}

