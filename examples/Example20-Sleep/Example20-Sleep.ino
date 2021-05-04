/*
  Using the BNO085 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output the i/j/k/real parts of the rotation vector.
  https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO085 breakout to avoid polling.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_BNO085_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
BNO085 myIMU;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO085 Sleep Example");

  Wire.begin();

  myIMU.begin();

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  myIMU.enableRotationVector(50000); //Send data update every 50ms

  Serial.println(F("Rotation vector enabled"));
  Serial.println(F("Output in form i, j, k, real, accuracy"));
}

unsigned long lastMillis = 0; // Keep track of time
bool lastPowerState = true; // Toggle between "On" and "Sleep"

void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable())
  {
    float quatI = myIMU.getQuatI();
    float quatJ = myIMU.getQuatJ();
    float quatK = myIMU.getQuatK();
    float quatReal = myIMU.getQuatReal();
    float quatRadianAccuracy = myIMU.getQuatRadianAccuracy();

    Serial.print(quatI, 2);
    Serial.print(F(","));
    Serial.print(quatJ, 2);
    Serial.print(F(","));
    Serial.print(quatK, 2);
    Serial.print(F(","));
    Serial.print(quatReal, 2);
    Serial.print(F(","));
    Serial.print(quatRadianAccuracy, 2);
    Serial.print(F(","));

    Serial.println();
  }

  //Check if it is time to change the power state
  if (millis() > (lastMillis + 5000)) // Change state every 5 seconds
  {
    lastMillis = millis(); // Keep track of time

    if (lastPowerState) // Are we "On"?
    {
      myIMU.modeSleep(); // Put BNO to sleep
    }
    else
    {
      myIMU.modeOn(); // Turn BNO back on
    }

    lastPowerState ^= 1; // Invert lastPowerState (using ex-or)
  }
}
