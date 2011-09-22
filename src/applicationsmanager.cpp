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
#include "applicationsmanager.h"
#include "applicationswidget.h"
#include "optionsmanager.h"
#include "filemanager.h"
#include "qpspmanager.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

ApplicationsManager::ApplicationsManager(QPSPManager *application, ApplicationsWidget &view, OptionsManager &options)
:m_view(view), m_application(application), m_options(options)
{
	m_view.setApplications(this);
    makeConnections();
    scanPSP();
    updatePSPGUI();
    scanComputer();
    updateComputerGUI();
}


ApplicationsManager::~ApplicationsManager()
{
}

void ApplicationsManager::scanComputer()
{
    initComputerDir();
    m_appComputerList.clear();
    if(m_options.isBackupDirSet())
    {
        QString computerAppDir = m_options.getBackupDir() + "/applications";
        scanDir(computerAppDir, m_appComputerList);
    }
}


void ApplicationsManager::scanComputerSlot()
{
    scanComputer();
    updateComputerGUI();
}


void ApplicationsManager::scanPSP()
{
    m_appPSPList.clear();
    if(m_options.isPSPDirSet())
    {
    		initPSPDir();
        QString PSPDir = m_options.getPSPDir();
        QString PSPAppDir = getAppDirectory(PSPDir, m_options.getFirmware());
        scanDir(PSPAppDir, m_appPSPList);
        emit PSPDirChanged();
    }
}


void ApplicationsManager::scanPSPSlot()
{
    scanPSP();
    updatePSPGUI();
}


void ApplicationsManager::updateComputerGUI()
{
    m_view.setComputerApplications(m_appComputerList); 
}


void ApplicationsManager::updatePSPGUI()
{
    m_view.setPSPApplications(m_appPSPList);
}


//Scans a directory for PSP Applications and adds them on the list
void ApplicationsManager::scanDir(QString dir, QList<PSPApplication> &list)
{
    QFileInfo fi(dir);
    if (!fi.isDir())
    {
        return;
    }
    
    QDir directory(dir);
    
    QFileInfoList dirList = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < dirList.size(); ++i)
    {
        QDir appDir(dirList.at(i).filePath());
        QFileInfoList appDirList = appDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
        for (int j = 0; j < appDirList.size(); j++)
        {
            if (appDirList.at(j).fileName().toLower() == "eboot.pbp")
            {
            		try
            		{
                		PSPApplication app(appDirList.at(j).filePath());
                		//We only add the app if it's a new one
                		//This is the easiest way to get rid of the
                		//multiple pbp problem
                		if (!findApp(list, app.getPath()))
                		{
                    		list.append(app);
                		}
                }
                catch (QPSPManagerPBPException e)
        				{
            				m_view.showErrorMessage(tr("%1 doesn't seem to be a valid application").arg(appDirList.at(j).filePath()));
        				}
            }
        }
    }
}


QString ApplicationsManager::getAppDirectory(QString PSPDirectory, int firmware)
{
    QFileInfo fi(PSPDirectory);
    if (!fi.isDir())
    {
        return "";
    }
    
    QString appDir;
    
    if (firmware == FIRMWARE_3XX)
    {
        appDir = "game150";
    }
    else
    {
        appDir = "game";
    }
    
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
                if (pspDirList.at(j).fileName().toLower() == appDir)
                {
                    //Bingo!
                    return pspDirList.at(j).filePath();
                }
            }
        }
    }
    
    return "";
}


bool ApplicationsManager::findApp(QList<PSPApplication> &list, QString path)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].getPath() == path)
        {
            return true;
        }
    }
    
    return false;
}

void ApplicationsManager::makeConnections()
{
    connect(&m_options, SIGNAL(PSPDirChanged()), this, SLOT(scanPSPSlot()));
    connect(&m_options, SIGNAL(backupsDirChanged()), this, SLOT(scanComputerSlot()));
    connect(m_application, SIGNAL(PSPDirSizeAvailable(quint64)), this, SLOT(updatePSPSize(quint64)));
}

void ApplicationsManager::initComputerDir()
{
    if (m_options.isBackupDirSet())
    {
        QDir appDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
            if (dirList.at(i).fileName().toLower() == "applications")
            {
                return;
            }
        }
        //Applications dir not found, we create it
        if (!appDir.mkdir("applications"))
        {
            //TODO: exception
        }
    }
}


void ApplicationsManager::initPSPDir()
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
                    if (dirList.at(i).fileName().toLower() == "game150")
                    {
                        return;
                    }
                }
                //Applications dir not found, we create it
                if (!pspDir.mkdir("GAME150"))
                {
                    //TODO: exception
                }
            }
            return;
        }
        appDir.mkpath("PSP/GAME150");
    }
}


QString ApplicationsManager::getComputerDir()
{
    if (m_options.isBackupDirSet())
    {
        QDir appDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
        		qDebug() << dirList.at(i).fileName();
            if (dirList.at(i).fileName().toLower() == "applications")
            {
                return dirList.at(i).filePath();
            }
        }
    }
    
    return "";
}


void ApplicationsManager::addComputerApplication(QString file)
{
    if (m_options.isBackupDirSet())
    {
        try
        {
            PSPApplication app(file);
            //Move the added app to the computer directory
            QString appDir = getComputerDir() + "/" + app.getName();
            qDebug() << appDir;
            //TODO: check not only the dir but the PBP file too
            if (QDir(getComputerDir() + "/" + app.getName()).exists())
            {
                if (!m_view.askAppOverwrite(app.getName()))
                {
                    return;
                }
            }
            
            if (!app.copyToDir(getComputerDir() + "/" + app.getName(), FIRMWARE_3XX))
            {
                m_view.showErrorMessage(tr("Error moving the application to the QPSPManager application folder. Check space and permissions"));
                return;
            }
            scanComputer();
            updateComputerGUI();
        }
        catch (QPSPManagerPBPException e)
        {
            m_view.showErrorMessage(e.getMessage());
            return;
        }
    }
    else
    {
        m_view.showErrorMessage(tr("Please, set the Backup Directory in the options menu"));
    }
}


void ApplicationsManager::addPSPApplication(QString file)
{
    if (m_options.isPSPDirSet())
    {
        try
        {
            PSPApplication app(file);
            //Move the added app to the psp directory
            QString appDir = getAppDirectory(m_options.getPSPDir(), m_options.getFirmware()) + "/" + app.getName();
            qDebug() << appDir;
            //TODO: check not only the dir but the PBP file too
            if (QDir(appDir).exists())
            {
                if (!m_view.askAppOverwrite(app.getName()))
                {
                    return;
                }
            }
            
            if (!app.copyToDir(appDir, m_options.getFirmware()))
            {
                m_view.showErrorMessage(tr("Error moving the application to the PSP. Check space, location and permissions"));
                return;
            }
            scanPSP();
            updatePSPGUI();
        }
        catch (QPSPManagerPBPException e)
        {
            m_view.showErrorMessage(e.getMessage());
            return;
        }
    }
    else
    {
        m_view.showErrorMessage(tr("Please, set the Backup Directory in the options menu"));
    }
}


void ApplicationsManager::delComputerApplications(QList<int> list)
{
    //We delete the apps from the FS and we rescan the folder
    for (int i = 0; i < list.size(); i++)
    {
        QDir app(m_appComputerList.at(list.at(i)).getPath());
        app.cdUp();
        if (!FileManager::deleteDir(app.absolutePath()))
        {
            m_view.showErrorMessage(tr("Error deleting %1").arg(m_appComputerList.at(list.at(i)).getName()));
        }
    }
    scanComputer();
    updateComputerGUI();
}


void ApplicationsManager::delPSPApplications(QList<int> list)
{
    //We delete the apps from the FS and we rescan the folder
    for (int i = 0; i < list.size(); i++)
    {
        QDir app(m_appPSPList.at(list.at(i)).getPath());
        app.cdUp();
        if (!FileManager::deleteDir(app.absolutePath()))
        {
            m_view.showErrorMessage(tr("Error deleting %1").arg(m_appPSPList.at(list.at(i)).getName()));
        }
    }
    scanPSP();
    updatePSPGUI();
}


void ApplicationsManager::moveApplicationsToPSP(QList<int> list)
{
    if (m_options.isPSPDirSet())
    {
        for (int i = 0; i < list.size(); i++)
        {
            QDir sourceDir(m_appComputerList.at(list.at(i)).getPath());
            sourceDir.cdUp();
            QDir destDir(getAppDirectory(m_options.getPSPDir(), m_options.getFirmware()) + "/" + sourceDir.dirName());
            if (destDir.exists())
            {
                if (m_view.askAppOverwrite(m_appComputerList.at(list.at(i)).getName()))
                {
                    FileManager::deleteDir(destDir.absolutePath());
                    if (!FileManager::copyFolder(sourceDir, destDir, true))
                    {
                        m_view.showErrorMessage(tr("Error moving %1").arg(m_appComputerList.at(list.at(i)).getName()));
                    }
                }
            }
            else
            {
                if (!FileManager::copyFolder(sourceDir, destDir, true))
                {
                    m_view.showErrorMessage(tr("Error moving %1").arg(m_appComputerList.at(list.at(i)).getName()));
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


void ApplicationsManager::moveApplicationsToComputer(QList<int> list)
{
    if (m_options.isBackupDirSet())
    {
        for (int i = 0; i < list.size(); i++)
        {
            QDir sourceDir(m_appPSPList.at(list.at(i)).getPath());
            sourceDir.cdUp();
            QDir destDir(getComputerDir() + "/" + sourceDir.dirName());
            if (destDir.exists())
            {
                if (m_view.askAppOverwrite(m_appPSPList.at(list.at(i)).getName()))
                {
                    FileManager::deleteDir(destDir.absolutePath());
                    if (!FileManager::copyFolder(sourceDir, destDir, true))
                    {
                        m_view.showErrorMessage(tr("Error moving %1").arg(m_appPSPList.at(list.at(i)).getName()));
                    }
                }
            }
            else
            {
                if (!FileManager::copyFolder(sourceDir, destDir, true))
                {
                    m_view.showErrorMessage(tr("Error moving %1").arg(m_appPSPList.at(list.at(i)).getName()));
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


void ApplicationsManager::updatePSPSize(quint64 size)
{
    m_view.setPSPSize(size);
}
