/* Register based GPIO for Onion Omega2+

** Based on Onion's FastGPIO **
https://github.com/OnionIoT/fast-gpio

Copyright Onion

Copyright (C) 2019-2020 Koos du Preez (kdupreez@hotmail.com)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "FastGpioOmega2.hpp"

FastGpioOmega2::FastGpioOmega2(void)
{
	// setup the memory address space
	_SetupAddress(REG_BLOCK_ADDR, REG_BLOCK_SIZE);
}

FastGpioOmega2::~FastGpioOmega2(void)
{
	// nothing for now
}

void FastGpioOmega2::setGpioOffset(int gpio){
	int mod;
	mod = gpio / 32;
	if(mod == 0){
		this->ctrlOffset = REGISTER_CTRL0_OFFSET;
		this->dataOffset = REGISTER_DATA0_OFFSET;
		this->dataSetOffset = REGISTER_DSET0_OFFSET;
		this->dataClrOffset = REGISTER_DCLR0_OFFSET;
	}
	else if(mod == 1){
		this->ctrlOffset = REGISTER_CTRL1_OFFSET;
		this->dataOffset = REGISTER_DATA1_OFFSET;
		this->dataSetOffset = REGISTER_DSET1_OFFSET;
		this->dataClrOffset = REGISTER_DCLR1_OFFSET;
	} else{
		this->ctrlOffset = REGISTER_CTRL2_OFFSET;
		this->dataOffset = REGISTER_DATA2_OFFSET;
		this->dataSetOffset = REGISTER_DSET2_OFFSET;
		this->dataClrOffset = REGISTER_DCLR2_OFFSET;
	}
}
// public functions
void FastGpioOmega2::pinMode(int pinNum, int bOutput)
{
	unsigned long int regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current input and output settings
	regVal = _ReadReg(ctrlOffset);
	if (verbosityLevel > 0) printf("Direction setting read: 0x%08lx\n", regVal);

	// set the OE for this pin
	_SetBit(regVal, gpio, bOutput);
	if (verbosityLevel > 0) printf("Direction setting write: 0x%08lx\n", regVal);

	// write the new register value
	_WriteReg(ctrlOffset, regVal);

}

int FastGpioOmega2::getPinMode(int pinNum)
{
	unsigned long int regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current input and output settings
	regVal 	= _ReadReg(ctrlOffset);
	if (verbosityLevel > 0) printf("Direction setting read: 0x%08lx\n", regVal);

	return _GetBit(regVal, gpio);
}

void FastGpioOmega2::digitalWrite(int pinNum, int value)
{
	unsigned long int 	regAddr;
	unsigned long int 	regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;

	if (value == 0 )	{
		// write to the clear register
		regAddr 	= dataClrOffset;
	}
	else {
		// write to the set register
		regAddr 	= dataSetOffset;
	}

	// put the desired pin value into the register 
	regVal = (0x1 << gpio);

	// write to the register
	_WriteReg (regAddr, regVal);
}

int FastGpioOmega2::digitalRead(int pinNum)
{
	unsigned long int 	regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current value of all pins
	regVal 	= _ReadReg (dataOffset);

	// find the value of the specified pin
	return _GetBit(regVal, gpio);
}