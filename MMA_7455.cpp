// MMA_7455.cpp - 3 Axis Accelerometer Library
// Moritz Kemper, IAD Physical Computing Lab
// moritz.kemper@zhdk.ch
// ZHdK, 03/04/2012
// Released under Creative Commons Licence

#include "MMA_7455.h"

#define MMA_7455_ADDRESS 0x1D //I2C Adsress for the sensor

#define MODE_CTRL_OFF       (0x16)
#define MODE_CTRL_MODE_MASK (0x03 << 0)
#define MODE_CTRL_MODE_STBY (0x00 << 0)
#define MODE_CTRL_MODE_MSMT (0x01 << 0)
#define MODE_CTRL_MODE_LVL  (0x02 << 0)
#define MODE_CTRL_MODE_PLS  (0x03 << 0)
#define MODE_CTRL_GLVL_MASK (0x03 << 2)
#define MODE_CTRL_GLVL_2G   (0x01 << 2) //Set Sensitivity to 2g
#define MODE_CTRL_GLVL_4G   (0x02 << 2) //Set Sensitivity to 4g
#define MODE_CTRL_GLVL_8G   (0x00 << 2) //Set Sensitivity to 8g
#define MODE_CTRL_STON_EN   (0x01 << 4)

#define CTRL_1_OFF          (0x18)
#define CTRL_1_XDA_DIS      (0x01 << 3)
#define CTRL_1_YDA_DIS      (0x01 << 4)
#define CTRL_1_ZDA_DIS      (0x01 << 5)
#define CTRL_1_THOPT        (0x01 << 6)

#define CTRL_2_OFF          (0x19)
#define CTRL_2_LDPL         (0x01 << 0)
#define CTRL_2_PDPL         (0x01 << 1)

#define LVL_THRD_LIM_OFF    (0x1A)
#define LVL_THRD_LIM_MASK   (0xFF)
#define LVL_THRD_LIM_SIGN   (0x01 << 7)

#define PLS_THRD_LIM_OFF    (0x1B)
#define PLS_THRD_LIM_MASK   (0xFF)

#define PLS_DURATION_OFF    (0x1C)
#define PLS_DURATION_MASK   (0xFF)

#define LATENCY_TIME_OFF    (0x1D)
#define PLS2_DURATION_OFF   (0x1E)

#define X_OUT 0x06 //Register for reading the X-Axis
#define Y_OUT 0x07 //Register for reading the Y-Axis
#define Z_OUT 0x08 //Register for reading the Z-Axis

MMA_7455::MMA_7455()
{ 
  Wire.begin();
}

void MMA_7455::setSensitivity(int sensitivity)
{
  uint8_t selected = 0;
  uint8_t val      = 0;
  switch(sensitivity)
  {
    case 2:
      selected = MODE_CTRL_GLVL_2G;
      break;
    case 4:
      selected = MODE_CTRL_GLVL_4G;
      break;
    case 8:
      selected = MODE_CTRL_GLVL_8G;
      break;
    default:
      selected = MODE_CTRL_GLVL_2G;
      break;
  }
  val = readReg(MODE_CTRL_OFF);
  val &= ~MODE_CTRL_GLVL_MASK;
  val |= selected & MODE_CTRL_GLVL_MASK;
  writeReg(MODE_CTRL_OFF, val);
  return;
}

int MMA_7455::getSensitivity(void)
{
  int     selected = 0;
  uint8_t val      = 0;
  
  val = readReg(MODE_CTRL_OFF);
  val &= MODE_CTRL_GLVL_MASK;
  switch(val)
  {
    case MODE_CTRL_GLVL_2G:
      selected = 2;
      break;
    case MODE_CTRL_GLVL_4G:
      selected = 4;
      break;
    case MODE_CTRL_GLVL_8G:
      selected = 8;
      break;
    default:
      selected = 0;
      break;
  }
  return selected;
}

void MMA_7455::setMode(MODE mode)
{
  uint8_t selected = 0;
  uint8_t val      = 0;
  switch(mode)
  {
    case standy:
      selected = MODE_CTRL_MODE_STBY;
      break;
    case mesure:
      selected = MODE_CTRL_MODE_MSMT;
      break;
    case level:
      selected = MODE_CTRL_MODE_LVL;
      break;
    case pulse:
      selected = MODE_CTRL_MODE_PLS;
      break;
    default:
      selected = MODE_CTRL_MODE_MSMT;
      break;
  }
  val = readReg(MODE_CTRL_OFF);
  val &= ~MODE_CTRL_MODE_MASK;
  val |= selected & MODE_CTRL_MODE_MASK;
  writeReg(MODE_CTRL_OFF, val);
  return;
}

MODE MMA_7455::getMode(void)
{
  MODE    selected = 0;
  uint8_t val      = 0;
  
  val = readReg(MODE_CTRL_OFF);
  val &= MODE_CTRL_MODE_MASK;
  switch(mode)
  {
    case MODE_CTRL_MODE_STBY:
      selected = standy;
      break;
    case MODE_CTRL_MODE_MSMT:
      selected = mesure;
      break;
    case MODE_CTRL_MODE_LVL:
      selected = level;
      break;
    case MODE_CTRL_MODE_PLS:
      selected = pulse;
      break;
    default:
      selected = none;
      break;
  }
  return selected;
}

void MMA_7455::setSelfTest(bool enable)
{
  uint8_t val      = 0;
  val = readReg(MODE_CTRL_OFF);
  if(enable)
  {
    val |= MODE_CTRL_STON_EN;
  }
  else
  {
    val &= ~MODE_CTRL_STON_EN;
  }
  writeReg(MODE_CTRL_OFF, val);
  return;
}

void MMA_7455::enableDetectionXYZ(bool x, bool y, bool z)
{
  uint8_t val      = 0;
  val = readReg(CTRL_1_OFF);
  
  /* enable/disbale detection on X */
  if(x) val &= ~CTRL_1_XDA_DIS;
  else  val |= CTRL_1_XDA_DIS;
  /* enable/disbale detection on Y */
  if(y) val &= ~CTRL_1_YDA_DIS;
  else  val |= CTRL_1_YDA_DIS;
  /* enable/disbale detection on Z */
  if(z) val &= ~CTRL_1_ZDA_DIS;
  else  val |= CTRL_1_ZDA_DIS;
  
  writeReg(CTRL_1_OFF, val);
  return;
}

void MMA_7455::setLevelPolarity(LEVEL_MODE mode)
{
  uint8_t val      = 0;
  val = readReg(CTRL_2_OFF);
  switch(mode)
  {
    case positive:
      val &= ~CTRL_2_LDPL;
      break;
    case freefall:
      val |= CTRL_2_LDPL;
      break;
    default:
      val &= ~CTRL_2_LDPL;
      break;
  }
  writeReg(CTRL_2_OFF, val);
  return;
}

void MMA_7455::setLevelPolarity(unsigned int mode)
{
  uint8_t val      = 0;
  val = readReg(CTRL_2_OFF);
  switch(mode)
  {
    case 0:
      val &= ~CTRL_2_LDPL;
      break;
    case 1:
      val |= CTRL_2_LDPL;
      break;
    default:
      val &= ~CTRL_2_LDPL;
      break;
  }
  writeReg(CTRL_2_OFF, val);
  return;
}

void MMA_7455::setThresholdMode(TH_MODE mode)
{
  uint8_t val      = 0;
  val = readReg(CTRL_1_OFF);
  switch(mode)
  {
    case absoluteTH:
      val &= ~CTRL_1_THOPT;
      break;
    case signedTH:
      val |= CTRL_1_THOPT;
      break;
    default:
      val &= ~CTRL_1_THOPT;
      break;
  }
  writeReg(CTRL_1_OFF, val);
  return;
}

void MMA_7455::setThresholdMode(unsigned int mode)
{
  uint8_t val      = 0;
  val = readReg(CTRL_1_OFF);
  switch(mode)
  {
    case 0:
      val &= ~CTRL_1_THOPT;
      break;
    case 1:
      val |= CTRL_1_THOPT;
      break;
    default:
      val &= ~CTRL_1_THOPT;
      break;
  }
  writeReg(CTRL_1_OFF, val);
  return;
}

void MMA_7455::setThresholdLimit(int8_t limit)
{
  uint8_t val = readReg(CTRL_1_OFF);
  if(val & CTRL_1_THOPT)
  {
    /* signed value of 8-bit */
    /* range: [-127; +127] */
    val = (uint8_t)limit;
    val &= LVL_THRD_LIM_MASK;
    if(( (val & LVL_THRD_LIM_SIGN) && limit < 0) ||
       (!(val & LVL_THRD_LIM_SIGN) && limit >= 0))
    {
      /* capture valid condition */
    }
    else
    {
      /* force bad casting */
      val = (limit >= 0) ?
            (val & ~LVL_THRD_LIM_SIGN) :
            (val | LVL_THRD_LIM_SIGN);
    }
    writeReg(LVL_THRD_LIM_OFF, val);
  }
  else
  {
    /* unsigned value of 7-bit */
    /* range: [0; +127] */
    val = (uint8_t)limit;
    val &= ~LVL_THRD_LIM_SIGN
    val &= LVL_THRD_LIM_MASK
    writeReg(LVL_THRD_LIM_OFF, val);
  }
  return;
}

void setPulsePolarity(PULSE_MODE mode)
{
  
}

void MMA_7455::calibrateOffset(float x_axis_offset, float y_axis_offset, float z_axis_offset)
{
  _x_axis_offset = x_axis_offset;
  _y_axis_offset = y_axis_offset;
  _z_axis_offset = z_axis_offset;
}

uint8_t MMA_7455::readAxis(char axis)
{
  uint8_t reg = 0;
  
  if(axis == 'x' || axis == 'X')
  {
    reg = X_OUT;
    _buffer = _x_axis_offset;
  }
  else if(axis == 'y' || axis == 'Y')
  {
    reg = Y_OUT;
    _buffer = _y_axis_offset;
  }
  else if(axis == 'z' || axis == 'Z')
  {
    reg = Z_OUT;
    _buffer = _z_axis_offset;
  }
  else return 0;
  
  _buffer += readReg(reg);
  
  return _buffer;
}



uint8_t MMA_7455::readReg(uint8_t reg)
{
  Wire.beginTransmission(MMA_7455_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.beginTransmission(MMA_7455_ADDRESS);
  Wire.requestFrom(MMA_7455_ADDRESS, 1);
  if(Wire.available())
  {
    _buffer = Wire.read();
  }
  Wire.endTransmission();
  return (uint8_t)_buffer;
}

void MMA_7455::writeReg(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(MMA_7455_ADDRESS);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
  return;
}

void MMA_7455::writeReg(uint8_t reg)
{
  Wire.beginTransmission(MMA_7455_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  return;
}
