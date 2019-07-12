/*
  Temperature Alerts with the MCP9600 Thermocouple Amplifier
  By: Fischer Moseley
  SparkFun Electronics
  Date: July 12, 2019
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware License).

  This example shows how to use alerts on the MCP9600! The following code is intented to print out the temperature
  like normal, but when you touch the thermocouple it triggers an alert! It will also trigger another alert when
  you remove your hand. Both alerts will be printed over serial so that you can see them, and will also trigger pins
  on the MCP9600 to change state. These can be read with a multimeter or oscilloscope.

  Since each alert can only trigger on either a rising (cold -> hot) or falling (hot -> cold) transition, we 
  need to configure two alerts. 

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 115200 baud to serial monitor.
*/
#include <Wire.h>
#include <SparkFun_MCP9600.h>

MCP9600 tempSensor;
uint8_t risingAlert = 1; //What alert to use for detecting cold -> hot transitions.
uint8_t fallingAlert = 3; //What alert to use for detecting hot -> cold transitions.
                          //These numbers are arbitrary and can be anything from 1 to 4, but just can't be equal!

float alertTemp = 25.5;  //What temperature to trigger the alert at (before hysteresis).
                        //This is about the surface temperature of my finger, but please change this if 
                        //you have colder/warmer hands or if the ambient temperature is different.
uint8_t hysteresis = 2; //How much hysteresis to have, in degrees Celcius. Feel free to adjust this, but 2°C seems to be about right.

void setup(){
    Serial.begin(115200);
    Wire.begin(10000);

  //check if the sensor is connected
  if(tempSensor.isConnected()){
    Serial.println("Device will acknowledge!");
  }
  else {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1); //hang forever
  }

  //check if the Device ID is correct
  if(tempSensor.checkDeviceID()){
    Serial.println("Device ID is correct!");        
  }
  else {
    Serial.println("Device ID is not correct! Freezing.");
    while(1); //hang forever
  }

  //configure the temperature alert for when the thermocouple is touched
  tempSensor.configAlertEnable(risingAlert, 0); //disable the alert (if it was already enabled) while we configure it
  tempSensor.configAlertTemp(risingAlert, alertTemp);
  tempSensor.configAlertJunction(risingAlert, 0);
  tempSensor.configAlertHysteresis(risingAlert, hysteresis);
  tempSensor.configAlertEdge(risingAlert, HIGH);
  tempSensor.configAlertLogicLevel(risingAlert, HIGH);
  tempSensor.configAlertMode(risingAlert, 1);
  tempSensor.configAlertEnable(risingAlert, 1); //enable the alert!

  //configure the temperature alert for when the thermocouple is released!
  tempSensor.configAlertEnable(fallingAlert, 0); //disable the alert (if it was already enabled) while we configure it
  tempSensor.configAlertTemp(fallingAlert, alertTemp);
  tempSensor.configAlertJunction(fallingAlert, 0);
  tempSensor.configAlertHysteresis(fallingAlert, hysteresis);
  tempSensor.configAlertEdge(fallingAlert, HIGH);
  tempSensor.configAlertLogicLevel(fallingAlert, HIGH);
  tempSensor.configAlertMode(fallingAlert, 1);
  tempSensor.configAlertEnable(fallingAlert, 1); //enable the alert!
}

unsigned long clock = millis();
uint16_t updateTime = 200;
void loop(){
  if(clock < (millis() + updateTime)){
    Serial.print("Thermocouple: ");
    Serial.print(tempSensor.thermocoupleTemp());
    Serial.print(" °C   Ambient: ");
    Serial.print(tempSensor.ambientTemp());
    Serial.print(" °C   Temperature Delta: ");
    Serial.print(tempSensor.tempDelta());
    Serial.print(" °C");
    Serial.println(); 
  }

  if(tempSensor.isAlertTriggered(risingAlert)){
    Serial.println("Thermocouple has been touched!");
    tempSensor.clearAlert(risingAlert);
  }

  if(tempSensor.isAlertTriggered(fallingAlert)){
    Serial.println("Thermocouple has been released!");
    tempSensor.clearAlert(fallingAlert);
  }
}