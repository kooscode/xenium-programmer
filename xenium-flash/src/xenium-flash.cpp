/* Programming a FLASH on OpenXenium

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


#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstring>
#include <iomanip>

#include "Flash.hpp"

#define FLASH_SIZE 0x200000
#define KB 1024

int main(int argc, char** argv) 
{   
    XK::Flash flash;

    std::cout << "---------------------------" << std::endl;
    std::cout << "Xenium Flashing Tool - v0.1" << std::endl;
    std::cout << "---------------------------" << std::endl;

    if (argc < 2)
    {
        std::cout << "SYNTAX ERROR: Please provide flash BIN file path." << std::endl;
        std::cout << "\tExample: xenium_flash /path/to/file.bin" << std::endl;
        return -1;
    }

    // need pi hardware revision for timing control..
    std::string pi_version = flash.GetHardwareString();
    std::cout << "Checking Hardware: " << pi_version << std::endl;

// ********************  LOADING FLASH FILE ***************
    std::string infile = argv[1];
    std::cout << "Loading Xenium Flash BIN: " << infile << std::endl;

    std::ifstream fin(infile);
    if (fin.fail()) 
    {
        std::cout << "ERROR LOADING FILE: " << infile << std::endl;
        return -1;
    }

    // check file size
    fin.seekg(0, std::ios::end);
    uint32_t flash_size = fin.tellg();
    if (flash_size != FLASH_SIZE) 
    {
        std::cout << "ERROR: Xenium Flash BIN file must be " << FLASH_SIZE << " Bytes" << std::endl;
        fin.close(); 
        return -1;
    }

    //alloc memory and read file
    char* flash_buffer = new char[flash_size]; 
    // seek back to start
    fin.seekg(0, std::ios::beg);
    // read entire flash in ram
    fin.read(flash_buffer, flash_size);
    // close file
    fin.close(); 


// ******************** CHECK XENIUM HARDWARE ***************
    std::cout << "Detecting Xenium Flash: " << std::flush;
    flash.ChipReset();
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    uint8_t manufacturer = flash.GetManufacturerID();
    uint8_t deviceid = flash.GetDeviceID();

    // OpenXenium CPLD brings back 0x55 and 0x5F.. very convenient..
    if (manufacturer == 0x55 && deviceid == 0x5F)
    {
        std::cout << "ERROR\n\n** XENIUM NOT IN BITBUS MODE - Please program " 
                  << "the \"bitbus2flash.jed\" file first! **" << std::endl;
        return -1;
    }
    else if (manufacturer != 0x01 || deviceid != 0xC4)
    {
        std::cout << "ERROR\n\n** XENIUM FLASH DEVICE NOT FOUND - Manufacturer:" 
                  << (int) manufacturer << ", Device:" << (int) deviceid << " **\n" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "OK [Spansion S29AL016J]" << std::endl; 
    }

// ********************* FLASH CHIP ERASE ***************** 
    flash.ChipReset();
    std::string erase_ok;
    std::cout << "This will ERASE your Xenium Flash, ARE YOU SURE? (Yes/No): " << std::flush;
    std::getline (std::cin, erase_ok);
    if ((erase_ok.find("Y") != 0) &&  (erase_ok.find("y") != 0))
    {
        std::cout << "\n** XENIUM FLASH CANCELLED!! **\n" << std::endl;
        return -1;
    }

    flash.ChipErase();
    // wait for erase to complete..
    auto now = std::chrono::steady_clock::now();
    uint32_t erase_waits = 0;
    const char wait_anim[8] = {'-','\\','|','/'};
    uint8_t anim_pos = 0;
    while (true)
    {
        if (erase_waits > 120)
        {
            std::cout << "\nERROR\n\n** XENIUM FLASH ERASE FAILED!! **\n" << std::endl;
            flash.ChipReset();
            return -1;
        }

        //check toggle bits for erase complete after Xsec
        if ((erase_waits > 2) && (flash.Read(0x00) == flash.Read(0x00)))
        {
            break;
        }

        std::cout << "\rErasing Xenium Flash: " << wait_anim[anim_pos] << " (Waiting)    " << std::flush;
        std::this_thread::sleep_for (std::chrono::milliseconds(500));
        anim_pos = (anim_pos==3)?0:anim_pos+1;
        erase_waits++;
    }
    auto elapsed = std::chrono::steady_clock::now() - now;
    uint32_t sec = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    std::cout << "\rErasing Xenium Flash: DONE (" <<  sec << " Seconds)" << std::endl;


// ********************* FLASH CHIP PROGRAM ***************** 
    now = std::chrono::steady_clock::now();
    float progress = 0.0;
    for (uint32_t i = 0; i < flash_size; i++)
    {
        //write byte to flash
        flash.Write(i, flash_buffer[i]);
        
        float current_progress = (float) i / flash_size * 100.0f;
        if (current_progress > progress + 1)
        {
            progress = current_progress;
            std::cout << "\rWriting Flash: " << std::fixed << std::setprecision(0) 
                      << progress << "%  " << std::flush;
        }
    }
    elapsed = std::chrono::steady_clock::now() - now;
    sec = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    std::cout << "\rWriting Flash: DONE (" <<  sec << " Seconds)" << std::endl;


// ********************* FLASH CHIP VERIFY ***************** 
    now = std::chrono::steady_clock::now();
    progress = 0.0;
    for (uint32_t i = 0; i < flash_size; i++)
    {
        //read byte from flash
        uint inbyte = flash.Read(i);
        if (flash_buffer[i] == (char)inbyte)
        {
            flash_buffer[i] = inbyte;
        }
        else
        {
            std::cout << std::endl << "ERROR\n\n** XENIUM FLASH VERIFY FAILED!! **\n" << std::endl;
            flash.ChipReset();
            return -1;
        }
        
        float current_progress = (float) i / flash_size * 100.0f;
        if (current_progress > progress + 1)
        {
            progress = current_progress;
            std::cout << "\rVerifying Flash: " << std::fixed << std::setprecision(0) 
                      << progress << "%              " << std::flush;
        }
    }
    elapsed = std::chrono::steady_clock::now() - now;
    sec = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    std::cout << "\rVerifying Flash: DONE (" <<  sec << " Seconds)" << std::endl;

// you can uncomment this code if you wish to dump to file from xenium 
    // //write to file
    // std::string outfile = "flash.bin";
    // std::ofstream fout(outfile);
    // fout.write(flash_buffer, flash_size);
    // fout.close();
    // std::cout << "Writing Xenium flash dumped to file: " << outfile << std::endl;

    // clear out flash memory.
    delete[] flash_buffer;

    //reset to turn LED blue..
    flash.ChipReset();

    std::cout << std::endl << "Xenium Flash SUCCESS!!" << std::endl << std::endl;

    return 0;
}
