/*
 This program tests the Holocube BH1750 Light Sensor interface
 Remember to copy pin_definitions.h into soc_test folder or run ../header.sh -c
*/

#include "pin_definitions.h"
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup(){

  Serial.begin(9600);

  Wire.begin(SYSTEM_I2C_SDA_PIN, SYSTEM_I2C_SCL_PIN);
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, LIGHT_SENSOR_ADDRESS, &Wire);

  Serial.println(F("BH1750 Test begin"));

}


void loop() {

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

}
