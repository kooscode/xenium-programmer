/* Programming a SPANSION flash via 4-bit bus

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

#ifndef XK_FLASH_
#define XK_FLASH_

#include <memory>
#include <stdint.h>
#include <iostream>

#ifdef OMEGA2
    #include "BitBusOmega2.hpp"
    #define PLATFORMBUS BitBusOmega2
#else
    #include "BitBusPI.hpp"
    #define PLATFORMBUS BitBusPI
#endif

namespace XK
{
    class Flash final 
    {
        public:
            Flash();
            std::string GetHardwareString();
            uint8_t GetManufacturerID();
            uint8_t GetDeviceID();
            void    ChipReset();
            void    ChipErase();
            uint8_t Read(const uint32_t& address);
            void    Write(const uint32_t& address, const char& data);            
            
        private:
            std::unique_ptr<BitBus>  bitbus_;
            BusMode busmode_;
            GPIOMode gpiomode_;
            bool    platform_delay_;

            void    SetBusMode(BusMode busmode);
            void    SetGPIOMode(GPIOMode gpiomode);
            uint8_t ReadByte(const uint32_t& address);
            void    WriteByte(const uint32_t& address, const uint8_t& data);
    };
}

#endif