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
 
#include "video.h"
 
#include <QFileInfo>
#include <QDebug>
#include <QFile>
 
Video::Video(QString file, QString name, int type)
:m_name(name), m_path(file), m_type(type), m_size(QFileInfo(file).size())
{
}

 
QString Video::getName() const
{
    return m_name;
}


QString Video::getPath() const
{
    return m_path;
}


QString Video::getThumbnailPath() const
{
    QFileInfo file(m_path);
    return (file.absolutePath() + "/" + m_name + ".THM");
}


int Video::getType() const
{
    return m_type;
}


qint64 Video::getSize() const
{
    return m_size;
}
