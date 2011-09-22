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
 
#include "isocompressor.h"
#include "pspendian.h"
#include <QProgressDialog>
#include <QApplication>
#include <zlib.h>

bool ISOCompressor::compressISO(QString in, QString out, int level)
{
    QProgressDialog progress;
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setMinimum(0);
    progress.setCancelButton(0);
    progress.setLabelText(progress.tr("Compressing ISO"));

    FILE *fin,*fout;
    if ((fin = fopen(in.toUtf8(), "rb")) == NULL)
    {
        return false;
    }
    if ((fout = fopen(out.toUtf8(), "wb")) == NULL)
    {
        return false;
    }

    z_stream z;

    unsigned int *index_buf = NULL;
    unsigned int *crc_buf = NULL;
    unsigned char *block_buf1 = NULL;
    unsigned char *block_buf2 = NULL;

    long long file_size;
    unsigned long long write_pos;
    int index_size;
    int block;
    unsigned char buf4[64];
    int cmp_size;
    int status;
    int percent_period;
    int percent_cnt;
    int align,align_b,align_m;
    int ciso_total_block;
    CISO_H ciso;
    file_size = check_file_size(fin, ciso_total_block, ciso);
    if(file_size<0)
    {
        // free memory
        if(index_buf) free(index_buf);
        if(crc_buf) free(crc_buf);
        if(block_buf1) free(block_buf1);
        if(block_buf2) free(block_buf2);

        return false;
    }

    // allocate index block
    index_size = (ciso_total_block + 1 ) * sizeof(unsigned long);
    index_buf  = (unsigned int *)malloc(index_size);
    crc_buf    = (unsigned int *)malloc(index_size);
    block_buf1 = (unsigned char *)malloc(ciso.block_size);
    block_buf2 = (unsigned char *)malloc(ciso.block_size*2);

    if( !index_buf || !crc_buf || !block_buf1 || !block_buf2 )
    {
        // free memory
        if(index_buf) free(index_buf);
        if(crc_buf) free(crc_buf);
        if(block_buf1) free(block_buf1);
        if(block_buf2) free(block_buf2);
        
        return false;
    }
    memset(index_buf,0,index_size);
    memset(crc_buf,0,index_size);
    memset(buf4,0,sizeof(buf4));

    // init zlib
    z.zalloc = Z_NULL;
    z.zfree  = Z_NULL;
    z.opaque = Z_NULL;

    // write header block
    //we write a big endian block!
    CISO_H beCiso = ciso;
    beCiso.header_size = Endian::le32(beCiso.header_size);
    beCiso.total_bytes = Endian::le64(beCiso.total_bytes);
    beCiso.block_size = Endian::le32(beCiso.block_size);
    fwrite(&beCiso,1,sizeof(beCiso),fout);

    // dummy write index block
    fwrite(index_buf,1,index_size,fout);

    write_pos = sizeof(ciso) + index_size;

    // compress data
    percent_period = ciso_total_block/100;
    percent_cnt    = ciso_total_block/100;

    progress.setMaximum(100);    

    align_b = 1<<(ciso.align);
    align_m = align_b -1;

    for(block = 0;block < ciso_total_block ; block++)
    {
        if(--percent_cnt<=0)
        {
            percent_cnt = percent_period;
            progress.setValue(block / percent_period);
            qApp->processEvents();
        }

        if (deflateInit2(&z, level , Z_DEFLATED, -15,8,Z_DEFAULT_STRATEGY) != Z_OK)
        {
            // free memory
            if(index_buf) free(index_buf);
            if(crc_buf) free(crc_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);
            
            return false;
        }

        // write align
        align = (int)write_pos & align_m;
        if(align)
        {
            align = align_b - align;
            if((int)fwrite(buf4,1,align, fout) != align)
            {
                 // free memory
                 if(index_buf) free(index_buf);
                 if(crc_buf) free(crc_buf);
                 if(block_buf1) free(block_buf1);
                 if(block_buf2) free(block_buf2);

                 return false;
            }
            write_pos += align;
        }

        // mark offset index
        index_buf[block] = write_pos>>(ciso.align);

        // read buffer
        z.next_out  = block_buf2;
        z.avail_out = ciso.block_size*2;
        z.next_in   = block_buf1;
        z.avail_in  = fread(block_buf1, 1, ciso.block_size , fin);
        if(z.avail_in != ciso.block_size)
        {
            // free memory
            if(index_buf) free(index_buf);
            if(crc_buf) free(crc_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);

            return false;
        }

        // compress block
        status = deflate(&z, Z_FINISH);
        if (status != Z_STREAM_END)
        {
            // free memory
            if(index_buf) free(index_buf);
            if(crc_buf) free(crc_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);

            return false;
        }

        cmp_size = ciso.block_size*2 - z.avail_out;

        // choise plain / compress
        if(cmp_size >= (long)ciso.block_size)
        {
            cmp_size = ciso.block_size;
            memcpy(block_buf2,block_buf1,cmp_size);
            // plain block mark
            index_buf[block] |= 0x80000000;
        }

        // write compressed block
        if((int)fwrite(block_buf2, 1,cmp_size , fout) != cmp_size)
        {
            // free memory
            if(index_buf) free(index_buf);
            if(crc_buf) free(crc_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);

            return false;
        }

        // mark next index
        write_pos += cmp_size;

        // term zlib
        if (deflateEnd(&z) != Z_OK)
        {
            // free memory
            if(index_buf) free(index_buf);
            if(crc_buf) free(crc_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);

            return false;
        }
        //Flush! For testing
        fflush(fout);
    }
    progress.setValue(100);
    // last position (total size)
    index_buf[block] = write_pos>>(ciso.align);

    // write header & index block
    fseek(fout,sizeof(ciso),SEEK_SET);
    fwrite(index_buf,1,index_size,fout);

    // free memory
    if(index_buf) free(index_buf);
    if(crc_buf) free(crc_buf);
    if(block_buf1) free(block_buf1);
    if(block_buf2) free(block_buf2);

    // close files
    fclose(fin);
    fclose(fout);
    
    return true;

}

long long ISOCompressor::check_file_size(FILE *fp, int &ciso_total_block, CISO_H &ciso)
{
  long long pos;

  if( fseek(fp,0,SEEK_END) < 0)
    return -1;
  pos = ftell(fp);
  if(pos==-1) return pos;

  // init ciso header
  memset(&ciso,0,sizeof(ciso));

  ciso.magic[0] = 'C';
  ciso.magic[1] = 'I';
  ciso.magic[2] = 'S';
  ciso.magic[3] = 'O';
  ciso.ver      = 0x01;

  ciso.block_size  = 0x800; /* ISO9660 one of sector */
  ciso.total_bytes = pos;
#if 0
  /* align >0 has bug */
  for(ciso.align = 0 ; (ciso.total_bytes >> ciso.align) >0x80000000LL ; ciso.align++);
#endif

  ciso_total_block = pos / ciso.block_size ;

  fseek(fp,0,SEEK_SET);

  return pos;
}


bool ISOCompressor::expandISO(QString in, QString out)
{
    QProgressDialog progress;
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setMinimum(0);
    progress.setCancelButton(0);
    progress.setLabelText(progress.tr("Expanding ISO"));
    FILE *fin,*fout;
    if ((fin = fopen(in.toUtf8(), "rb")) == NULL)
    {
        return false;
    }
    if ((fout = fopen(out.toUtf8(), "wb")) == NULL)
    {
        return false;
    }

    z_stream z;

    unsigned int *index_buf = NULL;
    unsigned char *block_buf1 = NULL;
    unsigned char *block_buf2 = NULL;
    unsigned int index , index2;
    unsigned long long read_pos , read_size;
    int index_size;
    int block;
    int cmp_size;
    int status;
    int percent_period;
    int percent_cnt;
    int plain;

    // read header
    CISO_H ciso;
    if( fread(&ciso, 1, sizeof(ciso), fin) != sizeof(ciso) )
    {
        // free memory
        if(index_buf) free(index_buf);
        if(block_buf1) free(block_buf1);
        if(block_buf2) free(block_buf2);

        return false;
    }

		//Endianness
		ciso.header_size = Endian::le32(ciso.header_size);
    ciso.total_bytes = Endian::le64(ciso.total_bytes);
    ciso.block_size = Endian::le32(ciso.block_size);
    // check header
    if(
        ciso.magic[0] != 'C' ||
        ciso.magic[1] != 'I' ||
        ciso.magic[2] != 'S' ||
        ciso.magic[3] != 'O' ||
        ciso.block_size ==0  ||
        ciso.total_bytes == 0
        )
    {
        // free memory
        if(index_buf) free(index_buf);
        if(block_buf1) free(block_buf1);
        if(block_buf2) free(block_buf2);

        return false;
    }
  
    int ciso_total_block = ciso.total_bytes / ciso.block_size;

    // allocate index block
    index_size = (ciso_total_block + 1 ) * sizeof(unsigned long);
    index_buf  = (unsigned int *)malloc(index_size);
    block_buf1 = (unsigned char *)malloc(ciso.block_size);
    block_buf2 = (unsigned char *)malloc(ciso.block_size*2);

    if( !index_buf || !block_buf1 || !block_buf2 )
    {
        // free memory
        if(index_buf) free(index_buf);
        if(block_buf1) free(block_buf1);
        if(block_buf2) free(block_buf2);

        return false;
    }
    memset(index_buf,0,index_size);

    // read index block
    if( (int)fread(index_buf, 1, index_size, fin) != index_size )
    {
        // free memory
        if(index_buf) free(index_buf);
        if(block_buf1) free(block_buf1);
        if(block_buf2) free(block_buf2);

        return false;
    }

    // init zlib
    z.zalloc = Z_NULL;
    z.zfree  = Z_NULL;
    z.opaque = Z_NULL;

    // decompress data
    percent_period = ciso_total_block/100;
    percent_cnt = 0;
    progress.setMaximum(100);

    for(block = 0;block < ciso_total_block ; block++)
    {
        if(--percent_cnt<=0)
        {
            percent_cnt = percent_period;
            progress.setValue(block / percent_period);
            qApp->processEvents();
        }

        if (inflateInit2(&z,-15) != Z_OK)
        {
            //free memory
            if(index_buf) free(index_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);

            return false;
        }

        // check index
        index  = index_buf[block];
        plain  = index & 0x80000000;
        index  &= 0x7fffffff;
        read_pos = index << (ciso.align);
        if(plain)
        {
            read_size = ciso.block_size;
        }
        else
        {
            index2 = index_buf[block+1] & 0x7fffffff;
            read_size = (index2-index) << (ciso.align);
        }
        fseek(fin,read_pos,SEEK_SET);

        z.avail_in  = fread(block_buf2, 1, read_size , fin);
        if(z.avail_in != read_size)
        {
            // free memory
            if(index_buf) free(index_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);

            return false;
        }

        if(plain)
        {
            memcpy(block_buf1,block_buf2,read_size);
            cmp_size = read_size;
        }
        else
        {
            z.next_out  = block_buf1;
            z.avail_out = ciso.block_size;
            z.next_in   = block_buf2;
            status = inflate(&z, Z_FULL_FLUSH);
            if (status != Z_STREAM_END)
            {
                // free memory
                if(index_buf) free(index_buf);
                if(block_buf1) free(block_buf1);
                if(block_buf2) free(block_buf2);

                return false;
            }
            cmp_size = ciso.block_size - z.avail_out;
            if(cmp_size != (long)ciso.block_size)
            {
                // free memory
                if(index_buf) free(index_buf);
                if(block_buf1) free(block_buf1);
                if(block_buf2) free(block_buf2);

                return false;
            }
        }
        // write decompressed block
        if((int)fwrite(block_buf1, 1,cmp_size , fout) != cmp_size)
        {
            // free memory
            if(index_buf) free(index_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);

            return false;
        }

        // term zlib
        if (inflateEnd(&z) != Z_OK)
        {
            // free memory
            if(index_buf) free(index_buf);
            if(block_buf1) free(block_buf1);
            if(block_buf2) free(block_buf2);

            return false;
        }
        //Flush! For testing
        fflush(fout);
    }
    progress.setValue(100);

    // free memory
    if(index_buf) free(index_buf);
    if(block_buf1) free(block_buf1);
    if(block_buf2) free(block_buf2);

    // close files
    fclose(fin);
    fclose(fout);
    
    return true;
}
