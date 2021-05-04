/*
  Using the BNO085 IMU
  By: Evan Mitchell
  SparkFun Electronics
  Date: December 1st, 2020
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to tare the sensor. See document 1000-4045.

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO085 breakout to avoid polling.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 115200 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_BNO085_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
BNO085 myIMU;
bool tared = false;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO085 Read Example");

  Wire.begin();

  myIMU.begin();

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  //Enable Rotation Vector output
  myIMU.enableRotationVector(50000); //Send data update every 50ms

  Serial.println(F("Press 't' to tare"));
}

void loop()
{
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

    Serial.println();
  }

  if (Serial.available())
  {
    byte incoming = Serial.read();
    if (incoming == 't')
    {
      myIMU.tareAllAxes(TARE_ROTATION_VECTOR); //Tares the rotation vector along all axes
      // myIMU.tareZAxis(TARE_ROTATION_VECTOR); //Tares the rotation vector along the Z-axis

      tared = true;
      Serial.println("Tared. Press 'p' to persist");
      delay(1000);
    }
    else if (incoming == 'p' && tared)
    {
      myIMU.persistTare(); //Persists the results of the last tare to flash

      Serial.println("Saved");
      delay(1000);
    }
  }
}
