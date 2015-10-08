# Freescale 3-axis accelerometer MMA7455 library for Arduino
> A serious MMA7455 arduino library

## Features
* Support both I2C and SPI protocol
* Get the 8-bit and 10-bit values of each axis
* Get the value in 'g' for each axis
* Support the standard measurement mode
* Support the level mode (with interrupts)
* Support the pulse mode (with interrupts)
* Support the double pulse mode (with interrupts)

## Limitations
* The library relies on the standard Arduino libraries: the Wire library (for I2C) and the SPI library.
* Particle SparkCore has some performance issues with this library (and I don't know why), however the Particle Photon works wonderfully.
* The SPI feature works only with a 3.3v logic level, this is an hardware limitation of the MMA7455.

## Hardware tested
* Arduino Uno (I2C only)
* Arduino Duemilanove (I2C only)
* Arduino Pro Mini 3.3v (I2C and SPI)
* Particle Photon (I2C and SPI)
* *Note: If your board is not mentioned in the list,
it doesn't mean it won't work with yours, it just means I didn't try it
because I don't have every Arduino board on planet Earth.*

## Examples
* MMA7455_Demo: Simply display the 10-bit raw value and the 'g' value for each axis in measurement mode.
* MMA7455_AutoCalibration: Let it run to determine the offset values of your accelerometer.
* MMA7455_InterruptLevel: Illustrate the level mode and the interrupts.
* MMA7455_InterruptPulse: Illustrate the pulse mode and the interrupts.
* MMA7455_InterruptDoublePulse: Illustrate the double pulse mode and the interrupts.

## How-to use it?
1. Download the library
2. Extract and rename the folder to "MMA_7455".
3. Place the renamed folder in arduino/libraries/
4. Open the arduino IDE
5. Open MMA7455_Demo sample code
6. Upload it in your arduino board
7. Open a serial console set at 9600 baud
8. See the live value of the 3 axis

## License GPLv2
```
  Freescale 3-Axis Accelerometer MMA7455 Library designed for Arduino
  Copyright (C) 2015  Alexandre Boni
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
```
