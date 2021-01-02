#ifndef XK_BITBUS_
#define XK_BITBUS_

#include <stdint.h>

namespace XK
{
    enum BusMode
    {
        BITBUS_WRITE = 0,
        BITBUS_READ = 1
    };

    enum GPIOMode
    {
        GPIO_IN = 0,
        GPIO_OUT = 1
    };

    class BitBus final 
    {
        public:
            BitBus();
            void SetPlatformDelay(bool delay_enabled);
            void SetGPIOMode(GPIOMode gpiomode);
            void Clock();
            void SetBusMode(BusMode busmode);
            void WriteByte(const uint8_t& data);
            uint8_t ReadByte();
        private:
            bool platform_delay_;
            void InitGPIO();
    };
}

#endif