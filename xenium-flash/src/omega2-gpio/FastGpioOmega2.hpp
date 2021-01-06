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

#ifndef _FAST_GPIO_OMEGA2_H_
#define _FAST_GPIO_OMEGA2_H_

#include <string.h> 

#include "Module.hpp"

//Define Macros in derived class. 
#define REG_BLOCK_ADDR			0x10000000
#define REG_BLOCK_SIZE			0x6AC
//DIRECTION CONTROL REGISTERS

//GPIO_CTRL_0 10000600(Directions for GPIO0-GPIO31)
#define REGISTER_CTRL0_OFFSET		384
//GPIO_CTRL_1 10000604(Directions for GPIO32-GPIO63)
#define REGISTER_CTRL1_OFFSET		385
//GPIO_CTRL_2 10000608(Directions for GPIO64-GPIO95)
#define REGISTER_CTRL2_OFFSET		386

//DATA REGISTERS: STATES OF GPIOS 

//GPIO_DATA_0 10000620(GPIO0-31)
#define REGISTER_DATA0_OFFSET		392
//GPIO_DATA_1 10000624(GPIO32-63)
#define REGISTER_DATA1_OFFSET		393
//GPIO_DATA_2 10000628(GPIO64-95)
#define REGISTER_DATA2_OFFSET		394

//DATA SET REGISTERS: SET STATES OF GPIO_DATA_x registers

//GPIO_DSET_0 10000630(GPIO0-31)
#define REGISTER_DSET0_OFFSET		396
//GPIO_DSET_1 10000634(GPIO31-63)
#define REGISTER_DSET1_OFFSET		397
//GPIO_DSET_2 10000638(GPIO64-95)
#define REGISTER_DSET2_OFFSET		398

//DATA CLEAR REGISTERS: CLEAR BITS OF GPIO_DATA_x registers

//GPIO_DCLR_0 10000640(GPIO0-31)
#define REGISTER_DCLR0_OFFSET		400
//GPIO_DCLR_1 10000644(GPIO31-63)
#define REGISTER_DCLR1_OFFSET		401
//GPIO_DCLR_2 10000648(GPIO64-95)
#define REGISTER_DCLR2_OFFSET		402

#define GPIO_OUT 1
#define GPIO_IN 0

#define HIGH 1
#define LOW 0

#define GPIO_0  0
#define GPIO_1  1
#define GPIO_2  2
#define GPIO_3  3
#define GPIO_6  6
#define GPIO_11 11
#define GPIO_15 15
#define GPIO_16 16
#define GPIO_17 17
#define GPIO_18 18
#define GPIO_19 19

class FastGpioOmega2 : public Module 
{
    public:
        FastGpioOmega2(void);
        ~FastGpioOmega2(void);

        void pinMode(int pinNum, int bOutput);
        int getPinMode(int pinNum);

        void digitalWrite (int pinNum, int value);
        int digitalRead(int pinNum);

    private:
        // private functions
        int pinNumber;
        int	ctrlOffset;
        int	dataOffset;
        int dataSetOffset;
        int dataClrOffset;

        //Populates the offset private members above depending on selected GPIO
        void setGpioOffset(int gpio);
};


#endif 	// _FAST_GPIO_OMEGA2_H_