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
 
 #include "savegame.h"
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


Savegame::Savegame(QString path)
:m_path(path)
{
    //qDebug() << "Attempting to create a savegame from" << path;
    if (!QFileInfo(m_path).isDir())
    {
        throw QPSPManagerPBPException();
    }
     
    m_name = getSavegameName(m_path);
    m_icon = getSavegameIcon(m_path);
     
    //qDebug() << "Savegame created:" << m_path << m_name;
    //qDebug() << m_icon.width() << m_icon.height();
}
 
Savegame::~Savegame()
{}


QString Savegame::getPath() const
{
    return m_path;
}


QString Savegame::getName() const
{
    return m_name;
}


QPixmap Savegame::getIcon() const
{
    return m_icon;
}


QString Savegame::getSavegameName(QString path)
{
    QDir dir(path);
    QFileInfoList files;
    files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
    for (int i = 0; i < files.size(); ++i) 
    {
        if (files.at(i).fileName().toLower() == "param.sfo")
        {
            QFile file(files.at(i).filePath());
            if(!(file.open(QIODevice::ReadOnly)))
            {
                throw QPSPManagerPBPException();
            }

            QDataStream stream(&file);
            char buffer[file.size()];
            if (stream.readRawData(buffer, file.size()) != file.size())
            {
                throw QPSPManagerPBPException();
            }
            SFOHeader *header = (SFOHeader *)buffer;
            SFODir *entries = (SFODir *)(buffer+sizeof(SFOHeader));
            QString appTitle;

            //qDebug() << "File size:" << file.size();
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
    }
    
    throw QPSPManagerPBPException();
}


QPixmap Savegame::getSavegameIcon(QString path)
{
    QDir dir(path);
    QFileInfoList files;
    files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
    for (int i = 0; i < files.size(); ++i) 
    {
        if (files.at(i).fileName().toLower() == "icon0.png")
        {
            QPixmap icon;
            if (icon.load(files.at(i).filePath()))
            {
                return icon;
            }
            else
            {
                throw QPSPManagerPBPException();
            }
        }
    }
    
    throw QPSPManagerPBPException();
}
