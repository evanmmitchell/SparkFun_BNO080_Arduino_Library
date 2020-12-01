/*
  Using the BNO085 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

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

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("BNO085 Read Example");

    Wire.begin();

    myIMU.begin();

    Wire.setClock(400000); //Increase I2C data rate to 400kHz

    myIMU.enableAccelerometer(50000);    //We must enable the accel in order to get MEMS readings even if we don't read the reports.
    myIMU.enableRawAccelerometer(50000); //Send data update every 50ms
    myIMU.enableGyro(50000);
    myIMU.enableRawGyro(50000);
    myIMU.enableMagnetometer(50000);
    myIMU.enableRawMagnetometer(50000);

    Serial.println(F("Raw MEMS readings enabled"));
    Serial.println(F("Output is: (accel) x y z (gyro) x y z (mag) x y z"));
}

void loop()
{
    //Look for reports from the IMU
    if (myIMU.dataAvailable() == true)
    {
        int x = myIMU.getRawAccelX();
        int y = myIMU.getRawAccelY();
        int z = myIMU.getRawAccelZ();

        Serial.print(x);
        Serial.print("\t");
        Serial.print(y);
        Serial.print("\t");
        Serial.print(z);
        Serial.print("\t");

        int gx = myIMU.getRawGyroX();
        int gy = myIMU.getRawGyroY();
        int gz = myIMU.getRawGyroZ();

        Serial.print(gx);
        Serial.print("\t");
        Serial.print(gy);
        Serial.print("\t");
        Serial.print(gz);
        Serial.print("\t");

        int mx = myIMU.getRawMagX();
        int my = myIMU.getRawMagY();
        int mz = myIMU.getRawMagZ();

        Serial.print(mx);
        Serial.print("\t");
        Serial.print(my);
        Serial.print("\t");
        Serial.print(mz);
        Serial.print("\t");

        Serial.println();
    }
}
