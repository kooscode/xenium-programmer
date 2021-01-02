/* JTAG low level functions and base class for cables

Copyright (C) 2004 Andrew Rogers
Copyright (C) 2005-2013 Uwe Bonnes bon@elektron.ikp.physik.tu-darmstadt.de
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
    Removed everything not needed for Raspberry PI and Xilinx xc9572xl
*/

#include "IOBase.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
 
using namespace std;
IOBase::IOBase()
{
    verbose = false;
    memset( ones,0xff,CHUNK_SIZE);
    memset(zeros,   0,CHUNK_SIZE);
    memset(tms_buf,   0,CHUNK_SIZE);
    tms_len = 0;
}    

void IOBase::flush_tms(int force)
{
  if (tms_len)
    tx_tms(tms_buf, tms_len, force);
  memset(tms_buf,   0,CHUNK_SIZE);
  tms_len = 0;
}

void IOBase::set_tms(bool val)
{
  if( tms_len + 1 > CHUNK_SIZE*8)
    flush_tms(false);
  if(val)
    tms_buf[tms_len/8] |= (1 <<(tms_len &0x7));
  tms_len++;
}
    
void IOBase::shiftTDITDO(const unsigned char *tdi, unsigned char *tdo,
			 int length, bool last)
{
  if(length==0) return;
  flush_tms(false);
  txrx_block(tdi, tdo, length,last);
  return;
}

void IOBase::shiftTDI(const unsigned char *tdi, int length, bool last)
{
  shiftTDITDO(tdi, NULL, length,last);
}

// TDI gets a load of zeros, we just record TDO.
void IOBase::shiftTDO(unsigned char *tdo, int length, bool last)
{
    shiftTDITDO(NULL, tdo, length,last);
}

// TDI gets a load of zeros or ones, and we ignore TDO
void IOBase::shift(bool tdi, int length, bool last)
{
    int len = length;
    unsigned char *block = (tdi)?ones:zeros;
    flush_tms(false);
    while (len > CHUNK_SIZE*8)
    {
	txrx_block(block, NULL, CHUNK_SIZE*8, false);
	len -= (CHUNK_SIZE*8);
    }
    shiftTDITDO(block, NULL, len, last);
}


void IOBase::Usleep(unsigned int usec)
{
  flush_tms(false);
  flush();

  usleep(usec);
}

