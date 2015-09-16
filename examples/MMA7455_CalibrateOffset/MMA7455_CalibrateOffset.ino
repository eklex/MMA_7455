/*
  Calibration step for Parallax MMA7455 3-axis accelerometer module (Parallax #28526).
  
  ------------

  This sketch REQUIRES the use of a third-party library, MMA_7455. 
   Refer to the 'MMA7455_Demo' sketch for important setup information
   for this library.
   
   ------------
  
  Optional: Run this sketch first to obtain position offsets. Place the 
  sensor on a flat surface so that all pins face down. Run this sketch 
  and display the Arduino Serial Monitor window. Be sure communications
  is set to 9600 baud.
  
  Note the average of the return X, Y, and Z values, including the - sign
  character.
  
  The desired output should be as close as possible to 0, 0, 63.
  This indicates 0g for the X and Y axes, and 1g for the Z axis.
  
  An EXAMPLE result might be 
  
  X: = -4   Y: = -14   Z: = 70
  
  Use YOUR values to calibrate the offset in the  MMA744_Demo sketch.
  Look for the following line of code in the MMA744_Demo sketch:
  
  accel.calibrateOffset(0, 0, 0);
  
  and set the difference of YOUR values for the X, Y, Z parameters.
  Using the previous example:
  
  accel.calibrateOffset(4, 14, -7);
  
  Remember: Use your own values to set the offset, not the ones
  in the example.
  
  Note: It is normal for the sensor to report slight variations of one
  or two points, even when it is not in motion.

*/

#include <Wire.h>
#include <MMA_7455.h>

MMA_7455 accel = MMA_7455();

char xVal, yVal, zVal;

void setup() {
  Serial.begin(9600);
  accel.initSensitivity(2);           
  accel.calibrateOffset(0, 14, -12); 
}

void loop() {
  xVal = accel.readAxis('x');
  yVal = accel.readAxis('y');
  zVal = accel.readAxis('z');
  Serial.print("X: = ");
  Serial.print(xVal, DEC);
  Serial.print("   Y: = ");
  Serial.print(yVal, DEC);
  Serial.print("   Z: = ");
  Serial.println(zVal, DEC);
  delay(100);
}
