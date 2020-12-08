/*
  Using the BNO085 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output accelerometer values

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 115200 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_BNO085_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
BNO085 myIMU;

//These pins can be any GPIO
byte imuCSPin = 10;
byte imuWAKPin = 9;
byte imuINTPin = 8;
byte imuRSTPin = 7;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO085 Read Example");

  //Setup BNO085 to use SPI interface with default SPI port and max BNO085 clk speed of 3MHz
  myIMU.beginSPI(imuCSPin, imuWAKPin, imuINTPin, imuRSTPin);

  myIMU.enableLinearAccelerometer(50000); //Send data update every 50ms

  Serial.println(F("Linear Accelerometer enabled"));
  Serial.println(F("Output in form x, y, z, in m/s^2"));
}

void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {
    float x = myIMU.getLinAccelX();
    float y = myIMU.getLinAccelY();
    float z = myIMU.getLinAccelZ();
    byte linAccuracy = myIMU.getLinAccelAccuracy();

    Serial.print(x, 2);
    Serial.print(F(","));
    Serial.print(y, 2);
    Serial.print(F(","));
    Serial.print(z, 2);
    Serial.print(F(","));
    Serial.print(linAccuracy);

    Serial.println();
  }
}
