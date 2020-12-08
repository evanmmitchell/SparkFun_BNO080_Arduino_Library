/*
  Using the BNO085 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to calibrate the sensor. See document 1000-4044.

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
bool calibrationSaved = false;

void setup()
{
  Serial.begin(115200);
    Serial.println();
  Serial.println("BNO085 Read Example");

  Wire.begin();

  myIMU.begin();

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  //Enable dynamic calibration for accel, gyro, and mag
  myIMU.calibrateAll(); //Turn on cal for Accel, Gyro, and Mag

  //Enable Game Rotation Vector output
  myIMU.enableGameRotationVector(50000); //Send data update every 50ms as specified in calibration procedures

  //Enable Magnetic Field output
  myIMU.enableMagnetometer(50000); //Send data update every 50ms as specified in calibration procedures

  //Once magnetic field is 2 or 3, run the Save DCD Now command
  Serial.println(F("Calibrating. Press 's' to save to flash"));
  while (!calibrationSaved)
  {
    //Look for reports from the IMU
    if (myIMU.dataAvailable())
    {
      float x = myIMU.getMagX();
      float y = myIMU.getMagY();
      float z = myIMU.getMagZ();
      byte accuracy = myIMU.getMagAccuracy();

      float quatI = myIMU.getQuatI();
      float quatJ = myIMU.getQuatJ();
      float quatK = myIMU.getQuatK();
      float quatReal = myIMU.getQuatReal();
      byte sensorAccuracy = myIMU.getQuatAccuracy();

      Serial.print(x, 2);
      Serial.print(F(","));
      Serial.print(y, 2);
      Serial.print(F(","));
      Serial.print(z, 2);
      Serial.print(F(","));
      printAccuracyLevel(accuracy);
      Serial.print(F(","));

      Serial.print("\t");

      Serial.print(quatI, 2);
      Serial.print(F(","));
      Serial.print(quatJ, 2);
      Serial.print(F(","));
      Serial.print(quatK, 2);
      Serial.print(F(","));
      Serial.print(quatReal, 2);
      Serial.print(F(","));
      printAccuracyLevel(sensorAccuracy);
      Serial.print(F(","));

      Serial.println();
    }

    if (Serial.available())
    {
      byte incoming = Serial.read();
      if (incoming == 's')
      {
        myIMU.saveCalibration(); //Saves the current dynamic calibration data (DCD) to memory

        //Wait for calibration response, timeout if no response
        for (int i = 100; i > 0; i--)
        {
          if (myIMU.dataAvailable() && myIMU.calibrationComplete())
          {
            Serial.println("Calibration data successfully stored");
            calibrationSaved = true;

            //myIMU.endCalibration(); //Turns off all calibration
            //In general, calibration should be left on at all times. The BNO085
            //auto-calibrates and auto-records cal data roughly every 5 minutes

            return;
          }
          delay(1);
        }
        Serial.println("Calibration data failed to store. Please try again.");
        delay(1000);
      }
    }
  }
}

void loop()
{
}

//Given a accuracy number, print what it means
void printAccuracyLevel(byte accuracyNumber)
{
  if (accuracyNumber == 0) Serial.print(F("Unreliable"));
  else if (accuracyNumber == 1) Serial.print(F("Low"));
  else if (accuracyNumber == 2) Serial.print(F("Medium"));
  else if (accuracyNumber == 3) Serial.print(F("High"));
}
