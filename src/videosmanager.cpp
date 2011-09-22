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
#include "videosmanager.h"
#include "optionsmanager.h"
#include "qpspmanager.h"
#include "videoswidget.h"
#include "videooptions.h"
#include "videojobmanager.h"
#include "filemanager.h"

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

VideosManager::VideosManager(QPSPManager *application, VideosWidget &view, OptionsManager &options)
:m_view(view), m_application(application), m_options(options), m_encoding(false)
{
 	m_view.setVideos(this);
    initComputerDirs();
    scanPSP();
    updatePSPGUI();
    scanComputer();
    updateComputerGUI();
    m_jobManager = new VideoJobManager(m_jobList, m_options.getFFmpeg());
    makeConnections();
}


VideosManager::~VideosManager()
{
    delete m_jobManager;
}


void VideosManager::addVideoJob(QString inputFile, QString title, int passes, int type, int aspectRatio, int audio, int video)
{
    //VideoJob(QString inputFile, QString outputFile, QString title, int passes, int type, int aspectRatio);
    int vtype;
    QString outputDir;
    if (type != TYPE_SP)
    {
        vtype = TYPE_AVC;
        outputDir = getComputerAVCDir();
    }
    else
    {
        vtype = TYPE_SP;
        outputDir = getComputerSPDir();
    }
    
    if (title.isEmpty())
    {
        title = QFileInfo(inputFile).completeBaseName();
    }
    
    if (findVideo(title, vtype, m_videoComputerList))
    {
        if (!m_view.askOverwrite(title))
        {
            return;
        }
    }
    QString outputFile = outputDir + "/" + title + ".MP4";
    VideoJob v(inputFile, outputFile, title, passes, type, aspectRatio, m_options.getThreads(), audio, video);
    m_jobList.append(v);
    updateJobGUI();
}


void VideosManager::scanComputer()
{
    if (m_options.isBackupDirSet())
    {
        m_videoComputerList.clear();
        scanDir(getComputerSPDir(), "", m_videoComputerList, TYPE_SP);
        scanDir(getComputerAVCDir(), "", m_videoComputerList, TYPE_AVC);
    }
}


void VideosManager::scanPSP()
{
    if (m_options.isPSPDirSet())
    {
    		initPSPDirs();
        m_videoPSPList.clear();
        scanDir(getPSPSPDir(), "M4V", m_videoPSPList, TYPE_SP);
        scanDir(getPSPAVCDir(), "MAQ", m_videoPSPList, TYPE_AVC);
        emit PSPDirChanged();
    }
}


void VideosManager::updateComputerGUI()
{
    m_view.setVideosComputer(m_videoComputerList);
}


void VideosManager::updatePSPGUI()
{
    m_view.setVideosPSP(m_videoPSPList);
}


void VideosManager::updateJobGUI()
{
    m_view.setVideoJobs(m_jobList);
}


void VideosManager::scanDir(QString dir, QString filePrefix, QList<Video> &list, int type)
{
    //We have to do two things here: not only scan for MP4 files but also for
    //info files which may contain the name of the video
    qDebug() << "Scanning" << dir << "for" << filePrefix << "videos";
    QFileInfo fi(dir);
    if (!fi.isDir())
    {
        return;
    }
    
    QDir directory(dir);
    QStringList filters;
    filters << "*.MP4" << "*.mp4";
    QStringList filtersInfo;
    filtersInfo << "*.INFO" << "*.info";
    QFileInfoList dirList = directory.entryInfoList(filters, QDir::Files | QDir::NoSymLinks, QDir::Name);
    QFileInfoList dirInfoList = directory.entryInfoList(filtersInfo, QDir::Files | QDir::NoSymLinks, QDir::Name);
    for (int i = 0; i < dirList.size(); ++i)
    {
        if (filePrefix.isEmpty() || dirList.at(i).fileName().startsWith(filePrefix.toUpper()) || dirList.at(i).fileName().startsWith(filePrefix.toLower()))
        {
            //We have found a video file, we try to get the info file with the title name if exists
            //if not, we use the base name as the name
            QString name = "";
            for (int j = 0; j < dirInfoList.size(); ++j)
            {
                if (dirInfoList.at(j).fileName().startsWith(dirList.at(i).completeBaseName()))
                {
                    qDebug() << dirInfoList.at(j).fileName();
                    //We found the info file!
                    name = dirInfoList.at(j).completeBaseName();
                    //QStringList splitName = name.split('-');
                    int index = name.indexOf('-');
                    //The name is in the second position
                    if (index > -1)
                    {
                        name = name.right(name.length() - index - 1);
                    }
                }
            }
            //We have not found the info file, the name is the base name
            if (name.isEmpty())
            {
                name = dirList.at(i).completeBaseName();
            }
            
            //We create the video
            Video v(dirList.at(i).filePath(), name, type);
            list.append(v);
        }
    }
}


QString VideosManager::getPSPSPDir()
{
    if (m_options.isPSPDirSet())
    {
        QDir dir(m_options.getPSPDir());
        QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

        for (int i = 0; i < dirList.size(); ++i) 
        {
            if (dirList.at(i).fileName().toLower() == "mp_root")
            {
                QDir pspdir(dirList.at(i).filePath());
                QFileInfoList pspDirList = pspdir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
                for (int j = 0; j < pspDirList.size(); j++)
                {
                    if (pspDirList.at(j).fileName().toLower() == "100mnv01")
                    {
                        //Bingo!
                        return pspDirList.at(j).filePath();
                    }
                }
            }
        }
        return "";
    }
    else
    {
        return "";
    }
}


QString VideosManager::getPSPAVCDir()
{
    if (m_options.isPSPDirSet())
    {
        QDir dir(m_options.getPSPDir());
        QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

        for (int i = 0; i < dirList.size(); ++i) 
        {
            if (dirList.at(i).fileName().toLower() == "mp_root")
            {
                QDir pspdir(dirList.at(i).filePath());
                QFileInfoList pspDirList = pspdir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
                for (int j = 0; j < pspDirList.size(); j++)
                {
                    if (pspDirList.at(j).fileName().toLower() == "101anv01")
                    {
                        //Bingo!
                        return pspDirList.at(j).filePath();
                    }
                }
            }
        }
        return "";
    }
    else
    {
        return "";
    }
}


void VideosManager::makeConnections()
{
    connect(&m_options, SIGNAL(PSPDirChanged()), this, SLOT(scanPSPSlot()));
    connect(&m_options, SIGNAL(backupsDirChanged()), this, SLOT(scanComputerSlot()));
    connect(m_jobManager, SIGNAL(finished()), this, SLOT(jobsFinished()));
		connect(m_jobManager, SIGNAL(labelChanged(QString)), this, SLOT(slotProgressLabel(QString)));
		connect(m_jobManager, SIGNAL(minimumChanged(int)), this, SLOT(slotProgressMinimum(int)));
		connect(m_jobManager, SIGNAL(maximumChanged(int)), this, SLOT(slotProgressMaximum(int)));
		connect(m_jobManager, SIGNAL(valueChanged(int)), this, SLOT(slotProgressValue(int)));
    connect(m_application, SIGNAL(PSPDirSizeAvailable(quint64)), this, SLOT(updatePSPSize(quint64)));
}


void VideosManager::initComputerDirs()
{
    if (m_options.isBackupDirSet())
    {
        QDir appDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        bool spFound = false;
        bool avcFound = false;
        for (int i = 0; i < dirList.size(); ++i) 
        {
        		qDebug() << dirList.at(i).fileName();
            if (dirList.at(i).fileName().toLower() == "videos_sp")
            {
                spFound = true;
            }
            if (dirList.at(i).fileName().toLower() == "videos_avc")
            {
                avcFound = true;
            }
        }
        if (!spFound)
        {
            if (!appDir.mkdir("videos_sp"))
            {
                //TODO: exception
            }
        }
        if (!avcFound)
        {
            if (!appDir.mkdir("videos_avc"))
            {
                //TODO: exception
            }
        }
    }
}


void VideosManager::initPSPDirs()
{
    if (m_options.isPSPDirSet())
    {
        QDir appDir(m_options.getPSPDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
        		qDebug() << dirList.at(i).fileName();
            if (dirList.at(i).fileName().toLower() == "mp_root")
            {
                QDir pspDir(dirList.at(i).filePath());
                QFileInfoList pspList;
                pspList = pspDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
                bool spFound = false;
                bool avcFound = false;
                for (int i = 0; i < pspList.size(); ++i) 
                {
                		qDebug() << pspList.at(i).fileName();
                    if (dirList.at(i).fileName().toLower() == "100mnv01")
                    {
                        spFound = true;
                    }
                    if (dirList.at(i).fileName().toLower() == "101anv01")
                    {
                        avcFound = true;
                    }
                }
                if (!spFound)
                {
                    if (!pspDir.mkdir("100MNV01"))
                    {
                        //TODO: exception
                    }
                }
                if (!avcFound)
                {
                    if (!pspDir.mkdir("101ANV01"))
                    {
                        //TODO: exception
                    }
                }
                return;
            }
        }
        appDir.mkpath("MP_ROOT/100MNV01");
        appDir.mkpath("MP_ROOT/101ANV01");
    }
}


QString VideosManager::getComputerSPDir()
{
    if (m_options.isBackupDirSet())
    {
        QDir appDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
            if (dirList.at(i).fileName().toLower() == "videos_sp")
            {
                return dirList.at(i).filePath();
            }
        }
    }
    
    return "";
}


QString VideosManager::getComputerAVCDir()
{
    if (m_options.isBackupDirSet())
    {
        QDir appDir(m_options.getBackupDir());
        QFileInfoList dirList;
        dirList = appDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < dirList.size(); ++i) 
        {
            if (dirList.at(i).fileName().toLower() == "videos_avc")
            {
                return dirList.at(i).filePath();
            }
        }
    }
    
    return "";
}


void VideosManager::scanPSPSlot()
{
    scanPSP();
    updatePSPGUI();
}

void VideosManager::scanComputerSlot()
{
    scanComputer();
    updateComputerGUI();
}


bool VideosManager::findVideo(QString title, int type, QList<Video> &list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if ((list.at(i).getName() == title) && (list.at(i).getType() == type))
        {
            return true;
        }
    }
    return false;
}


void VideosManager::startJobs()
{
    if (m_options.isFFmpegSet())
    {
    		if (m_options.isBackupDirSet())
    		{
    			m_jobManager->setFFmpeg(m_options.getFFmpeg());
	        qDebug() << "Start encoding of" << m_jobList.size() << "item(s)";
	        if (m_jobList.size() > 0)
	        {
	            m_view.setJobStatus(false);
	            m_jobManager->start();
                m_encoding = true;
	        }
	      }
	      else
    		{
        	m_view.showErrorMessage(tr("Please, select a valid backup directory in the options"));
    		}
    }
    else
    {
        m_view.showErrorMessage(tr("Please, select a valid FFmpeg binary in the options"));
    }
}

void VideosManager::jobsFinished()
{
    m_view.setJobStatus(true);
    if (m_jobManager->succeed())
    {
        m_view.setInfoLabel(tr("Status: encoding succeed"));
    }
    else
    {
        m_view.setInfoLabel(tr("Status: encoding failed"));
    }
    m_view.setProgressBarValue(0);
    m_encoding = false;
    updateJobGUI();
    scanComputer();
    updateComputerGUI();
}


QString VideosManager::findFreeFile(int type)
{
    QString dir;
    QString prefix;
    if (type == TYPE_SP)
    {
        dir = getPSPSPDir();
        prefix = "M4V";
    }
    else
    {
        dir = getPSPAVCDir();
        prefix = "MAQ";
    }
    QDir pspDir(dir);
    QStringList filters;
    filters << "*.MP4" << "*.mp4";
    QFileInfoList dirList = pspDir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks, QDir::Name);
    
    int number = 1;
    bool fileFound = false;
    QString fileToFind;
    
    while (!fileFound)
    {
        fileToFind = prefix + QString("%1").arg(QString::number(number), 5, '0') + ".MP4";
        qDebug() << "Trying to find" << fileToFind;
        
        bool end = true;
        for (int i = 0; i < dirList.size(); ++i)
        {
            if (dirList.at(i).fileName().toUpper() == fileToFind)
            {
                end = false;
            }
        }
        fileFound = end;
        number++;
    }
    return fileToFind;
}


void VideosManager::delComputer(QList<int> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        QString videoFile = m_videoComputerList.at(list.at(i)).getPath();
        QString thumbnailFile = videoFile.left(videoFile.length() - 4) + ".THM";
        FileManager::deleteFile(videoFile);
        FileManager::deleteFile(thumbnailFile);
    }
    scanComputer();
    updateComputerGUI();
}


void VideosManager::delPSP(QList<int> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        QString videoFile = m_videoPSPList.at(list.at(i)).getPath();
        QString thumbnailFile = videoFile.left(videoFile.length() - 4) + ".THM";
        QString titleFile = videoFile.left(videoFile.length() - 4)  + "-" + m_videoPSPList.at(list.at(i)).getName() + ".INFO";
        FileManager::deleteFile(videoFile);
        FileManager::deleteFile(thumbnailFile);
        FileManager::deleteFile(titleFile);
    }
    scanPSP();
    updatePSPGUI();
}


void VideosManager::moveToPSP(QList<int> list)
{
    if (m_options.isPSPDirSet())
    {
        for (int i = 0; i < list.size(); i++)
        {
            QString outputFileVideo;
            QString outputFileTitle;
            QString outputFileThumbnail;
            if (m_videoComputerList.at(list.at(i)).getType() == TYPE_SP)
            {
                outputFileVideo = getPSPSPDir() + "/" + findFreeFile(TYPE_SP);
            }
            else
            {
                outputFileVideo = getPSPAVCDir() + "/" + findFreeFile(TYPE_AVC);
            }
             
            outputFileTitle = outputFileVideo.left(outputFileVideo.length() - 4) + "-" + m_videoComputerList.at(list.at(i)).getName() + ".INFO";
            outputFileThumbnail = outputFileVideo.left(outputFileVideo.length() - 4) + ".THM";
            
            if (findVideo(m_videoComputerList.at(list.at(i)).getName(), m_videoComputerList.at(list.at(i)).getType(), m_videoPSPList))
            {
                if (m_view.askOverwrite(m_videoComputerList.at(list.at(list.at(i))).getName()))
                {
                    if (!FileManager::copyFile(m_videoComputerList.at(list.at(i)).getPath(), outputFileVideo, true))
                    {
                        m_view.showErrorMessage(tr("Error moving %1").arg(m_videoComputerList.at(list.at(i)).getName()));
                    }
                    else
                    {
                        FileManager::touchFile(outputFileTitle);
                        FileManager::copyFile(m_videoComputerList.at(list.at(i)).getThumbnailPath(), outputFileThumbnail, true);
                    }
                }
            }
            else
            {
                if (!FileManager::copyFile(m_videoComputerList.at(list.at(i)).getPath(), outputFileVideo, true))
                {
                    m_view.showErrorMessage(tr("Error moving %1").arg(m_videoComputerList.at(list.at(i)).getName()));
                }
                else
                {
                    FileManager::touchFile(outputFileTitle);
                    FileManager::copyFile(m_videoComputerList.at(list.at(i)).getThumbnailPath(), outputFileThumbnail, true);
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


void VideosManager::moveToComputer(QList<int> list)
{
    if (m_options.isBackupDirSet())
    {
        for (int i = 0; i < list.size(); i++)
        {
            QString outputFileVideo;
            QString outputFileTitle;
            QString outputFileThumbnail;
            if (m_videoPSPList.at(list.at(i)).getType() == TYPE_SP)
            {
                outputFileVideo = getComputerSPDir() + "/" + m_videoPSPList.at(list.at(i)).getName() + ".MP4";
            }
            else
            {
                outputFileVideo = getComputerAVCDir() + "/" + m_videoPSPList.at(list.at(i)).getName() + ".MP4";
            }
             
            outputFileThumbnail = outputFileVideo.left(outputFileVideo.length() - 4) + ".THM";
            
            if (findVideo(m_videoPSPList.at(list.at(i)).getName(), m_videoPSPList.at(list.at(i)).getType(), m_videoComputerList))
            {
                if (m_view.askOverwrite(m_videoPSPList.at(list.at(list.at(i))).getName()))
                {
                    if (!FileManager::copyFile(m_videoPSPList.at(list.at(i)).getPath(), outputFileVideo, true))
                    {
                        m_view.showErrorMessage(tr("Error moving %1").arg(m_videoPSPList.at(list.at(i)).getName()));
                    }
                    else
                    {
                        FileManager::copyFile(m_videoPSPList.at(list.at(i)).getThumbnailPath(), outputFileThumbnail, true);
                    }
                }
            }
            else
            {
                if (!FileManager::copyFile(m_videoPSPList.at(list.at(i)).getPath(), outputFileVideo, true))
                {
                    m_view.showErrorMessage(tr("Error moving %1").arg(m_videoPSPList.at(list.at(i)).getName()));
                }
                else
                {
                    FileManager::copyFile(m_videoPSPList.at(list.at(i)).getThumbnailPath(), outputFileThumbnail, true);
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


void VideosManager::delJobs(QList<int> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        m_jobList.removeAt(list.at(i));
    }
    updateJobGUI();
}


void VideosManager::stopJobs()
{
    m_jobManager->stop();
}


void VideosManager::updatePSPSize(quint64 size)
{
    m_view.setPSPSize(size);
}


void VideosManager::slotProgressLabel(QString newText)
{
		m_view.setInfoLabel(newText);
}


void VideosManager::slotProgressValue(int newValue)
{
		m_view.setProgressBarValue(newValue);
}


void VideosManager::slotProgressMinimum(int newMinimum)
{
		m_view.setProgressBarMinimum(newMinimum);
}


void VideosManager::slotProgressMaximum(int newMaximum)
{
		m_view.setProgressBarMaximum(newMaximum);
}


bool VideosManager::encoding()
{
    return m_encoding;
}
