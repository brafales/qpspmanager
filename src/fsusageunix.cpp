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

#include "fsusage.h"
#include <sys/statvfs.h>
#include <QDebug>

int FSUsage::getFSUsage(QString fsRoot)
{
    struct statvfs s;
    if (statvfs(fsRoot.toUtf8(), &s) != 0)
    	return -1;

    //Get percentage
    quint64 totalBlocks = s.f_blocks;
    quint64 availableBlocks = s.f_bfree;
    
    //qDebug() << "f_blocks" << s.f_blocks;
    //qDebug() << "f_bfree" << s.f_bfree;
    //qDebug() << "f_frsize" << s.f_frsize;

    qreal percentageBlocks = 100 * (qreal)((qreal)availableBlocks/(qreal)totalBlocks);

    return (int)percentageBlocks;
}

quint64 FSUsage::getFSSize(QString fsRoot)
{
    struct statvfs s;
    if (statvfs(fsRoot.toUtf8(), &s) != 0)
    	return 0;
    
    return ((qreal)s.f_blocks / (qreal)(1024 * 1024)) * s.f_frsize;
}


quint64 FSUsage::getFSUsed(QString fsRoot)
{
    struct statvfs s;
    if (statvfs(fsRoot.toUtf8(), &s) != 0)
    	return 0;

    return ((qreal)(s.f_blocks - s.f_bfree) / (qreal)(1024 * 1024)) * s.f_frsize;
}


quint64 FSUsage::getFSFree(QString fsRoot)
{
    struct statvfs s;
    if (statvfs(fsRoot.toUtf8(), &s) != 0)
    	return 0;

    //qDebug() << "f_blocks" << s.f_blocks;
    //qDebug() << "f_bfree" << s.f_bfree;
    //qDebug() << "f_frsize" << s.f_frsize;
    
    return ((qreal)s.f_bfree / (qreal)(1024 * 1024)) * s.f_frsize; 
}
