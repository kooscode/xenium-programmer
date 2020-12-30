
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstring>
#include <iomanip>

#include "wiringPi.h"
#include "PiPinMap.h"

//BitBus pins
#define BITBUS_D0 GPIO_16
#define BITBUS_D1 GPIO_21
#define BITBUS_D2 GPIO_18
#define BITBUS_D3 GPIO_20
#define BITBUS_CLK GPIO_24
#define BITBUS_DIR GPIO_23

#define FLASH_SIZE 0x200000
// #define FLASH_SIZE 64

int main(int argc, char** argv) 
{   
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "|       Bare Knuckles Xenium Flash Dumper - Pi Zero ONLY         |" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "|      Fast dumping from Xenium Flash - NO hardware checks!      |" << std::endl;
    std::cout << "|  Requires \"bitbusdumper.jed\" to be programmed to Xilinx CPLD   |" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl << std::endl;

    //setup wiringPi in GPIO pin numbering mode..
    wiringPiSetupGpio();

    //setup BitBus control pins for DIR and CLK
    pinMode(BITBUS_CLK, OUTPUT);
    pinMode(BITBUS_DIR, OUTPUT);

    pinMode(BITBUS_D0, INPUT);
    pinMode(BITBUS_D1, INPUT);
    pinMode(BITBUS_D2, INPUT);
    pinMode(BITBUS_D3, INPUT);    
    
    //alloc memory and read file
    char* flash_buffer = new char[FLASH_SIZE]; 
    memset (flash_buffer, 0x00, FLASH_SIZE);

    // write mode
    digitalWrite(BITBUS_DIR, LOW);
    digitalWrite(BITBUS_CLK, HIGH);
    digitalWrite(BITBUS_CLK, LOW);
    // std::cout << "BLUE/WHITE " << std::flush;

    // read mode
    digitalWrite(BITBUS_DIR, HIGH);
    digitalWrite(BITBUS_CLK, HIGH);
    digitalWrite(BITBUS_CLK, LOW);
    // std::cout << "BLUE" << std::endl;

    auto now = std::chrono::steady_clock::now();
    float progress = 0.0;
    for (uint32_t i = 0; i < FLASH_SIZE; i++)
    {

        //clock set OE Low (address latch)
        digitalWrite(BITBUS_CLK, HIGH);
        digitalWrite(BITBUS_CLK, LOW);
        // std::cout << "GREEN ";

        //clock to copy data to out buffer and ready for read
        digitalWrite(BITBUS_CLK, HIGH);
        digitalWrite(BITBUS_CLK, LOW);

        // read data
        flash_buffer[i] = digitalRead(BITBUS_D0);
        flash_buffer[i] |= digitalRead(BITBUS_D1) << 1;
        flash_buffer[i] |= digitalRead(BITBUS_D2) << 2;
        flash_buffer[i] |= digitalRead(BITBUS_D3) << 3;

        //clock to copy data to out buffer and ready for read
        digitalWrite(BITBUS_CLK, HIGH);
        digitalWrite(BITBUS_CLK, LOW);

        //red data
        flash_buffer[i] |= digitalRead(BITBUS_D0) << 4;
        flash_buffer[i] |= digitalRead(BITBUS_D1) << 5;
        flash_buffer[i] |= digitalRead(BITBUS_D2) << 6;
        flash_buffer[i] |= digitalRead(BITBUS_D3) << 7;

        //clock for OE = high and setup new address
        digitalWrite(BITBUS_CLK, HIGH);
        digitalWrite(BITBUS_CLK, LOW);

        float current_progress = (float) i / FLASH_SIZE * 100.0f;
        if (current_progress > progress + 1)
        {
            progress = current_progress;
            std::cout << "\rReading Flash: " << std::fixed << std::setprecision(0) 
                      << progress << "%              " << std::flush;
        }

    }
    auto elapsed = std::chrono::steady_clock::now() - now;
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    std::cout << "\rReading Flash: DONE (" <<  sec << " Seconds)" << std::endl;

    //write to file
    std::string outfile = "flash.bin";
    std::ofstream fout(outfile);
    fout.write(flash_buffer, FLASH_SIZE);
    fout.close();

    // clear out flash memory.
    delete[] flash_buffer;

    std::cout << "Xenium flash dumped to file: " << outfile << std::endl;

    return 0;
}
