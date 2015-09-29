/**
 *   Freescale 3-Axis Accelerometer MMA7455 Library designed for Arduino
 *   Copyright (C) 2015  Alexandre Boni
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/**
 *  Name:      MMA_7455
 *  Author:    Alexandre Boni
 *  Created:   2015/09/16
 *  Modified:  2015/09/26
 *  Version:   0.1
 *  IDE:       Arduino 1.6.5-r2
 *  License:   GPLv2
 *
 *  Release:
 *    0.1
 *          Creation of this code from
 *          Moritz Kemper's MMA7455 library
 *          released in 03/04/2012 under
 *          Creative Commons License.
 *          (moritz.kemper@zhdk.ch)
 *
 *
 *
 */

#include "MMA_7455.h"

MMA_7455::MMA_7455(void)
{
  Wire.begin();
}

void MMA_7455::reset(void)
{
  this->writeReg(XOFFL_OFF,  0x00);
  this->writeReg(XOFFH_OFF,  0x00);
  this->writeReg(YOFFL_OFF,  0x00);
  this->writeReg(YOFFH_OFF,  0x00);
  this->writeReg(ZOFFL_OFF,  0x00);
  this->writeReg(ZOFFH_OFF,  0x00);
  this->writeReg(MCTL_OFF,   0x00);
  this->writeReg(INTRST_OFF, 0x03);
  this->writeReg(INTRST_OFF, 0x00);
  this->writeReg(CTL1_OFF,   0x00);
  this->writeReg(CTL2_OFF,   0x00);
  this->writeReg(LDTH_OFF,   0x00);
  this->writeReg(PDTH_OFF,   0x00);
  this->writeReg(PW_OFF,     0x00);
  this->writeReg(LT_OFF,     0x00);
  this->writeReg(TW_OFF,     0x00);
  return;
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
  val = this->readReg(MCTL_OFF);
  val &= ~MCTL_GLVL_MASK;
  val |= selected & MCTL_GLVL_MASK;
  this->writeReg(MCTL_OFF, val);
  return;
}

int MMA_7455::getSensitivity(void)
{
  int     selected = 0;
  uint8_t val      = 0;
  
  val = this->readReg(MCTL_OFF);
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
  val = this->readReg(MCTL_OFF);
  val &= ~MCTL_MOD_MASK;
  val |= selected & MCTL_MOD_MASK;
  this->writeReg(MCTL_OFF, val);
  return;
}

MODE MMA_7455::getMode(void)
{
  MODE    selected = none;
  uint8_t val      = 0;
  
  val = this->readReg(MCTL_OFF);
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
  uint8_t val = this->readReg(MCTL_OFF);
  if(enable)
  {
    val |= MCTL_STON;
  }
  else
  {
    val &= ~MCTL_STON;
  }
  this->writeReg(MCTL_OFF, val);
  return;
}

void MMA_7455::enableDetectionXYZ(bool x, bool y, bool z)
{
  uint8_t val = this->readReg(CTL1_OFF);
  
  /* enable/disbale detection on X */
  if(x) val &= ~CTL1_XDA_DIS;
  else  val |= CTL1_XDA_DIS;
  /* enable/disbale detection on Y */
  if(y) val &= ~CTL1_YDA_DIS;
  else  val |= CTL1_YDA_DIS;
  /* enable/disbale detection on Z */
  if(z) val &= ~CTL1_ZDA_DIS;
  else  val |= CTL1_ZDA_DIS;
  
  this->writeReg(CTL1_OFF, val);
  return;
}

void MMA_7455::setLevelPolarity(LEVEL_MODE mode)
{
  uint8_t val = this->readReg(CTL2_OFF);
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
  this->writeReg(CTL2_OFF, val);
  return;
}

void MMA_7455::setLevelPolarity(unsigned int mode)
{
  uint8_t val = this->readReg(CTL2_OFF);
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
  this->writeReg(CTL2_OFF, val);
  return;
}

void MMA_7455::setThresholdMode(TH_MODE mode)
{
  uint8_t val = this->readReg(CTL1_OFF);
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
  this->writeReg(CTL1_OFF, val);
  return;
}

void MMA_7455::setThresholdMode(unsigned int mode)
{
  uint8_t val = this->readReg(CTL1_OFF);
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
  this->writeReg(CTL1_OFF, val);
  return;
}

void MMA_7455::setLevelThresholdLimit(int8_t limit)
{
  uint8_t val = this->readReg(CTL1_OFF);
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
    this->writeReg(LDTH_OFF, val);
  }
  else
  {
    /* unsigned value of 7-bit */
    /* range: [0; +127] */
    val = (uint8_t)limit;
    val &= ~LDTH_SIGN;
    val &= LDTH_MASK;
    this->writeReg(LDTH_OFF, val);
  }
  return;
}

void MMA_7455::setPulsePolarity(PULSE_MODE mode)
{
  uint8_t val = this->readReg(CTL2_OFF);
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
  this->writeReg(CTL2_OFF, val);
  return;
}

void MMA_7455::setPulsePolarity(unsigned int mode)
{
  uint8_t val = this->readReg(CTL2_OFF);
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
  this->writeReg(CTL2_OFF, val);
  return;
}

void MMA_7455::setPulseThresholdLimit(uint8_t limit)
{
  uint8_t val = limit;
  val &= PDTH_MASK;
  this->writeReg(PDTH_OFF, val);
  return;
}

void MMA_7455::setPulseDuration(uint8_t time)
{
  uint8_t val = time;
  val &= PW_MASK;
  this->writeReg(PW_OFF, val);
  return;
}

void MMA_7455::setPulseLatency(uint8_t time)
{
  uint8_t val = time;
  val &= LT_MASK;
  this->writeReg(LT_OFF, val);
  return;
}

void MMA_7455::setPulseDuration2(uint8_t time)
{
  uint8_t val = time;
  val &= TW_MASK;
  this->writeReg(TW_OFF, val);
  return;
}

int8_t MMA_7455::readAxis8(char axis)
{
  uint8_t reg   = 0;
  uint8_t mask  = 0;
  uint8_t u_val = 0;
  int8_t  s_val = 0;
  
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
  
  u_val = this->readReg(reg) & mask;
  s_val = (int8_t)u_val;
  
  return s_val;
}

void MMA_7455::readAxis8(int8_t* x, int8_t* y, int8_t* z)
{
  if(x) *x = this->readAxis8('x');
  if(y) *y = this->readAxis8('y');
  if(z) *z = this->readAxis8('z');
  return;
}

float MMA_7455::readAxis8g(char axis)
{
  float  f_val = 0;
  int8_t s_val = 0;
  int    sens  = 0;
  
  sens  = this->getSensitivity();
  s_val = this->readAxis8(axis);
  
  /* convert N to g/s */
  f_val = 2.0*(float)sens;
  f_val /= 256.0;
  f_val *= (float)s_val;
  
  return f_val;
}

void MMA_7455::readAxis8g(float* x, float* y, float* z)
{
  if(x) *x = this->readAxis8g('x');
  if(y) *y = this->readAxis8g('y');
  if(z) *z = this->readAxis8g('z');
  return;
}

int16_t MMA_7455::readAxis10(char axis)
{
  uint8_t  reg[2]  = {0};
  uint8_t  mask[2] = {0};
  uint16_t u_val   = 0;
  int16_t  s_val   = 0;
  
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
  
  u_val  = this->readReg(reg[0]) & mask[0];
  u_val |= (this->readReg(reg[1]) & mask[1]) << 8;
  s_val  = (int16_t)u_val;
  
  /* fill of ones if negative value
   * to make it valid in 16 bit format */
  if(s_val & (1 << 9))
  {
    s_val |= 0xFC00;
  }
  
  return s_val;
}

void MMA_7455::readAxis10(int16_t* x, int16_t* y, int16_t* z)
{
  if(x) *x = this->readAxis10('x');
  if(y) *y = this->readAxis10('y');
  if(z) *z = this->readAxis10('z');
  return;
}

float MMA_7455::readAxis10g(char axis)
{
  float   f_val = 0;
  int16_t s_val = 0;
  int     sens  = 0;
  
  sens  = this->getSensitivity();
  s_val = this->readAxis10(axis);
  
  /* convert N to g/s */
  f_val = 2.0*(float)sens;
  f_val /= 1024.0;
  f_val *= (float)s_val;
  
  return f_val;
}

void MMA_7455::readAxis10g(float* x, float* y, float* z)
{
  if(x) *x = this->readAxis10g('x');
  if(y) *y = this->readAxis10g('y');
  if(z) *z = this->readAxis10g('z');
  return;
}

void MMA_7455::setAxisOffset(int16_t x, int16_t y, int16_t z)
{
  this->writeReg(XOFFL_OFF, x & XOFFL_MASK);
  this->writeReg(XOFFH_OFF, (x >> 8) & XOFFH_MASK);
  
  this->writeReg(YOFFL_OFF, y & YOFFL_MASK);
  this->writeReg(YOFFH_OFF, (y >> 8) & YOFFH_MASK);
  
  this->writeReg(ZOFFL_OFF, z & ZOFFL_MASK);
  this->writeReg(ZOFFH_OFF, (z >> 8) & ZOFFH_MASK);
  
  return;
}

void MMA_7455::getAxisOffset(int16_t* x, int16_t* y, int16_t* z)
{
  if(x == NULL || y == NULL || z == NULL)   return;
  
  *x = this->readReg(XOFFL_OFF) & XOFFL_MASK;
  *x |= (this->readReg(XOFFH_OFF) & XOFFH_MASK) << 8;
  *x |= (*x & (1 << 10)) ? 0xF800 : 0x0000;
  
  *y = this->readReg(YOFFL_OFF) & YOFFL_MASK;
  *y |= (this->readReg(YOFFH_OFF) & YOFFH_MASK) << 8;
  *y |= (*y & (1 << 10)) ? 0xF800 : 0x0000;
  
  *z = this->readReg(ZOFFL_OFF) & ZOFFL_MASK;
  *z |= (this->readReg(ZOFFH_OFF) & ZOFFH_MASK) << 8;
  *z |= (*z & (1 << 10)) ? 0xF800 : 0x0000;
  
  return;
}

void MMA_7455::setInterruptMode(ISR_MODE mode)
{
  uint8_t val = this->readReg(CTL1_OFF);
  val &= ~CTL1_INTRG_MASK;
  val |= mode & CTL1_INTRG_MASK;
  this->writeReg(CTL1_OFF, val);
  return;
}

void MMA_7455::getLevelDetection(bool* x, bool* y, bool* z)
{
  uint8_t val = this->readReg(DETSRC_OFF);
  if(x) *x = val & DETSRC_LDX ? true : false;
  if(y) *y = val & DETSRC_LDY ? true : false;
  if(z) *z = val & DETSRC_LDZ ? true : false;
  return;
}

void MMA_7455::getPulseDetection(bool* x, bool* y, bool* z)
{
  uint8_t val = this->readReg(DETSRC_OFF);
  if(x) *x = val & DETSRC_PDX ? true : false;
  if(y) *y = val & DETSRC_PDY ? true : false;
  if(z) *z = val & DETSRC_PDZ ? true : false;
  return;
}

void MMA_7455::getInterrupt(bool* int1, bool* int2)
{
  uint8_t val = this->readReg(DETSRC_OFF);
  if(int1)  *int1 = val & DETSRC_INT1 ? true : false;
  if(int2)  *int2 = val & DETSRC_INT2 ? true : false;
  return;
}

void MMA_7455::clearInterrupt(void)
{
  this->writeReg(INTRST_OFF, INTRST_CLRINT1 | INTRST_CLRINT2);
  this->writeReg(INTRST_OFF, 0);
  return;
}

void MMA_7455::enableInterruptPins(bool enable)
{
  uint8_t val = this->readReg(MCTL_OFF);
  if(enable)    val |= MCTL_DRPD;
  else          val &= ~MCTL_DRPD;
  this->writeReg(MCTL_OFF, val);
  return;
}

uint8_t MMA_7455::readReg(uint8_t reg)
{
  uint8_t buff = 0;
  Wire.beginTransmission(MMA7455_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.beginTransmission(MMA7455_ADDRESS);
  Wire.requestFrom(MMA7455_ADDRESS, 1);
  if(Wire.available())
  {
    buff = Wire.read();
  }
  Wire.endTransmission();
  return buff;
}

void MMA_7455::writeReg(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(MMA7455_ADDRESS);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
  return;
}

void MMA_7455::writeReg(uint8_t reg)
{
  Wire.beginTransmission(MMA7455_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  return;
}
