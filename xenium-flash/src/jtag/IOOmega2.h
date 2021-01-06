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

#ifndef __IO_OMEGA_2__
#define __IO_OMEGA_2__

#include "IOBase.h"
#include "FastGpioOmega2.hpp"

class IOOmega2 : public IOBase
{
 public:
  IOOmega2(int tms, int tck, int tdi, int tdo);
  virtual ~IOOmega2();

 protected:
  void tx(bool tms, bool tdi);
  bool txrx(bool tms, bool tdi);

  void txrx_block(const unsigned char *tdi, unsigned char *tdo, int length, bool last);
  void tx_tms(unsigned char *pat, int length, int force);

  int TMSPin;
  int TCKPin;
  int TDIPin;
  int TDOPin;
 
  FastGpioOmega2 omega_;

};

#endif
