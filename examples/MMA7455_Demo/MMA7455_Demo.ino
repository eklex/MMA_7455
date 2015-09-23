/*
  Demonstration code for the Parallax MMA7455 3-axis accelerometer, #28526
  
  Optional: Before use run the MMA7455_CalibrateOffset sketch to obtain the
  offset values. The offsets are subtracted from those received from the
  accelerometer, so that the return values are as close to X=0, Y=0, X=63
  as possible, when the device is stationary. 0 means 0g; 63 means 1g.
  
  ------------

  This sketch REQUIRES the use of a third-party library, MMA_7455. 
  It is available for download from the following source.

  https://code.google.com/p/mma-7455-arduino-library/downloads/detail?name=MMA_7455_UNO.zip

  Once downloaded, unzip the files and place them into your Arduino 'libraries' folder. 
  This folder is located within your Arduino sketch directory; on Windows this is usually 
  My Documents\Arduino. If there is no 'libraries' folder within the sketch 
  directory you must create one.

  For your convenience, the MMA_7455 library is also included in the download with this 
  demonstration sketch. Prior to use you MUST move the 'MMA_7455' library folder into 
  your Arduino sketch 'libraries' folder.

  Important! After moving the MMA_7455 library to the 'libraries' folder
  you MUST restart the Arduino IDE.
  
  This example code is for the Arduino Uno and direct compatible boards. 
  It has not been tested, nor designed for, other Arduino boards, including
  the Arduino Due.
  
  ------------

  Connections:
  MMA7455      Arduino
  GND          GND
  VDD          5V
  DATA*        Analog pin A4 (I2C SDA)
  CLK*         Analog pin A5 (I2C CLK)
  
  * Also add 1 each 4.7K resistor between the DATA and CLK pins and
  5V, as noted in the 28526-MMA7455-3-Axis-Accelerometer documentation.
  (Your accelerometer may work without these resistors, but for 
  consistent results they should be included.)
  
  Note: Refer to the 28526-MMA7455-3-Axis-Accelerometer documentation 
  for the pin locations. The pins are not definitively marked on the 
  device itself.
  
  Be VERY careful to not reverse the GND and VDD connections!  
  
  ------------
   
  To run this demo sketch, upload it to your Arduino, and display the
  Serial Monitor window. Be sure communications speed is set to 9600 baud.
  
  Return values for each of the X, Y, and Z axes are from -127 to 127,
  where (at 2g sensitivity mode) 0 = 0g, 63 = 1g, and -63 = -1g.
  
*/

#include <Wire.h>               //Include the Wire library
#include <MMA_7455.h>           //Include the MMA_7455 library

MMA_7455 accel = MMA_7455();    // Make MMA7455 object

int16_t xavg, yavg, zavg;
int16_t x10, y10, z10;

void setup() {
  Serial.begin(9600);           // Use the Serial Monitor window at 9600 baud
  
  // Set the g force sensitivity: 2=2g, 4=4g, 8-8g
  accel.setSensitivity(2);
  
  accel.setMode(mesure);
  
 // Provide oiffset values so that sensor displays 0, 0, 63
 //  (or close to it) when positioned on a flat surface, all pins
 //  facing down
 
 // Update the numbers with your own values from the MMA7455_CalibrateOffset sketch.
  accel.setAxisOffset(0, 40, -25);
}

void loop() {
 
  // Get the X, Y, anx Z axis values from the device
  x10 = accel.readAxis10('x');   // Read 10-bit X Axis
  y10 = accel.readAxis10('y');   // Read 10-bit Y Axis
  z10 = accel.readAxis10('z');   // Read 10-bit Z Axis
  
  xavg = xavg*0.5 + x10*0.5;
  yavg = yavg*0.5 + y10*0.5; 
  zavg = zavg*0.5 + z10*0.5;
  
  // Display them in the Serial Monitor window.
  Serial.print("X: ");   Serial.print(xavg, DEC);
  Serial.print("\tY: "); Serial.print(yavg, DEC);
  Serial.print("\tZ: "); Serial.println(zavg, DEC);
  delay(1000);
}