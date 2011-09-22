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
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFileInfoList>

//Forward declarators
class QWidget;
class QDir;
class QString;

class FileManager
{
public:
    static bool copyFiles(QDir outputDir, QFileInfoList dirList, bool progressEnabled = false);
    static bool copyFile(QString inFile, QString outFile, bool progressEnabled = false);
    static bool deleteDir(QString directory, bool progressEnabled = false);
    static bool copyFolder(QDir source, QDir dest, bool progressEnabled = false);
    static bool deleteFile(QString file, bool progressEnabled = false);
    static int getFSUse(QString fsRoot, bool progressEnabled = false);
    static qint64 getPathSize(QString path, bool progressEnabled = false);
    static bool touchFile(QString fileName);
};

#endif
