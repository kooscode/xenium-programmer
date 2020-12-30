#include "BitBus.hpp"

#include <thread>
#include <chrono>

#include "wiringPi.h"

#include "PiPinMap.h"

//BitBus pins
#define BITBUS_D0 GPIO_16
#define BITBUS_D1 GPIO_21
#define BITBUS_D2 GPIO_18
#define BITBUS_D3 GPIO_20
#define BITBUS_CLK GPIO_24
#define BITBUS_DIR GPIO_23

namespace XK
{
    BitBus::BitBus()
    {
        InitGPIO();
    }

    void BitBus::SetPlatformDelay(bool delay_enabled)
    {
        platform_delay_ = delay_enabled;
    }

    void  BitBus::SetGPIOMode(GPIOMode gpiomode)
    {
        pinMode(BITBUS_D0, gpiomode);
        pinMode(BITBUS_D1, gpiomode);
        pinMode(BITBUS_D2, gpiomode);
        pinMode(BITBUS_D3, gpiomode);
    }

    void  BitBus::Clock()
    {

        digitalWrite(BITBUS_CLK, HIGH);
        if (platform_delay_)
        {   
            //ugly way to delay for 3B+.. cause normal thread_this:wait_until aint working!
            auto waituntil = std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(1);
            while(true) if (std::chrono::high_resolution_clock::now() >= waituntil) break;
        }       
        else
        {
// TODO - TEST double clock with 3B+ and remove above ugglyness...
            digitalWrite(BITBUS_CLK, HIGH);
            digitalWrite(BITBUS_CLK, LOW);
        }         
        digitalWrite(BITBUS_CLK, LOW);
    }

    void BitBus::SetBusMode(BusMode busmode)
    {
        digitalWrite(BITBUS_DIR, busmode);
        Clock();
    }

    void BitBus::WriteByte(const uint8_t& data)
    {
        // Send lower nibble
        digitalWrite(BITBUS_D0, (data & 0x01));
        digitalWrite(BITBUS_D1, (data >> 1 & 0x01));
        digitalWrite(BITBUS_D2, (data >> 2 & 0x01));
        digitalWrite(BITBUS_D3, (data >> 3 & 0x01));
        Clock();

        // Send upper nibble
        digitalWrite(BITBUS_D0, (data >> 4 & 0x01));
        digitalWrite(BITBUS_D1, (data >> 5 & 0x01));
        digitalWrite(BITBUS_D2, (data >> 6 & 0x01));
        digitalWrite(BITBUS_D3, (data >> 7 & 0x01));

        Clock();
    }

    uint8_t BitBus::ReadByte()
    {
        uint8_t inbyte = 0x00;

        // receive lower nibble
        Clock();
        inbyte = digitalRead(BITBUS_D0);
        inbyte |= digitalRead(BITBUS_D1) << 1;
        inbyte |= digitalRead(BITBUS_D2) << 2;
        inbyte |= digitalRead(BITBUS_D3) << 3;

        // receive upper nibble
        Clock();
        inbyte |= digitalRead(BITBUS_D0) << 4;
        inbyte |= digitalRead(BITBUS_D1) << 5;
        inbyte |= digitalRead(BITBUS_D2) << 6;
        inbyte |= digitalRead(BITBUS_D3) << 7;

        return inbyte;
    }

    void  BitBus::InitGPIO()
    {
        //setup wiringPi
        wiringPiSetupGpio();

        //setup BitBus control pins
        pinMode(BITBUS_CLK, OUTPUT);
        pinMode(BITBUS_DIR, OUTPUT);
    }

}

