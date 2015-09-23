/*
 * Auto-Calibration for Freescale 3-axis Accelerometer MMA-7455.
 * 
 * 
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

  If the autocalibration runs forever and does not find the offsets,
  try to decrease lpratio.

*/

#include <Wire.h>
#include <MMA_7455.h>

MMA_7455 accel = MMA_7455();

int16_t xavg, yavg, zavg;
int16_t x10, y10, z10;
int16_t xc = 0, yc = 0, zc = 0;
float lpratio = 0.6; /* between 0 and 1 */

void setup()
{
  Serial.begin(9600);
  accel.setSensitivity(2);
  if(accel.getSensitivity() != 2)   Serial.println("Sensitivity failure");
  accel.setMode(mesure);
  if(accel.getMode() != mesure)     Serial.println("Set mode failure");
  /* Reset current axis offsets */
  accel.setAxisOffset(0, 0, 0);

  Serial.print("MMA7455 Autocalibration -----\n");
  Serial.print("-----------------------------\n");
  Serial.print("The calibration will look for\n");
  Serial.print("the following idle values:\n");
  Serial.print("X: 0\tY: 0\tZ: 64\n");
  Serial.print("-----------------------------\n");
}

void loop()
{
  /* Get 10-bit axis values */
  x10 = accel.readAxis10('x');
  y10 = accel.readAxis10('y');
  z10 = accel.readAxis10('z');
  /* Low-pass filter */
  xavg = xavg*lpratio + x10*(1-lpratio);
  yavg = yavg*lpratio + y10*(1-lpratio);
  zavg = zavg*lpratio + z10*(1-lpratio);
  /* Display current axis values */
  Serial.print("X: ");   Serial.print(xavg, DEC);
  Serial.print("\tY: "); Serial.print(yavg, DEC);
  Serial.print("\tZ: "); Serial.println(zavg, DEC);
  /* Calculate axis offset */
  xc += -2*x10;
  yc += -2*y10;
  zc += (z10 > 65 ? 0.5*(z10-64) : 0.5*(64-z10));
  accel.setAxisOffset(xc, yc, zc);
  /* Check if offsets found */
  if(xavg == 0 && yavg == 0 && zavg == 64)
  {
    Serial.print("-----------------------------\n");
    Serial.print("Axis Offset:\n");
    accel.getAxisOffset(&xc, &yc, &zc);
    Serial.print("X: ");   Serial.print(xc, DEC);
    Serial.print("\tY: "); Serial.print(yc, DEC);
    Serial.print("\tZ: "); Serial.println(zc, DEC);
    Serial.print("-----------------------------\n");
    Serial.print("DONE. -----------------------\n");
    while(1);
  }
  
  delay(200);
}
