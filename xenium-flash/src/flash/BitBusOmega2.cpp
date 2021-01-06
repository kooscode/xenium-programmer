/* Onion Omega2+ implementation of Custom 4-bit "BitBus"

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

#include "BitBusOmega2.hpp"

#include <thread>
#include <chrono>
#include <fstream>

#include "XeniumDefines.h"

namespace XK
{
    BitBusOmega2::BitBusOmega2() : BitBus()
    {
        InitGPIO();
    }

    void BitBusOmega2::DelayMicroseconds(uint64_t usec)
    {
        usleep(usec);
    }

    std::string BitBusOmega2::GetHardwareString()
    {
        std::ifstream vfile("/sys/firmware/devicetree/base/model");
        std::string vstr;

        if (vfile.is_open())
            std::getline(vfile, vstr);
        else
            vstr = "UNKNOWN";

        vfile.close();

        return vstr;
    }

    void  BitBusOmega2::SetGPIOMode(GPIOMode gpiomode)
    {
        omega_.pinMode(BITBUS_D0, (int)gpiomode);
        omega_.pinMode(BITBUS_D1, (int)gpiomode);
        omega_.pinMode(BITBUS_D2, (int)gpiomode);
        omega_.pinMode(BITBUS_D3, (int)gpiomode);
    }

    void  BitBusOmega2::Clock()
    {

        omega_.digitalWrite(BITBUS_CLK, HIGH);
//         if (platform_delay_)
//         {   
//             //ugly way to delay for 3B+.. cause normal thread_this:wait_until aint working!
//             auto waituntil = std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(1);
//             while(true) if (std::chrono::high_resolution_clock::now() >= waituntil) break;
//         }       
//         else
//         {
// // TODO - TEST double clock with 3B+ and remove above ugglyness...
//             omega_.digitalWrite(BITBUS_CLK, HIGH);
//             omega_.digitalWrite(BITBUS_CLK, LOW);
//         }         
        omega_.digitalWrite(BITBUS_CLK, LOW);
    }

    void BitBusOmega2::SetBusMode(BusMode busmode)
    {
        omega_.digitalWrite(BITBUS_DIR, busmode);
        Clock();
    }

    void BitBusOmega2::WriteByte(const uint8_t& data)
    {
        // Send lower nibble
        omega_.digitalWrite(BITBUS_D0, (data & 0x01));
        omega_.digitalWrite(BITBUS_D1, (data >> 1 & 0x01));
        omega_.digitalWrite(BITBUS_D2, (data >> 2 & 0x01));
        omega_.digitalWrite(BITBUS_D3, (data >> 3 & 0x01));
        Clock();

        // Send upper nibble
        omega_.digitalWrite(BITBUS_D0, (data >> 4 & 0x01));
        omega_.digitalWrite(BITBUS_D1, (data >> 5 & 0x01));
        omega_.digitalWrite(BITBUS_D2, (data >> 6 & 0x01));
        omega_.digitalWrite(BITBUS_D3, (data >> 7 & 0x01));
        Clock();
    }

    uint8_t BitBusOmega2::ReadByte()
    {
        uint8_t inbyte = 0x00;

        // receive lower nibble
        Clock();
        inbyte = omega_.digitalRead(BITBUS_D0);
        inbyte |= omega_.digitalRead(BITBUS_D1) << 1;
        inbyte |= omega_.digitalRead(BITBUS_D2) << 2;
        inbyte |= omega_.digitalRead(BITBUS_D3) << 3;

        // receive upper nibble
        Clock();
        inbyte |= omega_.digitalRead(BITBUS_D0) << 4;
        inbyte |= omega_.digitalRead(BITBUS_D1) << 5;
        inbyte |= omega_.digitalRead(BITBUS_D2) << 6;
        inbyte |= omega_.digitalRead(BITBUS_D3) << 7;

        return inbyte;
    }

    void  BitBusOmega2::InitGPIO()
    {
        //setup BitBusOmega2 control pins
        omega_.pinMode(BITBUS_CLK, (int)GPIO_OUT);
        omega_.pinMode(BITBUS_DIR, (int)GPIO_OUT);
    }

}

