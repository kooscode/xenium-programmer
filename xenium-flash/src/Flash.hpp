#ifndef XK_FLASH_
#define XK_FLASH_

#include <stdint.h>
#include <iostream>

#include "BitBus.hpp"

namespace XK
{
    class Flash final 
    {
        public:
            Flash();
            std::string GetPiVersion();
            uint8_t GetManufacturerID();
            uint8_t GetDeviceID();
            void    ChipReset();
            void    ChipErase();
            uint8_t Read(const uint32_t& address);
            void    Write(const uint32_t& address, const char& data);            
            
        private:
            BitBus  bitbus_;
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