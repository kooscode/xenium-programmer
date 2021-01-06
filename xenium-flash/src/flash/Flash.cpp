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

#include "Flash.hpp"

namespace XK
{
    Flash::Flash() :
        bitbus_(std::make_unique<PLATFORMBUS>())
    {        
        //check if delay needs to be injected for non Pi-Zero models..
        platform_delay_ = bitbus_->GetHardwareString().find("Zero") == std::string::npos;
        bitbus_->SetPlatformDelay(platform_delay_);

        // Innitialize with GPIO for READ
        bitbus_->SetGPIOMode(GPIO_IN);
        gpiomode_ = GPIO_IN;

        // toggle modes to reset bus to READ
        bitbus_-> SetBusMode(BITBUS_WRITE);
        bitbus_-> SetBusMode(BITBUS_READ);
        busmode_ = BITBUS_READ;

    }

    std::string Flash::GetHardwareString()
    {
        return bitbus_->GetHardwareString();
    }

    uint8_t Flash::GetManufacturerID()
    {
        // Identification Command
        // Address 0x00 = ManufacturerID 
        SetBusMode(BITBUS_WRITE);
        WriteByte(0xAAA, 0xAA);
        WriteByte(0x555, 0x55);
        WriteByte(0xAAA, 0x90);

        SetBusMode(BITBUS_READ);
        uint8_t manid = ReadByte(0x00);   
        return manid; 
    }

    uint8_t Flash::GetDeviceID()
    {
        // Identification Command
        // Address 0x02 = DeviceID
        SetBusMode(BITBUS_WRITE);
        WriteByte(0xAAA, 0xAA);
        WriteByte(0x555, 0x55);
        WriteByte(0xAAA, 0x90);

        SetBusMode(BITBUS_READ);
        uint8_t chipid = ReadByte(0x02);   
        return chipid; 
    }

    void Flash::ChipReset()
    {
        //ManufacturerID Command
        SetBusMode(BITBUS_WRITE);        
        WriteByte(0x000, 0xF0);

        //force toggle mode lines to reset bus 
        bitbus_-> SetBusMode(BITBUS_READ);
        bitbus_-> SetBusMode(BITBUS_WRITE);
    }

    void Flash::ChipErase()
    {
        SetBusMode(BITBUS_WRITE);
        // Send Chip Erase Command
        WriteByte(0xAAA, 0xAA);
        WriteByte(0x555, 0x55); 
        WriteByte(0xAAA, 0x80); 
        WriteByte(0xAAA, 0xAA); 
        WriteByte(0x555, 0x55); 
        WriteByte(0xAAA, 0x10); 
    }

    uint8_t Flash::Read(const uint32_t& address)
    {
        SetBusMode(BITBUS_READ);
        uint8_t ret = ReadByte(address);
        return ret;
    }

    void Flash::Write(const uint32_t& address, const char& data)
    {
        SetBusMode(BITBUS_WRITE);
        //Send Program Command
        WriteByte(0xAAA, 0xAA);
        WriteByte(0x555, 0x55);
        WriteByte(0xAAA, 0xA0);

        //write byte
        WriteByte(address, data);    
    }

    void Flash::SetBusMode(BusMode busmode)
    {
        if (busmode_ != busmode)
        {
            busmode_ = busmode;
            bitbus_->SetBusMode(busmode_);
        }
    }

    void Flash::SetGPIOMode(GPIOMode gpiomode)
    {
        if (gpiomode_ != gpiomode)
        {
            gpiomode_ = gpiomode;
            bitbus_->SetGPIOMode(gpiomode_);
        }
    }

    uint8_t Flash::ReadByte(const uint32_t& address)
    {
        uint8_t bb_data = 0x00;
        uint8_t* addr_byte = (uint8_t*) &address;

        // transmit 24-bits of address (only 21-bits are used)
        SetGPIOMode(GPIO_OUT);
        bitbus_->WriteByte(addr_byte[0]);
        bitbus_->WriteByte(addr_byte[1]);
        bitbus_->WriteByte(addr_byte[2]);

        // flash: we=1, oe=0;
        bitbus_->Clock();

        // read 8-bit data
        SetGPIOMode(GPIO_IN);
        bb_data = bitbus_->ReadByte();

        // flash: we=1, oe=1
        bitbus_->Clock();

        return bb_data;
    }

    void Flash::WriteByte(const uint32_t& address, const uint8_t& data)
    {
        uint8_t* addr_byte = (uint8_t*) &address;
        
        // transmit 24-bits of address (only 21-bits are used)
        SetGPIOMode(GPIO_OUT);
        bitbus_->WriteByte(addr_byte[0]);
        bitbus_->WriteByte(addr_byte[1]);
        bitbus_->WriteByte(addr_byte[2]);

        // flash: we=0, oe=1;
        bitbus_->Clock();

        // transmit 8 bit data
        bitbus_->WriteByte(data);

        // flash: we=1, oe=1;
        bitbus_->Clock();

        // small delay between writes makes it much more stable
        bitbus_->DelayMicroseconds(3);
    }

}
