/*
  Setting Thermocouple Type with the MCP9600 Thermocouple Amplifier
  By: Fischer Moseley
  SparkFun Electronics
  Date: July 8, 2019
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware License).

  This example outputs the ambient and thermocouple temperatures from the MCP9600 sensor, but allows for a non
  K-type thermocouple to be used.

  The Qwiic MCP9600 supports K/J/T/N/S/E/B/R type thermocouples, and the type can be configured below!

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 115200 baud to serial monitor.
*/


#include <SparkFun_MCP9600.h>
MCP9600 tempSensor;
Thermocouple_Type type = TYPE_S; //the type of thermocouple to change to!

void setup(){
    Serial.begin(115200);
    Wire.begin(10000);

    //check that the sensor is connected
    if(tempSensor.isConnected()){
        Serial.println("Device will acknowledge!");
    }
    else {
        Serial.println("Device did not acknowledge! Freezing.");
        while(1); //hang forever
    }

    //check that the Device ID is correct
    if(tempSensor.checkDeviceID()){
        Serial.println("Device ID is correct!");        
    }
    else {
        Serial.println("Device ID is not correct! Freezing.");
        while(1);
    }

    //change the thermocouple type being used
    Serial.println("Setting Thermocouple Type!");
    tempSensor.setThermocoupleType(type);

    //make sure the type was set correctly!
    if(tempSensor.getThermocoupleType() == type){
        Serial.println("Thermocouple Type set sucessfully!");
    }

    else{
        Serial.println("Setting Thermocouple Type failed!");
    }
}

void loop(){ //print the thermocouple, ambient and delta temperatures every 200ms
    Serial.print("Thermocouple: ");
    Serial.print(tempSensor.thermocoupleTemp());
    Serial.print(" °C   Ambient: ");
    Serial.print(tempSensor.ambientTemp());
    Serial.print(" °C   Temperature Delta: ");
    Serial.print(tempSensor.tempDelta());
    Serial.print(" °C");
    Serial.println();
    delay(200);
}