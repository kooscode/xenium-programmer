/* Onion Omega 2 implementation of XC3SPROG IOBase

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

#include "IOWiringPi.h"
#include <wiringPi.h>

IOWiringPi::IOWiringPi(int tms, int tck, int tdi, int tdo)
 : TMSPin(tms), TCKPin(tck), TDIPin(tdi), TDOPin(tdo)
{
    wiringPiSetupGpio(); 
    pinMode(TDIPin, OUTPUT);
    pinMode(TMSPin, OUTPUT);
    pinMode(TCKPin, OUTPUT);
    pinMode(TDOPin, INPUT);
}

IOWiringPi::~IOWiringPi()
{
}

void IOWiringPi::txrx_block(const unsigned char *tdi, unsigned char *tdo, int length, bool last)
{
  int i=0;
  int j=0;
  unsigned char tdo_byte=0;
  unsigned char tdi_byte;

  if (tdi)
      tdi_byte = tdi[j];
      
  while( i < length-1)
  {
    tdo_byte = tdo_byte + (txrx(false, (tdi_byte&1) == 1) << (i%8));

    if (tdi)
        tdi_byte = tdi_byte >> 1;

    i++;

    // Next byte
    if ((i%8) == 0)
    { 
        // Save the TDO byte
	    if(tdo)
	        tdo[j] = tdo_byte; 
        
        // Get the next TDI byte
        tdo_byte = 0;
        j++;
        
        if (tdi)
	        tdi_byte = tdi[j]; 
    }
  }

  tdo_byte = tdo_byte + (txrx(last, (tdi_byte&1)==1) << (i%8));

  if(tdo)
      tdo[j]=tdo_byte;

  digitalWrite(TCKPin, LOW);

  return;
}

void IOWiringPi::tx_tms(unsigned char *pat, int length, int force)
{
    int i;
    unsigned char tms;

    for (i = 0; i < length; i++)
    {
        if ((i & 0x7) == 0)
	        tms = pat[i>>3];

        tx((tms & 0x01), true);
        tms = tms >> 1;
    }
    
   digitalWrite(TCKPin, LOW);
}

void IOWiringPi::tx(bool tms, bool tdi)
{
    digitalWrite(TCKPin, LOW);

    if(tdi)
        digitalWrite(TDIPin, HIGH);
    else
        digitalWrite(TDIPin, LOW);

    if(tms)
        digitalWrite(TMSPin, HIGH);
    else
        digitalWrite(TMSPin, LOW);

    digitalWrite(TCKPin, HIGH);
}


bool IOWiringPi::txrx(bool tms, bool tdi)
{
  tx(tms, tdi);
    
  return digitalRead(TDOPin);  
}


