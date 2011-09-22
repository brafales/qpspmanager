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
 
#include "filemanager.h"
#include "fsusage.h"

#include <QProgressDialog>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QApplication>

static const int BLOCK_SIZE = 10000; //1MB

//TODO: change bool returns to voids and use exceptions

bool FileManager::copyFiles(QDir outputDir, QFileInfoList dirList, bool progressEnabled)
{
    QProgressDialog progress;
    if (progressEnabled)
    {
        progress.setWindowModality(Qt::ApplicationModal);
        progress.setMinimum(0);
        progress.setMaximum(dirList.size());
        progress.setCancelButton(0);
    }
    for (int i = 0; i < dirList.size(); i++)
    {
        if(progressEnabled)
        {
            progress.setValue(i + 1);
            progress.setLabelText(progress.tr("Copying %1").arg(dirList[i].path()));
        }
        qApp->processEvents();
        if (dirList[i].isDir())
        {
            //Create the dir and copy the files recursively
            QDir newOutDir((outputDir.absolutePath() + "/" + dirList[i].fileName()));
            if (!newOutDir.mkdir(newOutDir.absolutePath()))
            {
                if (progressEnabled)
                {
                    progress.setValue(dirList.size());
                }
                return false;
            }
            QDir subdir(dirList[i].absoluteFilePath());
            QFileInfoList subdirList = subdir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
            if (!copyFiles(newOutDir, subdirList))
            {
                deleteDir(newOutDir.absolutePath());
                if (progressEnabled)
                {
                    progress.setValue(dirList.size());
                }
                return false;
            }
        }
        else
        {
            //Copy the file (assumes output path is already created)
            if (!copyFile(dirList[i].absoluteFilePath(), outputDir.absolutePath() + "/" + dirList[i].fileName()))
            {
                if (progressEnabled)
                {
                    progress.setValue(dirList.size());
                }
                return false;
            }
        }
    }
    if (progressEnabled)
    {
        progress.setValue(dirList.size());
    }
    return true;
}


bool FileManager::copyFile(QString inputFile, QString outputFile, bool progressEnabled)
{
    qDebug() << "Copying" << inputFile << "to" << outputFile;
    QFile inFile(inputFile);
    QFile outFile(outputFile);
    if ((!inFile.open(QIODevice::ReadOnly)) || (!outFile.open(QIODevice::WriteOnly)))
    {
        return false;
    }
    
    //We'll use a block size and copy block to block
    int numBlocks = inFile.size() / BLOCK_SIZE;
    char data[BLOCK_SIZE];
    int bytes = 0;
    
    QProgressDialog progress;
    if (progressEnabled)
    {
        progress.setWindowModality(Qt::ApplicationModal);
        progress.setMinimum(0);
        progress.setMaximum(numBlocks);
        progress.setCancelButton(0);
        progress.setLabelText(progress.tr("Copying %1").arg(QFileInfo(inFile.fileName()).fileName()));
    }
    
    for (int i = 0; i < numBlocks; i++)
    {
        if (progressEnabled)
        {
            progress.setValue(i + 1);
        }
        qApp->processEvents();
        if (inFile.read(data, BLOCK_SIZE) == -1)
        {
            outFile.remove();
            if (progressEnabled)
            {
                progress.setValue(numBlocks);
            }
            return false;
        }
        if (outFile.write(data, BLOCK_SIZE) == -1)
        {
            outFile.remove();
            if (progressEnabled)
            {
                progress.setValue(numBlocks);
            }
            return false;
        }
        bytes += BLOCK_SIZE;
    }
    if (inFile.read(data, inFile.size() - bytes) != (int)inFile.size() - bytes)
    {
        outFile.remove();
        if (progressEnabled)
        {
            progress.setValue(numBlocks);
        }
        return false;
    }
    if (outFile.write(data, inFile.size() - bytes) != (int)inFile.size() - bytes)
    {
        outFile.remove();
        if (progressEnabled)
        {
            progress.setValue(numBlocks);
        }
        return false;
    }
    inFile.close();
    outFile.close();
    
    if (progressEnabled)
    {
        progress.setValue(numBlocks);
    }
    return true;
}


//Deletes de directory recursively. Returns FALSE if any file
//or dir fails to be removed
bool FileManager::deleteDir(QString directory, bool progressEnabled)
{
    qDebug() << "Deleting" << directory;
    QDir dir(directory);
    if (!dir.exists(directory))
    {
        return true;
    }
    QFileInfoList dirList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    
    QProgressDialog progress;
    if (progressEnabled)
    {
        progress.setWindowModality(Qt::ApplicationModal);
        progress.setMinimum(0);
        progress.setMaximum(dirList.size());
        progress.setCancelButton(0);
    }
    
    for (int i = 0; i < dirList.size(); i++)
    {
        if(progressEnabled)
        {
            progress.setValue(i + 1);
            progress.setLabelText(progress.tr("Deleting %1").arg(dirList[i].path()));
        }
        qApp->processEvents();
        if (dirList.at(i).isDir())
        {
            if (deleteDir(dirList.at(i).absoluteFilePath()))
            {
                dir.rmpath(dirList.at(i).absoluteFilePath());
            }
            else
            {
                qDebug() << "Error deleting" << dirList.at(i).absoluteFilePath();
                progress.setValue(dirList.size());
                return false;
            }
        }
        else
        {
            QFile fileToRemove(dirList.at(i).absoluteFilePath());
            qDebug() << "Removing" << dirList.at(i).absoluteFilePath();
            if (!fileToRemove.remove())
            {
                qDebug() << "Error remove" << dirList.at(i).absoluteFilePath();
                progress.setValue(dirList.size());
                return false;
            }
        }
    }
    
    dir.rmdir(directory);
    progress.setValue(dirList.size());
    
    return !(dir.exists(directory));
}


//Copies a source folder to a destination folder
bool FileManager::copyFolder(QDir source, QDir dest, bool progressEnabled)
{
    qDebug() << "Copying" << source.absolutePath() << "to" << dest.absolutePath();
    if (!dest.exists(dest.absolutePath()))
    {
        dest.mkpath(dest.absolutePath());
    }

    QFileInfoList dirList = source.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    if (copyFiles(dest, dirList, progressEnabled))
    {
        return true;
    }
    else
    {
        dest.rmdir(dest.absolutePath());
        return false;
    }
}


bool FileManager::deleteFile(QString file, bool progressEnabled)
{
    qDebug() << "Deleting file" << file;
    QFile byeFile(file);
    return byeFile.remove();
}

int FileManager::getFSUse(QString fsRoot, bool progressEnabled)
{
    return FSUsage::getFSUsage(fsRoot);
}


//Returns the size of the file or dir
qint64 FileManager::getPathSize(QString path, bool progressEnabled)
{
    QFileInfo file(path);
    if (file.isFile())
    {
        return file.size();
    }
    else
    {
        qint64 size = 0;
        QDir dir(path);
        QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (int i = 0; i < fileList.size(); i++)
        {
            size += getPathSize(fileList.at(i).absoluteFilePath());
        }
        return size;
    }
}


bool FileManager::touchFile(QString fileName)
{
    qDebug() << "Touching" << fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }
    file.close();
    return true;
}
