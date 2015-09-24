#include <Wire.h>
#include <MMA_7455.h>

MMA_7455 accel = MMA_7455();

int8_t x8, y8, z8;
bool int1, int2;
bool xlvl, ylvl, zlvl;

/* Level threshold value */
#define TH_LEVEL  (32)

void setup()
{
  /* Set serial baud rate */
  Serial.begin(9600);
  /* Set accelerometer sensibility */
  /* Note: Level and pulse detections run at 8g */
  accel.setSensitivity(8);
  if(accel.getSensitivity() != 8)   Serial.println("Sensitivity failure");
  /* Set main mode */
  accel.setMode(level);
  if(accel.getMode() != level)     Serial.println("Set mode failure");
  /* Set axis offsets */
  /* Note: the offset is hardware specific
   * and defined thanks to the auto-calibration example. */
  accel.setAxisOffset(2, 38, -27);
  /* Enable the detection on each axis */
  accel.enableDetectionXYZ(true, true, true);
  /* Set level polarity to positive (not free fall) */
  accel.setLevelPolarity(lvl_positive);
  /* Set threshold to absolute which means TH limit is positive */
  accel.setThresholdMode(th_absolute);
  /* Set threshold limit for level detection */
  accel.setLevelThresholdLimit(TH_LEVEL); /* TH_LEVEL x 16N/g = g */
  /* Set interrupt mode with INT1 as level and INT2 as pulse */
  accel.setInterruptMode(level_pulse);
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
  /* Check if level threshold is reached */
  if(z8 > TH_LEVEL) Serial.print("\tTRIGGER");
  
  /* Get interrupts */
  accel.getInterrupt(&int1, &int2);
  /* Get level detection status on each axis */
  accel.getLevelDetection(&xlvl, &ylvl, &zlvl);
  /* Display the triggered interrupt */
  if(int1 || int2)
  {
    if(int1) Serial.print("\tISR1");
    if(int2) Serial.print("\tISR2");
    /* Clear interrupts */
    accel.clearInterrupt();
  }
  /* Display which axis triggered the interrupt */
  if(xlvl)  Serial.print("\tXlvl");
  if(ylvl)  Serial.print("\tYlvl");
  if(zlvl)  Serial.print("\tZlvl");
  
  Serial.println();
}
