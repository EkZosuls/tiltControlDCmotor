/*
 * Modified by Aleks Zosuls for a demonstration of a tilt sensor used to control 
 * the speed and direction of a brushed motor with an H Bridge.
  Arduino LSM9DS1 - Simple Accelerometer

  This example reads the acceleration values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  delay(2000);
  //Serial.println("Started");
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
}

void loop() {
  float x, y, z;
  int xI, pw;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    //Serial.print(y);
    //Serial.print('\t');
    //Serial.println(z);
    if (x>= 0.10){
       xI = x * 100;
      Serial.print(xI);  
       pw = map(xI, 0, 100, 0, 255);
      digitalWrite(2,HIGH);
      analogWrite(4, 0);
      analogWrite(3, pw);
      Serial.print('\t');
      Serial.print(pw);
      Serial.println(" pos");
      
    }
        if (x<= -0.10){
       xI = x * -100;
      Serial.print(xI);  
       pw = map(xI, 0, 100, 0, 255);
      digitalWrite(2,HIGH);
      analogWrite(3, 0);
      analogWrite(4, pw);
      Serial.print('\t');
      Serial.print(pw);
      Serial.println(" neg");
      
    }
    if (x>-0.1 && x<0.1) {
          digitalWrite(2,LOW);
          analogWrite(3, 0);
          analogWrite(4, 0);
          Serial.println(" neu");
  }
  delay(20);
  }
}
