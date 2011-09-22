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
#include "backupsmanager.h"
#include "backupswidget.h"
#include "optionsmanager.h"
#include "filemanager.h"
#include "qpspmanager.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

BackupsManager::BackupsManager(QPSPManager *application, BackupsWidget &view, OptionsManager &options)
:m_view(view), m_application(application), m_options(options)
{
	m_view.setBackups(this);
    makeConnections();
    scanPSP();
    updatePSPGUI();
    scanComputer();
    updateComputerGUI();
}


BackupsManager::~BackupsManager()
{
}

void BackupsManager::scanComputer()
{
    initComputerDir();
    m_saveComputerList.clear();
    if(m_options.isBackupDirSet())
    {
        QString computerSaveDir = m_options.getBackupDir() + "/savegames";
        scanDir(computerSaveDir, m_saveComputerList);
    }
}


void BackupsManager::scanComputerSlot()
{
    scanComputer();
    updateComputerGUI();
}


void BackupsManager::scanPSP()
{
    m_savePSPList.clear();
    if(m_options.isPSPDirSet())
    {
    		initPSPDir();
        QString PSPDir = m_options.getPSPDir();
        QString PSPSaveDir = getSavegameDirectory(PSPDir);
        scanDir(PSPSaveDir, m_savePSPList);
        emit PSPDirChanged();
    }
}


void BackupsManager::scanPSPSlot()
{
    scanPSP();
    updatePSPGUI();
}


void BackupsManager::updateComputerGUI()
{
    m_view.setComputerBackups(m_saveComputerList); 
}


void BackupsManager::updatePSPGUI()
{
    m_view.setPSPBackups(m_savePSPList);
}


//Scans a directory for PSP savegames and adds them on the list
void BackupsManager::scanDir(QString dir, QList<Savegame> &list)
{
    qDebug() << "Scanning" << dir << "for savegames";
    QFileInfo fi(dir);
    if (!fi.isDir())
    {
        return;
    }
    
    QDir directory(dir);
    
    QFileInfoList dirList = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < dirList.size(); ++i)
    {
        try
        {
            Savegame s(dirList.at(i).filePath());
            list.append(s);
        }
        catch(QPSPManagerPBPException e)
        {
            qDebug() << dirList.at(i).filePath() << "is not a savegame";
        }
    }
}


QString BackupsManager::getSavegameDirectory(QString PSPDirectory)
{
    QFileInfo fi(PSPDirectory);
    if (!fi.isDir())
    {
        return "";
    }
    
    QString saveDir = "savedata";
    
    QDir directory(PSPDirectory);

    QFileInfoList dirList;
    dirList = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    
    for (int i = 0; i < dirList.size(); ++i) 
    {
        if (dirList.at(i).fileName().toLower() == "psp")
        {
            QDir pspdir(dirList.at(i).filePath());
            QFileInfoList pspDirList = pspdir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (int j = 0; j < pspDirList.size(); j++)
            {
                if (pspDirList.at(j).fileName().toLower() == saveDir)
                {
                    //Bingo!
                    return pspDirList.at(j).filePath();
                }
            }
        }
    }
    
    return "";
}



void BackupsManager::makeConnections()
{
    connect(&m_options, SIGNAL(PSPDirChanged()), this, SLOT(scanPSPSlot()));
    connect(&m_options, SIGNAL(backupsDirChanged()), this, SLOT(scanComputerSlot()));
    connect(m_application, SIGNAL(PSPDirSizeAvailable(quint64)), this, SLOT(updatePSPSize(quint64)));
}

void BackupsManager::initComputerDir()
{
    if (m_options.isBackupDirSet())
    {
        QDir appDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
        		qDebug() << dirList.at(i).fileName();
            if (dirList.at(i).fileName().toLower() == "savegames")
            {
                return;
            }
        }
        //Applications dir not found, we create it
        if (!appDir.mkdir("savegames"))
        {
            //TODO: exception
        }
    }
}


void BackupsManager::initPSPDir()
{
    if (m_options.isPSPDirSet())
    {
        QDir appDir(m_options.getPSPDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
        		qDebug() << dirList.at(i).fileName();
            if (dirList.at(i).fileName().toLower() == "psp")
            {
                QDir pspDir(dirList.at(i).filePath());
                QFileInfoList pspList;
                pspList = pspDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
                for (int i = 0; i < pspList.size(); ++i) 
                {
                		qDebug() << pspList.at(i).fileName();
                    if (pspList.at(i).fileName().toLower() == "savedata")
                    {
                        return;
                    }
                }
                //Applications dir not found, we create it
                if (!pspDir.mkdir("SAVEDATA"))
                {
                    //TODO: exception
                }
            }
            return;
        }
        appDir.mkpath("PSP/SAVEDATA");
    }
}


QString BackupsManager::getComputerDir()
{
    if (m_options.isBackupDirSet())
    {
        QDir appDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
            if (dirList.at(i).fileName().toLower() == "savegames")
            {
                return dirList.at(i).filePath();
            }
        }
    }
    
    return "";
}


void BackupsManager::addComputerSavegame(QString file)
{
    if (m_options.isBackupDirSet())
    {
        try
        {
            Savegame s(file);
            //Move the added savegame to the computer directory
            QString saveDir = getComputerDir() + "/" + QFileInfo(file).baseName();
            qDebug() << saveDir;

            if (QDir(saveDir).exists())
            {
                if (!m_view.askSaveOverwrite(s.getName()))
                {
                    return;
                }
            }
            
            if (!FileManager::copyFolder(QDir(s.getPath()), QDir(saveDir), true))
            {
                m_view.showErrorMessage(tr("Error moving the savegame to the QPSPManager backups folder. Check space and permissions"));
                return;
            }
            scanComputer();
            updateComputerGUI();
        }
        catch (QPSPManagerPBPException e)
        {
            m_view.showErrorMessage(tr("The selected folder does not seem to be a PSP savegame folder"));
            return;
        }
    }
    else
    {
        m_view.showErrorMessage(tr("Please, set the Backup Directory in the options menu"));
    }
}


void BackupsManager::addPSPSavegame(QString file)
{
    if (m_options.isPSPDirSet())
    {
        try
        {
            Savegame s(file);
            //Move the added savegame to the psp directory
            QString saveDir = getSavegameDirectory(m_options.getPSPDir()) + "/" + QFileInfo(file).baseName();
            qDebug() << saveDir;

            if (QDir(saveDir).exists())
            {
                if (!m_view.askSaveOverwrite(s.getName()))
                {
                    return;
                }
            }
            
            if (!FileManager::copyFolder(QDir(s.getPath()), QDir(saveDir), true))
            {
                m_view.showErrorMessage(tr("Error moving the savegame to the PSP. Check space, location and permissions"));
                return;
            }
            scanPSP();
            updatePSPGUI();
        }
        catch (QPSPManagerPBPException e)
        {
            m_view.showErrorMessage(tr("The selected folder does not seem to be a PSP savegame folder"));
            return;
        }
    }
    else
    {
        m_view.showErrorMessage(tr("Please, set the Backup Directory in the options menu"));
    }
}


void BackupsManager::delComputerSavegames(QList<int> list)
{
    //We delete the savegames from the FS and we rescan the folder
    for (int i = 0; i < list.size(); i++)
    {
        QDir save(m_saveComputerList.at(list.at(i)).getPath());
        if (!FileManager::deleteDir(save.absolutePath()))
        {
            m_view.showErrorMessage(tr("Error deleting %1").arg(m_saveComputerList.at(list.at(i)).getName()));
        }
    }
    scanComputer();
    updateComputerGUI();
}


void BackupsManager::delPSPSavegames(QList<int> list)
{
    //We delete the savegames from the FS and we rescan the folder
    for (int i = 0; i < list.size(); i++)
    {
        QDir save(m_savePSPList.at(list.at(i)).getPath());
        if (!FileManager::deleteDir(save.absolutePath()))
        {
            m_view.showErrorMessage(tr("Error deleting %1").arg(m_savePSPList.at(list.at(i)).getName()));
        }
    }
    scanPSP();
    updatePSPGUI();
}


void BackupsManager::moveSavegamesToPSP(QList<int> list)
{
    if (m_options.isPSPDirSet())
    {
        for (int i = 0; i < list.size(); i++)
        {
            QDir sourceDir(m_saveComputerList.at(list.at(i)).getPath());
            QDir destDir(getSavegameDirectory(m_options.getPSPDir()) + "/" + QDir(m_saveComputerList.at(list.at(i)).getPath()).dirName());
            if (destDir.exists())
            {
                if (m_view.askSaveOverwrite(m_saveComputerList.at(list.at(i)).getName()))
                {
                    FileManager::deleteDir(destDir.absolutePath());
                    if (!FileManager::copyFolder(sourceDir, destDir, true))
                    {
                        m_view.showErrorMessage(tr("Error moving %1").arg(m_saveComputerList.at(list.at(i)).getName()));
                    }
                }
            }
            else
            {
                if (!FileManager::copyFolder(sourceDir, destDir, true))
                {
                    m_view.showErrorMessage(tr("Error moving %1").arg(m_saveComputerList.at(list.at(i)).getName()));
                }
            }
        }
        
        scanPSP();
        updatePSPGUI();
    }
    else
    {
        m_view.showErrorMessage(tr("Please, set the PSP Directory in the options menu"));
    }

}


void BackupsManager::moveSavegamesToComputer(QList<int> list)
{
    if (m_options.isBackupDirSet())
    {
        for (int i = 0; i < list.size(); i++)
        {
            QDir sourceDir(m_savePSPList.at(list.at(i)).getPath());
            QDir destDir(getComputerDir() + "/" + QDir(m_savePSPList.at(list.at(i)).getPath()).dirName());
            if (destDir.exists())
            {
                if (m_view.askSaveOverwrite(m_savePSPList.at(list.at(i)).getName()))
                {
                    FileManager::deleteDir(destDir.absolutePath());
                    if (!FileManager::copyFolder(sourceDir, destDir, true))
                    {
                        m_view.showErrorMessage(tr("Error moving %1").arg(m_savePSPList.at(list.at(i)).getName()));
                    }
                }
            }
            else
            {
                if (!FileManager::copyFolder(sourceDir, destDir, true))
                {
                    m_view.showErrorMessage(tr("Error moving %1").arg(m_savePSPList.at(list.at(i)).getName()));
                }
            }
        }
        scanComputer();
        updateComputerGUI();
    }
    else
    {
        m_view.showErrorMessage(tr("Please, set the Backup Directory in the options menu"));
    }
}


void BackupsManager::updatePSPSize(quint64 size)
{
    m_view.setPSPSize(size);
}
