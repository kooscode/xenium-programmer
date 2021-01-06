/* OmegaS2+ implementation of XC3SPROG IOBase

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

Changes:
Koos du Preez [kdupreez@hotmail.com] Jan 1 2019 
    Prettified and made more readable.
*/

#include "IOOmega2.h"


IOOmega2::IOOmega2(int tms, int tck, int tdi, int tdo)
 : TMSPin(tms), TCKPin(tck), TDIPin(tdi), TDOPin(tdo)
{

    omega_.pinMode(TDIPin, OMEGA_GPIO_OUT);
    omega_.pinMode(TMSPin, OMEGA_GPIO_OUT);
    omega_.pinMode(TCKPin, OMEGA_GPIO_OUT);
    omega_.pinMode(TDOPin, OMEGA_GPIO_IN);
}

IOOmega2::~IOOmega2()
{
}

void IOOmega2::txrx_block(const unsigned char *tdi, unsigned char *tdo, int length, bool last)
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

   omega_.digitalWrite(TCKPin, OMEGA_LOW);

  return;
}

void IOOmega2::tx_tms(unsigned char *pat, int length, int force)
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
    
    omega_.digitalWrite(TCKPin, OMEGA_LOW);
}

void IOOmega2::tx(bool tms, bool tdi)
{
    omega_.digitalWrite(TCKPin, OMEGA_LOW);

    if(tdi)
        omega_.digitalWrite(TDIPin, OMEGA_HIGH);
    else
        omega_.digitalWrite(TDIPin, OMEGA_LOW);

    if(tms)
        omega_.digitalWrite(TMSPin, OMEGA_HIGH);
    else
        omega_.digitalWrite(TMSPin, OMEGA_LOW);

    omega_.digitalWrite(TCKPin, OMEGA_HIGH);
}


bool IOOmega2::txrx(bool tms, bool tdi)
{
  tx(tms, tdi);
  return omega_.digitalRead(TDOPin);
}


