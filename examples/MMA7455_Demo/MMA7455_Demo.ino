/**
 *  Name:      MMA7455_Demo
 *  Desc.:     Demo code for the MMA_7455 Library
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
 *          Creation of this code.
 *
 *  Notes:
 *    The code displays the 3 axis values in
 *    raw format on 10-bit and in g.
 *
 *    The library is designed to be interfaced
 *    with the Freescale 3 axis accelerometer
 *    MMA7455 on the I2C bus. The expected
 *    I2C address is hard-coded in the library (0x1D).
 *
 *    To find more details on the MMA7455, read the
 *    documentation provided along with the library.
 *
 *    The code expects to have the axis offset
 *    configured. To get the offset of your
 *    accelerometer, run MMA7455_AutoCalibration.
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

int16_t x10, y10, z10;
float   xg, yg, zg;

void setup()
{
  /* Set serial baud rate */
  Serial.begin(9600);
  /* Start accelerometer */
  accel.begin();
  /* Set accelerometer sensibility */
  accel.setSensitivity(2);
  /* Verify sensibility - optional */
  if(accel.getSensitivity() != 2)   Serial.println("Sensitivity failure");
  /* Set accelerometer mode */
  accel.setMode(measure);
  /* Verify accelerometer mode - optional */
  if(accel.getMode() != measure)    Serial.println("Set mode failure");
  /* Set axis offsets */
  /* Note: the offset is hardware specific
   * and defined thanks to the auto-calibration example. */
  accel.setAxisOffset(0, 0, 0);
}

void loop()
{
  /* Get 10-bit axis raw values */
  x10 = accel.readAxis10('x');
  y10 = accel.readAxis10('y');
  z10 = accel.readAxis10('z');
  /* Get 10-bit axis values in g */
  xg  = accel.readAxis10g('x');
  yg  = accel.readAxis10g('y');
  zg  = accel.readAxis10g('z');
  
  /* Display current axis values */
  Serial.print("X: ");    Serial.print(x10, DEC);
  Serial.print("\tY: ");  Serial.print(y10, DEC);
  Serial.print("\tZ: ");  Serial.print(z10, DEC);
  Serial.print("\tXg: "); Serial.print(xg, DEC);
  Serial.print("\tYg: "); Serial.print(yg, DEC);
  Serial.print("\tZg: "); Serial.print(zg, DEC);
  Serial.println();
  delay(500);
}
