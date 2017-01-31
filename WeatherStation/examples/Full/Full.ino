/*
  Full
  Example weatherstation sketch that reads
  the values of all the sensors and then
  outputs the results to the serial monitor.

  The following libraries are required:
  - Sparkfun BMP180 Library (https://github.com/sparkfun/BMP180_Breakout_Arduino_Library/archive/master.zip)

*/

#include <WeatherStation.h> //Import the WeatherStation library
#include <SFE_BMP180.h> //Import the Pressure Sensor library
#include <Wire.h> //Import the I2C Library

WeatherStation myStation; //Create an instance of the weatherstation named 'myStation'
SFE_BMP180 pressureSensor;  //Create an instance of the pressure sensor named 'pressureSensor'

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

}


void loop() {
  readValues(); //Read the values from the sensor
  output(); //Output the values to the Serial monitor
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

  waitTime = pressureSensor.startPressure(3); //Start the pressure reading, the '3' relates to the oversamplin setting - see the example 'SFE_BMP180_example' from the 'Sparkfun BMP180' Library for more details
  if (waitTime == 0) {
    return; //Something has gone wrong
  }
  delay(waitTime);  //Pause the allocated amount of time

  waitTime = pressureSensor.getPressure(pressure, temperature); //Get the pressure reading, and store it in the variable 'pressure'. The temperature is required to be passed into the function
  if (waitTime == 0) {
    return; //Something has gone wrong
  }
}

void output() {

  //Format the variables and send them to the Serial monitor

  if (raindrop) {
    Serial.print("There is a raindrop on the sensor. |  ");
  }
  else {
    Serial.print("There is not a raindrop on the sensor. |  ");
  }

  Serial.print("Light Intensity: ");
  Serial.print(light);
  Serial.print("% |  ");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% |  ");

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.print("% |  ");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print("mb |  ");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("deg C |");

  Serial.println();
}

