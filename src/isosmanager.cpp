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

#include "isosmanager.h"
#include "qpspmanager.h"
#include "optionsmanager.h"
#include "filemanager.h"
#include "isocompressor.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>

ISOsManager::ISOsManager(QPSPManager *application, ISOsWidget &view, OptionsManager &options)
:m_view(view), m_application(application), m_options(options)
{
	m_view.setISOs(this);
    makeConnections();
    scanPSP();
    updatePSPGUI();
    scanComputer();
    updateComputerGUI();    
}


ISOsManager::~ISOsManager()
{
    
}


void ISOsManager::addComputer(QString file, int compression)
{
    if (file.toLower().endsWith(".cso"))
    {
        compression = 0;
    }
    
    if (compression == 0)
    {
        QString destFile = getComputerDir() + "/" + QFileInfo(file).fileName();
        if (QFile::exists(destFile))
        {
            if (m_view.askOverwrite(QFileInfo(file).fileName()))
            {
                if (!FileManager::copyFile(file, destFile, true))
                {
                    m_view.showErrorMessage(tr("Error copying file %1").arg(QFileInfo(file).fileName()));
                }
            }
        }
        else
        {
            if (!FileManager::copyFile(file, destFile, true))
            {
                m_view.showErrorMessage(tr("Error copying file %1").arg(QFileInfo(file).fileName()));
            }
        }
    }
    else
    {
        QString destFile = getComputerDir() + "/" + QFileInfo(file).fileName().left(QFileInfo(file).fileName().length() - 3) + "CSO";
        if (!ISOCompressor::compressISO(file, destFile, compression))
        {
            FileManager::deleteFile(destFile);
            m_view.showErrorMessage(tr("Error compressing ISO, check space"));
        }
    }
    
    scanComputer();
    updateComputerGUI();
}


void ISOsManager::addPSP(QString file, int compression)
{
    if (file.toLower().endsWith(".cso"))
    {
        compression = 1;
    }
    
    if (compression == 1)
    {
        QString destFile = getPSPDir() + "/" + QFileInfo(file).fileName();
        if (QFile::exists(destFile))
        {
            if (m_view.askOverwrite(QFileInfo(file).fileName()))
            {
                if (!FileManager::copyFile(file, destFile, true))
                {
                    m_view.showErrorMessage(tr("Error copying file %1").arg(QFileInfo(file).fileName()));
                }
            }
        }
        else
        {
            if (!FileManager::copyFile(file, destFile, true))
            {
                m_view.showErrorMessage(tr("Error copying file %1").arg(QFileInfo(file).fileName()));
            }
        }
    }
    else
    {
        QString destFile = getPSPDir() + "/" + QFileInfo(file).fileName().left(QFileInfo(file).fileName().length() - 3) + "CSO";
        //TODO: compress the iso
    }
    
    scanPSP();
    updatePSPGUI();
}


void ISOsManager::delComputer(QList<int> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (!FileManager::deleteFile(getComputerDir() + "/" + m_computerList.at(list.at(i))))
        {
            m_view.showErrorMessage(tr("Error deleting %1").arg(m_computerList.at(list.at(i))));
        }
    }
    scanComputer();
    updateComputerGUI();
}


void ISOsManager::delPSP(QList<int> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (!FileManager::deleteFile(getPSPDir() + "/" + m_PSPList.at(list.at(i))))
        {
            m_view.showErrorMessage(tr("Error deleting %1").arg(m_PSPList.at(list.at(i))));
        }
    }
    scanPSP();
    updatePSPGUI();
}


void ISOsManager::moveToPSP(QList<int> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (m_PSPList.contains(m_computerList.at(list.at(i))))
        {
            if (m_view.askOverwrite(m_computerList.at(list.at(i))))
            {
                if (!FileManager::copyFile(getComputerDir() + "/" + m_computerList.at(list.at(i)), getPSPDir() + "/" + m_computerList.at(list.at(i)), true))
                {
                    m_view.showErrorMessage(tr("Error copying %1").arg( m_computerList.at(list.at(i))));
                }
            }
        }
        else
        {
            if (!FileManager::copyFile(getComputerDir() + "/" + m_computerList.at(list.at(i)), getPSPDir() + "/" + m_computerList.at(list.at(i)), true))
            {
                m_view.showErrorMessage(tr("Error copying %1").arg(m_computerList.at(list.at(i))));
            }
        }
    }
    scanPSP();
    updatePSPGUI();
}


void ISOsManager::moveToComputer(QList<int> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (m_computerList.contains(m_PSPList.at(list.at(i))))
        {
            if (m_view.askOverwrite(m_PSPList.at(list.at(i))))
            {
                if (!FileManager::copyFile(getPSPDir() + "/" + m_PSPList.at(list.at(i)), getComputerDir() + "/" + m_PSPList.at(list.at(i)), true))
                {
                    m_view.showErrorMessage(tr("Error copying %1").arg(m_PSPList.at(list.at(i))));
                }
            }
        }
        else
        {
            if (!FileManager::copyFile(getPSPDir() + "/" + m_PSPList.at(list.at(i)), getComputerDir() + "/" + m_PSPList.at(list.at(i)), true))
            {
                m_view.showErrorMessage(tr("Error copying %1").arg(m_PSPList.at(list.at(i))));
            }
        }
    }
    scanComputer();
    updateComputerGUI();
}


void ISOsManager::scanComputer()
{
    m_computerList.clear();
    if(m_options.isBackupDirSet())
    {
    		initComputerDir();
        scanDir(getComputerDir(), m_computerList);
    }
}


void ISOsManager::scanPSP()
{
    m_PSPList.clear();
    if(m_options.isPSPDirSet())
    {
    		initPSPDir();
        scanDir(getPSPDir(), m_PSPList);
        emit PSPDirChanged();
    }
}


void ISOsManager::updateComputerGUI()
{
    QList< QPair<QString, qint64> > list;
    for (int i = 0; i < m_computerList.size(); i++)
    {
        QPair<QString, qint64> p(m_computerList.at(i), QFileInfo(getComputerDir() + "/" + m_computerList.at(i)).size());
        list.append(p);
    }
    m_view.setComputer(list);
}


void ISOsManager::updatePSPGUI()
{
    QList< QPair<QString, qint64> > list;
    for (int i = 0; i < m_PSPList.size(); i++)
    {
        QPair<QString, qint64> p(m_PSPList.at(i), QFileInfo(getPSPDir() + "/" + m_PSPList.at(i)).size());
        list.append(p);
    }
    m_view.setPSP(list);
}


void ISOsManager::scanDir(QString dir, QList<QString> &list)
{
    qDebug() << "Scanning" << dir << "for iso files";
    QFileInfo fi(dir);
    if (!fi.isDir())
    {
        return;
    }
    
    QDir directory(dir);
    
    QFileInfoList dirList = directory.entryInfoList(QDir::Files | QDir::NoSymLinks);
    for (int i = 0; i < dirList.size(); ++i)
    {
        if (dirList.at(i).filePath().toLower().endsWith("iso") || dirList.at(i).filePath().toLower().endsWith("cso"))
        {
            list.append(dirList.at(i).fileName());
        }
    }
}


void ISOsManager::makeConnections()
{
    connect(&m_options, SIGNAL(PSPDirChanged()), this, SLOT(scanPSPSlot()));
    connect(&m_options, SIGNAL(backupsDirChanged()), this, SLOT(scanComputerSlot()));
    connect(m_application, SIGNAL(PSPDirSizeAvailable(quint64)), this, SLOT(updatePSPSize(quint64)));
}


void ISOsManager::initComputerDir()
{
    if (m_options.isBackupDirSet())
    {
        QDir isoDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = isoDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
        		qDebug() << dirList.at(i).fileName();
            if (dirList.at(i).fileName().toLower() == "isos")
            {
                return;
            }
        }
        //Applications dir not found, we create it
        if (!isoDir.mkdir("isos"))
        {
            //TODO: exception
        }
    }
}


void ISOsManager::initPSPDir()
{
    if (m_options.isPSPDirSet())
    {
        QDir isoDir(m_options.getPSPDir());
        QFileInfoList dirList;
        dirList = isoDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
        		qDebug() << dirList.at(i).fileName();
            if (dirList.at(i).fileName().toLower() == "iso")
            {
                return;
            }
        }
        if (!isoDir.mkdir("ISO"))
        {
            //TODO: exception
        }
    }
}

 
void ISOsManager::scanPSPSlot()
{
    scanPSP();
    updatePSPGUI();
}


void ISOsManager::scanComputerSlot()
{
    scanComputer();
    updateComputerGUI();
}


QString ISOsManager::getComputerDir()
{
    if (m_options.isBackupDirSet())
    {
        QDir isoDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = isoDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
            if (dirList.at(i).fileName().toLower() == "isos")
            {
                return dirList.at(i).filePath();
            }
        }
    }
    
    return "";
}


QString ISOsManager::getPSPDir()
{
    if (m_options.isPSPDirSet())
    {
        QDir isoDir(m_options.getPSPDir());
        QFileInfoList dirList;
        dirList = isoDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
            if (dirList.at(i).fileName().toLower() == "iso")
            {
                return dirList.at(i).filePath();
            }
        }
    }
    return "";
}


void ISOsManager::updatePSPSize(quint64 size)
{
    m_view.setPSPSize(size);
}
