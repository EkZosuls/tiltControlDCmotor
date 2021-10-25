/*
 * Modified by Aleks Zosuls for a demonstration of a tilt sensor used to control 
 * the speed and direction of a brushed motor with an H Bridge.
 * Note that this code can be generalized and used as an example of 
 * how to code an HBridge that has polarity reversal and a variable intensity 
 * based on a senor value. In this case the sensor is a built in IMU on a fancy
 * Arduino called a Nano 33 BLE sense.
 * 
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
  Serial.begin(9600); //initialize the serial connection to the computer
  //while (!Serial);
  delay(2000);
  //Serial.println("Started");
  pinMode(2,OUTPUT);  //make pin 2 an output and write it low.
  digitalWrite(2,LOW);  /*I use pin 2 to ENABLE the H bridge module
   I set pin 2 to :LOW to disable the H Bridge for safety. When I want to run the
   thing on the H bridge, I make pin 2 HIGH right before I call analogWrites. 
   This adds a bit of redundancy where I need to call two functions before my
   thing (in this case a motor) will be able to run.
                        
*/
  if (!IMU.begin()) {  //if the IMU does not initialize send a message and sit in a loop for eva
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
//get the default sample rate of the accelerometer and write it to the computer.
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();

}
  
void loop() {
  float x, y, z;   //declare these variables for the acceleration readings
  int xI, pw;     //I use xI to scale the x acceleration to an integer
                  //pw is the pulse width modulation value to send to analogWrite

  if (IMU.accelerationAvailable()) {  //if there is acceleration data in the queue...
    IMU.readAcceleration(x, y, z);    //read it! otherwise take another lap around the loop

    Serial.print(x);  //print out the x acceleration for debug purposes.
    Serial.print('\t'); //print a tab to separate the next output
    //Serial.print(y);
    //Serial.print('\t');
    //Serial.println(z);
    if (x>= 0.10){    //test the conditions. If x>0.1, run forwards
       xI = x * 100;  //scale the g's by 100 to get integers for the map function
      Serial.print(xI);  //print out the scaling to make sure it is sane.
       pw = map(xI, 0, 100, 0, 255);  //map the scaled g's to the 0-255 analogWrite range
      digitalWrite(2,HIGH); //enable our H bridge
       /* this H bridge has two 'halfs'. Each half controls the switching from Low to 
       high on this particular brand of H bridge module. If I keep one half at zero it 
       is like its output being connected to ground. Then if I make the other half have a 
       PWM then it will pulse high with a strength relative to the pw variable.
       Conversely if I switch the half that is kept at zero with the one that is getting the 
       non zero analogWrite then the polarity will switch and if this was a motor or TEC the
       current will flow the opposite direction reversing the direction or heat flow.
      */
      analogWrite(4, 0);   // keep this channel low. Call pin 4 the negative half
      analogWrite(3, pw); // put the PWM value on this pin. We can say the pin 3 is the + half
      Serial.print('\t'); // put in another tab and print out the value of pw for debugging
      Serial.print(pw);
      Serial.println(" pos"); // send a 'pos' meanin we are running positive
      
    }
        if (x<= -0.10){ //if the accleration is negative and less than some threshold...
       xI = x * -100; //negate and scale to get the value of xI positive for the map fcn.
      Serial.print(xI);  
       pw = map(xI, 0, 100, 0, 255); //do tht mapping
      digitalWrite(2,HIGH); //Enable our H bridge
      analogWrite(3, 0);    //now pin three side of the H bridge is acting as the negative half
      analogWrite(4, pw); // output a PWM to the pin 4 side of the bridge to reverse the device.
      Serial.print('\t');
      Serial.print(pw);
      Serial.println(" neg"); //tell the computer we are running negative.
      
    }
    if (x>-0.1 && x<0.1) { //if we are in a deadband around zero... disable the motor and Set all H bridges to zero
          digitalWrite(2,LOW);
          analogWrite(3, 0);
          analogWrite(4, 0);
          Serial.println(" neu");
  }
  delay(20); // throw in a delay to slow this down. Note it is somewhat dependent on the IMU sample rate anyhoo.
  }
}
