// MMA_7455.cpp - 3 Axis Accelerometer Library
// Moritz Kemper, IAD Physical Computing Lab
// moritz.kemper@zhdk.ch
// ZHdK, 03/04/2012
// Released under Creative Commons Licence

#include "MMA_7455.h"

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
      selected = MCTL_GLVL_2G;
      break;
    case 4:
      selected = MCTL_GLVL_4G;
      break;
    case 8:
      selected = MCTL_GLVL_8G;
      break;
    default:
      selected = MCTL_GLVL_2G;
      break;
  }
  val = readReg(MCTL_OFF);
  val &= ~MCTL_GLVL_MASK;
  val |= selected & MCTL_GLVL_MASK;
  writeReg(MCTL_OFF, val);
  return;
}

int MMA_7455::getSensitivity(void)
{
  int     selected = 0;
  uint8_t val      = 0;
  
  val = readReg(MCTL_OFF);
  val &= MCTL_GLVL_MASK;
  switch(val)
  {
    case MCTL_GLVL_2G:
      selected = 2;
      break;
    case MCTL_GLVL_4G:
      selected = 4;
      break;
    case MCTL_GLVL_8G:
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
    case standby:
      selected = MCTL_MOD_STBY;
      break;
    case mesure:
      selected = MCTL_MOD_MSMT;
      break;
    case level:
      selected = MCTL_MOD_LVL;
      break;
    case pulse:
      selected = MCTL_MOD_PLS;
      break;
    default:
      selected = MCTL_MOD_MSMT;
      break;
  }
  val = readReg(MCTL_OFF);
  val &= ~MCTL_MOD_MASK;
  val |= selected & MCTL_MOD_MASK;
  writeReg(MCTL_OFF, val);
  return;
}

MODE MMA_7455::getMode(void)
{
  MODE    selected = none;
  uint8_t val      = 0;
  
  val = readReg(MCTL_OFF);
  val &= MCTL_MOD_MASK;
  switch(val)
  {
    case MCTL_MOD_STBY:
      selected = standby;
      break;
    case MCTL_MOD_MSMT:
      selected = mesure;
      break;
    case MCTL_MOD_LVL:
      selected = level;
      break;
    case MCTL_MOD_PLS:
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
  val = readReg(MCTL_OFF);
  if(enable)
  {
    val |= MCTL_STON;
  }
  else
  {
    val &= ~MCTL_STON;
  }
  writeReg(MCTL_OFF, val);
  return;
}

void MMA_7455::enableDetectionXYZ(bool x, bool y, bool z)
{
  uint8_t val      = 0;
  val = readReg(CTL1_OFF);
  
  /* enable/disbale detection on X */
  if(x) val &= ~CTL1_XDA_DIS;
  else  val |= CTL1_XDA_DIS;
  /* enable/disbale detection on Y */
  if(y) val &= ~CTL1_YDA_DIS;
  else  val |= CTL1_YDA_DIS;
  /* enable/disbale detection on Z */
  if(z) val &= ~CTL1_ZDA_DIS;
  else  val |= CTL1_ZDA_DIS;
  
  writeReg(CTL1_OFF, val);
  return;
}

void MMA_7455::setLevelPolarity(LEVEL_MODE mode)
{
  uint8_t val      = 0;
  val = readReg(CTL2_OFF);
  switch(mode)
  {
    case lvl_positive:
      val &= ~CTL2_LDPL;
      break;
    case lvl_freefall:
      val |= CTL2_LDPL;
      break;
    default:
      val &= ~CTL2_LDPL;
      break;
  }
  writeReg(CTL2_OFF, val);
  return;
}

void MMA_7455::setLevelPolarity(unsigned int mode)
{
  uint8_t val      = 0;
  val = readReg(CTL2_OFF);
  switch(mode)
  {
    case 0:
      val &= ~CTL2_LDPL;
      break;
    case 1:
      val |= CTL2_LDPL;
      break;
    default:
      val &= ~CTL2_LDPL;
      break;
  }
  writeReg(CTL2_OFF, val);
  return;
}

void MMA_7455::setThresholdMode(TH_MODE mode)
{
  uint8_t val      = 0;
  val = readReg(CTL1_OFF);
  switch(mode)
  {
    case th_absolute:
      val &= ~CTL1_THOPT;
      break;
    case th_signed:
      val |= CTL1_THOPT;
      break;
    default:
      val &= ~CTL1_THOPT;
      break;
  }
  writeReg(CTL1_OFF, val);
  return;
}

void MMA_7455::setThresholdMode(unsigned int mode)
{
  uint8_t val      = 0;
  val = readReg(CTL1_OFF);
  switch(mode)
  {
    case 0:
      val &= ~CTL1_THOPT;
      break;
    case 1:
      val |= CTL1_THOPT;
      break;
    default:
      val &= ~CTL1_THOPT;
      break;
  }
  writeReg(CTL1_OFF, val);
  return;
}

void MMA_7455::setLevelThresholdLimit(int8_t limit)
{
  uint8_t val = readReg(CTL1_OFF);
  if(val & CTL1_THOPT)
  {
    /* signed value of 8-bit */
    /* range: [-128; +127] */
    val = (uint8_t)limit;
    val &= LDTH_MASK;
    if(( (val & LDTH_SIGN) && limit < 0) ||
       (!(val & LDTH_SIGN) && limit >= 0))
    {
      /* capture valid condition */
    }
    else
    {
      /* force bad casting */
      val = (limit >= 0) ?
            (val & ~LDTH_SIGN) :
            (val | LDTH_SIGN);
    }
    writeReg(LDTH_OFF, val);
  }
  else
  {
    /* unsigned value of 7-bit */
    /* range: [0; +127] */
    val = (uint8_t)limit;
    val &= ~LDTH_SIGN;
    val &= LDTH_MASK;
    writeReg(LDTH_OFF, val);
  }
  return;
}

void MMA_7455::setPulsePolarity(PULSE_MODE mode)
{
  uint8_t val      = 0;
  val = readReg(CTL2_OFF);
  switch(mode)
  {
    case pls_positive:
      val &= ~CTL2_PDPL;
      break;
    case pls_negative:
      val |= CTL2_PDPL;
      break;
    default:
      val &= ~CTL2_PDPL;
      break;
  }
  writeReg(CTL2_OFF, val);
  return;
}

void MMA_7455::setPulsePolarity(unsigned int mode)
{
  uint8_t val      = 0;
  val = readReg(CTL2_OFF);
  switch(mode)
  {
    case 0:
      val &= ~CTL2_PDPL;
      break;
    case 1:
      val |= CTL2_PDPL;
      break;
    default:
      val &= ~CTL2_PDPL;
      break;
  }
  writeReg(CTL2_OFF, val);
  return;
}

void MMA_7455::setPulseThresholdLimit(uint8_t limit)
{
  uint8_t val = limit;
  val &= PDTH_MASK;
  writeReg(PDTH_OFF, val);
  return;
}

void MMA_7455::setPulseDuration(uint8_t time)
{
  uint8_t val = time;
  val &= PW_MASK;
  writeReg(PW_OFF, val);
  return;
}

void MMA_7455::setPulseLatency(uint8_t time)
{
  uint8_t val = time;
  val &= LT_MASK;
  writeReg(LT_OFF, val);
  return;
}

void MMA_7455::setPulseDuration2(uint8_t time)
{
  uint8_t val = time;
  val &= TW_MASK;
  writeReg(TW_OFF, val);
  return;
}

int8_t MMA_7455::readAxis8(char axis)
{
  uint8_t reg   = 0;
  uint8_t mask  = 0;
  uint8_t u_val = 0;
  int8_t  s_val = 0;
  int     sens  = 0;
  
  sens = getSensitivity();
  switch(axis)
  {
    case 'x':
    case 'X':
      reg = XOUT8_OFF; mask = XOUT8_MASK;
      break;
    case 'y':
    case 'Y':
      reg = YOUT8_OFF; mask = YOUT8_MASK;
      break;
    case 'z':
    case 'Z':
      reg = ZOUT8_OFF; mask = ZOUT8_MASK;
      break;
    default:
      return 0;
  }
  
  u_val = readReg(reg) & mask;
  s_val += (int8_t)u_val;
  
  return s_val;
}

float MMA_7455::readAxis8g(char axis)
{
  float  f_val = 0;
  int8_t s_val = 0;
  int    sens  = 0;
  
  sens  = getSensitivity();
  s_val = readAxis8(axis);
  
  /* convert N to g/s */
  f_val = 2.0*(float)sens;
  f_val /= 256.0;
  f_val *= (float)s_val;
  
  return f_val;
}

int16_t MMA_7455::readAxis10(char axis)
{
  uint8_t  reg[2]  = {0};
  uint8_t  mask[2] = {0};
  uint16_t u_val   = 0;
  int16_t  s_val   = 0;
  int      sens    = 0;
  
  sens = getSensitivity();
  switch(axis)
  {
    case 'x':
    case 'X':
      reg[0] = XOUTL_OFF; mask[0] = XOUTL_MASK;
      reg[1] = XOUTH_OFF; mask[1] = XOUTH_MASK;
      break;
    case 'y':
    case 'Y':
      reg[0] = YOUTL_OFF; mask[0] = YOUTL_MASK;
      reg[1] = YOUTH_OFF; mask[1] = YOUTH_MASK;
      break;
    case 'z':
    case 'Z':
      reg[0] = ZOUTL_OFF; mask[0] = ZOUTL_MASK;
      reg[1] = ZOUTH_OFF; mask[1] = ZOUTH_MASK;
      break;
    default:
      return 0;
  }
  
  u_val  = readReg(reg[0]) & mask[0];
  u_val |= (readReg(reg[1]) & mask[1]) << 8;
  s_val += (int16_t)u_val;
  
  /* fill of ones if negative value
   * to make it valid in 16 bit format */
  if(s_val & (1 << 9))
  {
    s_val |= 0xFC00;
  }
  
  return s_val;
}

float MMA_7455::readAxis10g(char axis)
{
  float   f_val = 0;
  int16_t s_val = 0;
  int     sens  = 0;
  
  sens  = getSensitivity();
  s_val = readAxis10(axis);
  
  /* convert N to g/s */
  f_val = 2.0*(float)sens;
  f_val /= 1024.0;
  f_val *= (float)s_val;
  
  return f_val;
}

void MMA_7455::setAxisOffset(int16_t x, int16_t y, int16_t z)
{
  writeReg(XOFFL_OFF, x & XOFFL_MASK);
  writeReg(XOFFH_OFF, (x >> 8) & XOFFH_MASK);
  
  writeReg(YOFFL_OFF, y & YOFFL_MASK);
  writeReg(YOFFH_OFF, (y >> 8) & YOFFH_MASK);
  
  writeReg(ZOFFL_OFF, z & ZOFFL_MASK);
  writeReg(ZOFFH_OFF, (z >> 8) & ZOFFH_MASK);
  
  return;
}

void MMA_7455::getAxisOffset(int16_t* x, int16_t* y, int16_t* z)
{
  if(x == NULL || y == NULL || z == NULL)   return;
  
  *x = readReg(XOFFL_OFF) & XOFFL_MASK;
  *x |= (readReg(XOFFH_OFF) & XOFFH_MASK) << 8;
  *x |= (*x & (1 << 10)) ? 0xF800 : 0x0000;
  
  *y = readReg(YOFFL_OFF) & YOFFL_MASK;
  *y |= (readReg(YOFFH_OFF) & YOFFH_MASK) << 8;
  *y |= (*y & (1 << 10)) ? 0xF800 : 0x0000;
  
  *z = readReg(ZOFFL_OFF) & ZOFFL_MASK;
  *z |= (readReg(ZOFFH_OFF) & ZOFFH_MASK) << 8;
  *z |= (*z & (1 << 10)) ? 0xF800 : 0x0000;
  
  return;
}

uint8_t MMA_7455::readReg(uint8_t reg)
{
  uint8_t buff;
  Wire.beginTransmission(MMA_7455_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.beginTransmission(MMA_7455_ADDRESS);
  Wire.requestFrom(MMA_7455_ADDRESS, 1);
  if(Wire.available())
  {
    buff = Wire.read();
  }
  Wire.endTransmission();
  return buff;
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
