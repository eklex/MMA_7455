/**
 *  Name:      MMA7455_AutoCalibration
 *  Desc.:     Determine the natural offset of the accelerometer
 *  Author:    Alexandre Boni
 *  Created:   2015/09/16
 *  Modified:  2015/10/07
 *  Version:   0.2
 *  IDE:       Arduino 1.6.5-r2
 *             ParticleDev 1.0.15
 *  License:   GPLv2
 *
 *  Release:
 *    0.2
 *          Add support for SPI.
 *          Tested on Arduino Mini Pro 3.3v
 *          and Particle Photon.
 *    0.1
 *          Creation of this code
 *
 *  Notes:
 *    Place the accelerometer a flat surface in order to
 *    get 0g on the X and Y axis and 1g on the Z axis.
 *    Run the auto-calibration code to get the natural
 *    offset on each axis.
 *    The accelerometer must stay still while the program
 *    is running.
 *    When the program stops, save the values for each axis.
 *    These values belong to the function setAxisOffset(x,y,z).
 *    For example, in sample code MMA7455_Demo, write your values
 *    in setAxisOffset(x,y,z), and run the application. The output
 *    should be close to X = 0, Y = 0, Z = 64.
 *
 *    It is expected for the sensor to report slight variations of one
 *    or two points, even when the accelerometer is not in motion.
 *
 *    If the auto-calibration runs forever and does not find the offsets,
 *    try to decrease the value of lpratio.
 *
 */

#if defined(ARDUINO)
/* Mandatory includes for Arduino */
#include <SPI.h>
#include <Wire.h>
#endif

#include <MMA_7455.h>

/* Case 1: Accelerometer on the I2C bus (most common) */
MMA_7455 accel = MMA_7455(i2c_protocol);
/* Case 2: Accelerometer on the SPI bus with CS on pin 2 */
// MMA_7455 accel = MMA_7455(spi_protocol, A2);

int16_t xavg, yavg, zavg;
int16_t x10, y10, z10;
int16_t xc = 0, yc = 0, zc = 0;
float lpratio = 0.6; /* between 0 and 1 */

void setup()
{
  /* Set serial baud rate */
  Serial.begin(9600);
  /* Start accelerometer */
  accel.begin();
  /* Set accelerometer sensibility to 2g */
  accel.setSensitivity(2);
  /* Verify sensibility - optional */
  if(accel.getSensitivity() != 2)   Serial.println("Sensitivity failure");
  /* Set accelerometer mode */
  accel.setMode(measure);
  /* Verify accelerometer mode - optional */
  if(accel.getMode() != measure)    Serial.println("Set mode failure");
  /* Reset current axis offsets */
  accel.setAxisOffset(0, 0, 0);

  Serial.print("MMA7455 Auto-calibration ----\n");
  Serial.print("-----------------------------\n");
  Serial.print("The calibration will look for\n");
  Serial.print("the following idle values:\n");
  Serial.print("X: 0\tY: 0\tZ: 64\n");
  Serial.print("-----------------------------\n");
  Serial.print("Be patient,\n");
  Serial.print("good things take time...\n");
  Serial.print("-----------------------------\n");
  delay(2000);
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
    Serial.print("Copy this function in\nyour programs:\n");
    Serial.print("  setAxisOffset("); Serial.print(xc, DEC);
    Serial.print(", "); Serial.print(yc, DEC);
    Serial.print(", "); Serial.print(zc, DEC);
    Serial.print(");\n");
    Serial.print("-----------------------------\n");
    Serial.print("DONE. -----------------------\n");
    while(1);
  }
  
  delay(200);
}
