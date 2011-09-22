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
 
#ifndef ISOCOMPRESSOR_H
#define ISOCOMPRESSOR_H

#include <QString>
#include <iostream>

class ISOCompressor
{
public:
    static bool compressISO(QString in, QString out, int level);
    static bool expandISO(QString in, QString out);

private:
    typedef struct ciso_header
    {
        unsigned char magic[4];         /* +00 : 'C','I','S','O'                 */
        //unsigned long header_size;      /* +04 : header size (==0x18)            */
        uint32_t header_size;      /* +04 : header size (==0x18)            */
        //unsigned long long total_bytes; /* +08 : number of original data size    */
        uint64_t total_bytes; /* +08 : number of original data size    */
        //unsigned long block_size;       /* +10 : number of compressed block size */
        uint32_t block_size;       /* +10 : number of compressed block size */
        unsigned char ver;              /* +14 : version 01                      */
        unsigned char align;            /* +15 : align of index value            */
        unsigned char rsv_06[2];        /* +16 : reserved                        */
    }CISO_H;

    static long long check_file_size(FILE *fp, int &ciso_total_block, CISO_H &ciso);
        
};

#endif
