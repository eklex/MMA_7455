// MMA_7455.h - 3 Axis Accelerometer Library
// Moritz Kemper, IAD Physical Computing Lab
// moritz.kemper@zhdk.ch
// ZHdK, 03/04/2012
// Released under Creative Commons Licence

#ifndef MMA_7455_h
#define MMA_7455_h

#include "Arduino.h"
#include "Wire.h"

typedef enum _MODE
{
    standby = 0,
    mesure = 1,
    level = 2,
    pulse = 3,
    none
} MODE;

typedef enum _LEVEL_MODE
{
    positive = 0,
    freefall = 1
} LEVEL_MODE;

typedef enum _TH_MODE
{
    absoluteTH = 0,
    signedTH = 1
} TH_MODE;

typedef enum _PULSE_MODE
{
    positive = 0,
    negative = 1
} PULSE_MODE;

class MMA_7455
{
  public:
    MMA_7455();
    void setSensitivity(int sensitivity);
    int  getSensitivity(void);
    void setMode(MODE mode);
    MODE getMode(void);
    void setSelfTest(bool enable);
    void enableDetectionXYZ(bool x, bool y, bool z);
    void setLevelPolarity(LEVEL_MODE mode);
    void setLevelPolarity(unsigned int mode);
    void setThresholdMode(TH_MODE mode);
    void setThresholdMode(unsigned int mode);
    void setThresholdLimit(int8_t limit);
    void calibrateOffset(float x_axis_offset, float y_axis_offset, float z_axis_offset);
    uint8_t readAxis(char axis);
    uint8_t readReg(uint8_t reg);
    void writeReg(uint8_t reg, uint8_t val);
    void writeReg(uint8_t reg);

  private:
    uint8_t _buffer;
	float _x_axis_offset;
	float _y_axis_offset;
	float _z_axis_offset;
};

#endif
