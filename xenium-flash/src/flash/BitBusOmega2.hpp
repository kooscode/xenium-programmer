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

#ifndef XK_BITBUS_OMEGA2_
#define XK_BITBUS_OMEGA2_

#include <stdint.h>

#include "BitBus.hpp"
#include "FastGpioOmega2.hpp"

namespace XK
{
    class BitBusOmega2 : public BitBus 
    {
        public:
            BitBusOmega2();
            void DelayMicroseconds(uint64_t usec);
            std::string GetHardwareString();
            void SetGPIOMode(GPIOMode gpiomode);
            void Clock();
            void SetBusMode(BusMode busmode);
            void WriteByte(const uint8_t& data);
            uint8_t ReadByte();

        private:
            void InitGPIO();
            FastGpioOmega2 omega_;
            digitalRead
    };
}

#endif