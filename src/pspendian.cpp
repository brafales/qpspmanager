/***************************************************************************
 *   Copyright (C) 2007 by Bernat Ràfales,,,   *
 *   bernat@rafales-mulet.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#include "pspendian.h"

uint32_t Endian::le32(uint32_t bits)
{
		uint8_t *bytes; 	 
	  bytes = (uint8_t*) & bits;
		return (((uint32_t)bytes[0]) | 
					 (((uint32_t)bytes[1]) << 8) | 
					 (((uint32_t)bytes[2]) << 16) | 
					 (((uint32_t)bytes[3]) << 24));
}

uint16_t Endian::le16(uint16_t bits)
{
		uint8_t *bytes; 	 
	  bytes = (uint8_t*) & bits;
		return (((uint16_t)bytes[0]) | 
					 (((uint16_t)bytes[1]) << 8));
}

uint64_t Endian::le64(uint64_t bits)
{
		uint8_t *bytes; 	 
	  bytes = (uint8_t*) & bits;
		return (((uint64_t)bytes[0]) | 
					 (((uint64_t)bytes[1]) << 8) | 
					 (((uint64_t)bytes[2]) << 16) | 
					 (((uint64_t)bytes[3]) << 24) |
					 (((uint64_t)bytes[4]) << 32) | 
					 (((uint64_t)bytes[5]) << 40) | 
					 (((uint64_t)bytes[6]) << 48) |
					 (((uint64_t)bytes[7]) << 56));
}


int32_t Endian::le32(int32_t bits)
{
		int8_t *bytes; 	 
	  bytes = (int8_t*) & bits;
		return (((int32_t)bytes[0]) | 
					 (((int32_t)bytes[1]) << 8) | 
					 (((int32_t)bytes[2]) << 16) | 
					 (((int32_t)bytes[3]) << 24));
}

int16_t Endian::le16(int16_t bits)
{
		int8_t *bytes; 	 
	  bytes = (int8_t*) & bits;
		return (((int16_t)bytes[0]) | 
					 (((int16_t)bytes[1]) << 8));
}

int64_t Endian::le64(int64_t bits)
{
		int8_t *bytes; 	 
	  bytes = (int8_t*) & bits;
		return (((int64_t)bytes[0]) | 
					 (((int64_t)bytes[1]) << 8) | 
					 (((int64_t)bytes[2]) << 16) | 
					 (((int64_t)bytes[3]) << 24) |
					 (((int64_t)bytes[4]) << 32) | 
					 (((int64_t)bytes[5]) << 40) | 
					 (((int64_t)bytes[6]) << 48) |
					 (((int64_t)bytes[7]) << 56));
}
