/*
  Basic
  Example weatherstation sketch that reads
  the values of all the basic sensors and then
  outputs the results to the serial monitor.

  This example code is in the public domain.
*/

#include <WeatherStation.h> //Import the library

WeatherStation myStation; //Create an instance of the weatherstation named 'myStation'

//Declare variables to store the state of the sensors
int light;
float raindrop;
float humidity;
float soilMoisture;

void setup() {
  Serial.begin(9600); // Setup serial port
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

  Serial.println();
}

