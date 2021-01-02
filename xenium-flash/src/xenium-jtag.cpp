/* Programming a CPLD on OpenXenium

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
#include <memory>

#include "IOXenium.h"
#include "Jtag.h"
#include "JedecFile.h"
#include "ProgAlgXC95X.h"


// IDCODE   IR_len  ID_Cmd  Text
// 09604093 8       0xfe    XC9572XL

// Constants for Xilinx Device ID and string..
const uint32_t  XC9572XL_ID     = 0x09604093;
const int32_t   XC9572XL_IR_len = 8;
const char      XC9572XL_STR[]  = "XC9572XL";

int main(int argc, char** argv) 
{   
    std::cout << "-----------------------" << std::endl;
    std::cout << "Xenium JTAG Tool - v0.1" << std::endl;
    std::cout << "-----------------------" << std::endl;

    if (argc < 2)
    {
        std::cout << "SYNTAX ERROR: Please provide JED file path." << std::endl;
        std::cout << "\tExample: xenium_jtag /path/to/file.jed" << std::endl;
        return -1;
    }

// ********************  LOADING JED FILE ***************
    std::string jed_file = argv[1];
    std::cout << "Loading JEDEC File: " << std::endl;
    JedecFile jed;
    FILE *jed_fp =  fopen(jed_file.c_str(),"rb");

    //failed to load JED
    if (!jed_fp)
    {
        std::cout << "ERROR LOADING FILE: " << jed_file << std::endl;
        return -1;
    }

    //process JED file
    jed.readFile(jed_fp);
    std::string jed_device = jed.getDevice();
    std:: cout << "\tLoaded: " << jed_file << std::endl; 
    std:: cout << "\tFound JED file device: " << jed_device << std::endl; 

// ********************  SCANNING JTAG CHAIN ***************

    // create PI JTAG Obj
    std::unique_ptr<IOXenium> io = std::make_unique<IOXenium>();
    Jtag jtag(std::move(io));
    jtag.setVerbose(false);
    
    // Scanning JTAG Chain
    std::cout << "Scanning JTAG Chain: " << std::endl;

    int device_cnt = jtag.getChain();
    std::cout << "\tDevices found: " << device_cnt << std::endl;

    //zero devices found?
    if (device_cnt <= 0)
    {
        std::cout << "** ERROR- NO DEVICES ON JTAG CHAIN!" << std::endl;
        return -1;
    }

    //Check Device ID against xc9572xl
    uint32_t device_id = jtag.getDeviceID(0);
    std::cout << "\tChecking for " << XC9572XL_STR << ": ";
    if ((device_id & 0x0fffffff) == XC9572XL_ID)
    {
        std::cout << "[ OK ]" << std::endl;
    }
    else
    {
        std::cout << "[ ** NOT FOUND **]" << std::endl;
        return -1;
    }

    //Select JTAG device 0 and set instruction length
    jtag.selectDevice(0);
    jtag.setDeviceIRLength(0, XC9572XL_IR_len);

    // Validate JTAG device against JED device
    std:: cout << "\tJTAG/JED Device Match: " << jed_device << " "; 
    if (jed_device.find(XC9572XL_STR) != std::string::npos)
    {
        std::cout << "[ OK ]" << std::endl;
    }
    else
    {
        std::cout << "[ ** DEVICE MISMATCH ** ]" << std::endl;
        return -1;
    }

    // setup programmer
    std::cout << "JTAG Programming Device: " << std::endl;
    ProgAlgXC95X jtag_device(jtag, device_id);
    
    //Erase
    std:: cout << "\tErasing: " << std::flush;
    if (jtag_device.erase())
    {
        std:: cout << "[ OK ]" << std::endl; 
    }
    else            
    {
        std:: cout << "[ ** ERASE FAILED ** ]" << std::endl;
        return -1; 
    }

    //programming
    std:: cout << "\tProgramming: [ Please wait.. ]" << std::flush;
    jtag_device.array_program(jed);
    std:: cout << "\r\tProgramming: [ DONE ]         " << std::endl; 

    //verifying
    std:: cout << "\tVerifying: " << std::flush;
    if (jtag_device.array_verify(jed) == 0)
    {
        std:: cout << "[ OK ]" << std::endl; 
    }
    else            
    {
        std:: cout << "[ ** VERIFY FAILED ** ] " << std::endl;
        return -1; 
    }

    fclose(jed_fp);

    return 0;
}
