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
 
#include "pspapplication.h"
#include "filemanager.h"
#include "firmwares.h"
#include "pspendian.h"
#include <QFileInfo>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QDebug>

typedef struct  __attribute__((packed))
{
    uint32_t signature;
    uint32_t version;
    uint32_t fields_table_offs;
    uint32_t values_table_offs;
    int32_t nitems;
} SFOHeader;

typedef struct __attribute__((packed))
{
    uint16_t field_offs;
    unsigned char  unk;
    unsigned char  type; // 0x2 -> string, 0x4 -> number
    uint32_t length;
    uint32_t size;
    uint16_t val_offs;
    uint16_t unk4;
} SFODir;


//WARNING: if we use dinamic memory in the future be aware that
//this constructor throws an exception
 PSPApplication::PSPApplication(QString path)
 {
     //Initial checks. We have to make sure the file is a PBP file
     if (!isPBPFile(path))
     {
         throw QPSPManagerPBPException();
     }
     
     //Now we have to know if the PBP is a single file or it's a multiple one
     if (isSinglePBP(path))
     {
         m_type = Single;
         m_path = path;
     }
     else
     {
         m_type = Multiple;
         m_path = getMultiplePath(path);
     }
     
     //Now we get the rest of the info: icon and name
     m_name = getAppName(m_path);
     m_icon = getAppIcon(m_path);
     
     //qDebug() << "Application created:" << m_path << m_name;
     //qDebug() << m_icon.width() << m_icon.height();
 }
 
 PSPApplication::~PSPApplication()
 {}
 
PSPApplication::ApplicationType PSPApplication::getType() const
{
    return m_type;
}


QString PSPApplication::getPath() const
{
    return m_path;
}


QString PSPApplication::getName() const
{
    return m_name;
}


QPixmap PSPApplication::getIcon() const
{
    return m_icon;
}


//Returns true if the file is a valid PBP file
bool PSPApplication::isPBPFile(QString file)
{
    //First we check this is a file
    QFileInfo fi(file);
    if (!fi.isFile())
    {
        return false;
    }
    
    //We have a file, we check the signature to know if it's a PBP file or not
    if (!isPBPSignature(file))
    {
        return false;
    }
    
    return true;
}

//Return true if the signature of the file is the one from a PBP file
bool PSPApplication::isPBPSignature(QString file)
{
    bool isPBPFile = true;
    QFile inFile(file);
    
    if (!(inFile.open(QIODevice::ReadOnly)))
    {
        isPBPFile = false; //Cannot open file for reading
    }
    else
    {
        QDataStream inStream(&inFile);
    
        //Signature
        char signature[4];
        inStream.readRawData((char *)(signature), sizeof(signature));

        if ((signature[0] != 0x00) || (signature[1] != 0x50) || (signature[2] != 0x42) || (signature[3] != 0x50))
        {
            isPBPFile = false; //Not a PBP file
        }
    
        inFile.close();
    }
    
    return isPBPFile;
}


//Returns true if the file is from a single PBP
//Returns false if it's from a multiple PBP
bool PSPApplication::isSinglePBP(QString file)
{
    //We simply do a check using file structure. If the structure is fucked up
    //this will not work. We try to find both the SCE and old plain % directory
    //structure. If another kind of structure is used, bad luck! Feel free to
    //program it yourself ;)
    QDir dir(file);
    
    if (!dir.cdUp()) //the file has not a "parent" so it has to be a single pbp
    {
        return true;
    }
    
    if (isSCEMultiple(dir) || isPercentMultiple(dir))
    {
        return false;
    }
    
    //We could do some more checking here like finding if both files
    //have the same application name. Maybe in the future
    return true;
}


bool PSPApplication::isSCEMultiple(QDir dir)
{
    //Get the dir name of the pbp container
    QString dirName = dir.absolutePath();
    
    //Set the name of the other dir
    QString otherName;
    if (dirName.startsWith("%__SCE__"))
    {
        otherName = dirName.right(dirName.length() -1);
    }
    else if (dirName.startsWith("__SCE__"))
    {
        otherName = dirName;
        otherName.append("%");
    }
    else
    {
        return false;
    }
    
    //Now we have to check that both the otherName dir exists
    //and it has a valid PBP file
    QDir otherDir(otherName);
    if (!otherDir.exists())
    {
        return false;
    }
    
    //Find the eboot.pbp file inside the dir
    QFileInfoList files;
    files = otherDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
    for (int i = 0; i < files.size(); ++i) 
    {
        if (files.at(i).fileName().toLower() == "eboot.pbp")
        {
            if (isPBPSignature(files.at(i).filePath()))
            {
                return true; //We have the correct directory and contains a correct pbp file
            }
        }
    }
    
    return false;
}


bool PSPApplication::isPercentMultiple(QDir dir)
{
    //Get the dir name of the pbp container
    QString dirName = dir.absolutePath();
    
    //Set the name of the other dir
    QString otherName = dirName;
    if (dirName.endsWith("%"))
    {
        otherName.resize(otherName.length() -  1);
    }
    else
    {
        otherName.append("%");
    }
    
    
    //Now we have to check that both the otherName dir exists
    //and it has a valid PBP file
    QDir otherDir(otherName);
    if (!otherDir.exists())
    {
        return false;
    }
    
    //Find the eboot.pbp file inside the dir
    QFileInfoList files;
    files = otherDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
    for (int i = 0; i < files.size(); ++i) 
    {
        if (files.at(i).fileName().toLower() == "eboot.pbp")
        {
            if (isPBPSignature(files.at(i).filePath()))
            {
                return true; //We have the correct directory and contains a correct pbp file
            }
        }
    }
    
    return false;
}


//Given a file path, gets the pbp path if this one is not the one we want
QString PSPApplication::getMultiplePath(QString path)
{
    //We want the non stripped directory
    QDir dir(path);
    
    if (!dir.cdUp())
    {
        throw QPSPManagerPBPException();
    }
    
    QString destDirectory = dir.absolutePath();
    
    if (dir.absolutePath().startsWith("%__SCE__")) //We need to remove the initial %
    {
        destDirectory = dir.absolutePath().right(dir.absolutePath().length() - 1);
    }
    
    if (dir.absolutePath().endsWith("%")) //We need to remove the final %
    {
        destDirectory = dir.absolutePath().left(dir.absolutePath().length() - 1);
    }
    
    //We have the same dir as entered because all other possibilities
    //under the precondition that this is a multiple PBP file, are pointing
    //to the directory we want.
    //We need to add the PBP
    //Find the eboot.pbp file inside the dir
    QFileInfoList files;
    QDir targetDir(destDirectory);
    files = targetDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
    for (int i = 0; i < files.size(); ++i) 
    {
        if (files.at(i).fileName().toLower() == "eboot.pbp")
        {
            destDirectory + "/" + files.at(i).fileName();
            return destDirectory;
        }
    }
    
    //We should never reach here, if we do, we're in trouble :)
    return "";
}


QString PSPApplication::getAppName(QString filename)
{
    QFile file(filename);
    
    if(!(file.open(QIODevice::ReadOnly)))
    {
        throw QPSPManagerPBPException();
    }
    
    QDataStream stream(&file);

    quint8 signature[4];
    //Signature
    for (int i = 0; i < 4; i++)
    {
        stream >> signature[i];
    }

    //Version
    quint32 version;
    stream >> version;

    //Files offset
    uint32_t offset[8];
    stream.readRawData((char *)(offset), sizeof(offset));

    //offset[7] = file.size();

    //For endianness problems
    for (int i = 0; i < 8; i++)
    {
        offset[i] = Endian::le32(offset[i]);
    }

    //PARAM.SFO
    int paramSFOSize = offset[1] - offset[0];
    char buffer[paramSFOSize];
    
    if (stream.readRawData(buffer, paramSFOSize) != paramSFOSize)
    {
        throw QPSPManagerPBPException();
    }

    //We have the SFO file in the buffer, now we have to
    //seek the title field on it
    //TODO: document the SFO file format ffs!
    SFOHeader *header = (SFOHeader *)buffer;
    SFODir *entries = (SFODir *)(buffer+sizeof(SFOHeader));
    QString appTitle;
    
    header->fields_table_offs = Endian::le32(header->fields_table_offs);
	header->values_table_offs = Endian::le32(header->values_table_offs);
    header->nitems = Endian::le32(header->nitems);
    for (int i = 0; i < header->nitems; i++)
    {
    		entries[i].field_offs = Endian::le16(entries[i].field_offs);
        if (strcmp(buffer+header->fields_table_offs+entries[i].field_offs, "TITLE") == 0)
        {
        	 entries[i].size = Endian::le32(entries[i].size);
        	 entries[i].val_offs = Endian::le16(entries[i].val_offs);
            char title[entries[i].size];
            strncpy(title, buffer + header->values_table_offs+entries[i].val_offs, entries[i].size);
            appTitle = QString::fromUtf8(title);
        }
    }

    file.close();
    return appTitle;
}


QPixmap PSPApplication::getAppIcon(QString file)
{
    QFile inFile(file);
    
    if (!(inFile.open(QIODevice::ReadOnly)))
    {
        throw QPSPManagerPBPException();
    }
    
    QDataStream inStream( &inFile );
    
    //Signature
    char signature[4];
    inStream.readRawData((char *)(signature), sizeof(signature));

    if ((signature[0] != 0x00) || (signature[1] != 0x50) || (signature[2] != 0x42) || (signature[3] != 0x50))
    {
        throw QPSPManagerPBPException();
    }

    //Version
    unsigned int version;
    inStream >> version;

    //Files offset
    uint32_t offset[8];
    inStream.readRawData((char *)(offset), sizeof(offset));

    //offset[8] = inFile.size();

    //For endianness problems
    for (int i = 0; i < 8; i++)
    {
        offset[i] = Endian::le32(offset[i]);
    }

    //We ned to know where the icon starts and its size
    int iconSize = offset[2] - offset[1];
    int iconOffset = offset[1] - offset[0];
    char buffer[iconSize];
    if (inStream.skipRawData(iconOffset) != iconOffset)
    {
        throw QPSPManagerPBPException();
    }
    
    if (inStream.readRawData(buffer, iconSize) != iconSize)
    {
        throw QPSPManagerPBPException();
    }

    
    //Now we create the icon
    QPixmap result;
    //TODO: why the fuck this needs uchar and not char?
    QByteArray pngArray(buffer, iconSize);
    if (!result.loadFromData(pngArray))
    {
        //TODO:exception?
    }
    
    //Clean up and return
    inFile.close();

    return result;
}

//Copies the application to the specified directory
//using the firmware method given
bool PSPApplication::copyToDir(QString dir, int firmware)
{
    //For now we only have support for 3XX firmware
    if (firmware == FIRMWARE_3XX)
    {
        //We create the new directory and copy the application on it
        QDir appDir(m_path);
        appDir.cdUp();
        if (!FileManager::copyFolder(appDir, QDir(dir), true))
        {
            return false;
        }
        else
        {
            //Update the path
            m_path = dir;
            return true;
        }

    }
    return true;
}
