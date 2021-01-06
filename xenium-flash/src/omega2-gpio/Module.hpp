/* Register based GPIO for Onion Omega2+

** Based on Onion's FastGPIO **
https://github.com/OnionIoT/fast-gpio

Copyright Onion

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

#ifndef _MODULE_H_
#define _MODULE_H_

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

class Module {
public:
	Module(void);
	~Module(void);

	void 	SetVerbosity	(int input);
	void 	SetVerbosity	(bool input);

	void	SetDebugMode	(int input);
	void	SetDebugMode	(bool input);


protected:
	// protected functions
	int 				_SetupAddress	(unsigned long int blockBaseAddr, unsigned long int blockSize);
	void 				_WriteReg 		(unsigned long int registerOffset, unsigned long int value);
	unsigned long int 	_ReadReg 		(unsigned long int registerOffset);
	
	void 				_SetBit			(unsigned long int &regVal, int bitNum, int value);
	int 				_GetBit			(unsigned long int regVal, int bitNum);

	// protected members
	int				verbosityLevel;
	int 			debugLevel;

	unsigned long int *regAddress;
};

#endif 	// _MODULE_H_