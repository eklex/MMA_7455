/**
 *  Name:      MMA7455_InterruptPulse
 *  Desc.:     Illustrate pulse mode and interrupt use
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
 *    When one of the 3 axis goes above 2g
 *    within 50ms, the interrupt 1 is triggered.
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

int8_t x8, y8, z8;
bool int1;
bool xpls, ypls, zpls;

void setup()
{
  /* Set serial baud rate */
  Serial.begin(9600);
  /* Start accelerometer */
  accel.begin();
  /* Set accelerometer sensibility */
  /* Note: Level and pulse detections run at 8g */
  accel.setSensitivity(8);
  /* Verify sensibility - optional */
  if(accel.getSensitivity() != 8)   Serial.println("Sensitivity failure");
  /* Set accelerometer mode */
  accel.setMode(pulse);
  /* Verify accelerometer mode - optional */
  if(accel.getMode() != pulse)      Serial.println("Set mode failure");
  /* Set axis offsets */
  /* Note: the offset is hardware specific
   * and defined thanks to the auto-calibration example. */
  accel.setAxisOffset(0, 0, 0);
  /* Enable the detection on each axis */
  accel.enableDetectionXYZ(true, true, true);
  /* Set pulse polarity to positive */
  accel.setPulsePolarity(pls_positive);
  /* Set threshold to absolute which means TH limit is positive */
  accel.setThresholdMode(th_absolute);
  /* Set threshold limit for pulse detection */
  accel.setPulseThresholdLimit(32); /* 32 / 16N/g = 2g */
  /* Set max pulse duration */
  accel.setPulseDuration(100); /* 100 * 0.5ms = 50ms */
  /* Enable Interrupt pins */
  /* Note: This function disables pin DRDY and enable pin INT1 */
  accel.enableInterruptPins(true);
  /* Set interrupt mode with INT1 & INT2 as single pulse */
  accel.setInterruptMode(pulse_pulse);
  /* Sanity clear of interrupts */
  accel.clearInterrupt();
}

void loop()
{
  /* Get axis values */
  x8 = accel.readAxis8('x');
  y8 = accel.readAxis8('y');
  z8 = accel.readAxis8('z');
  /* Display the values */
  Serial.print("X8: ");   Serial.print(x8, DEC);
  Serial.print("\tY8: "); Serial.print(y8, DEC);
  Serial.print("\tZ8: "); Serial.print(z8, DEC);
  
  /* Get interrupts */
  accel.getInterrupt(&int1, NULL);
  /* Get pulse detection status on each axis */
  accel.getPulseDetection(&xpls, &ypls, &zpls);
  /* Display the triggered interrupt */
  if(int1)
  {
    Serial.print("\tISR1");
    /* Clear interrupts */
    accel.clearInterrupt();
  }
  /* Display which axis triggered the interrupt */
  if(xpls)  Serial.print("\tXpls");
  if(ypls)  Serial.print("\tYpls");
  if(zpls)  Serial.print("\tZpls");
  
  Serial.println();
  delay(500);
}