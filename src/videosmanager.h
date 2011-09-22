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
#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

#include "videojob.h"
#include "video.h"
#include "videojobmanager.h"

#include <QObject>
#include <QList>

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class VideosWidget;
class QString;
class QPSPManager;
class OptionsManager;

class VideosManager : public QObject
{
    Q_OBJECT
    
public:
    VideosManager(QPSPManager *application, VideosWidget &view, OptionsManager &options);
    ~VideosManager();
    
    void updateComputerGUI();
    void updatePSPGUI();
    void updateJobGUI();
    void addVideoJob(QString inputFile, QString title, int passes, int type, int aspectRatio, int audio, int video);
    void startJobs();
    void stopJobs();
    void delComputer(QList<int> list);
    void delJobs(QList<int> list);
    void delPSP(QList<int> list);
    void moveToPSP(QList<int> list);
    void moveToComputer(QList<int> list);
    bool encoding();

signals:
    void PSPDirChanged();

private:
    void scanComputer();
    void scanPSP();
    void scanDir(QString dir, QString filePrefix, QList<Video> &list, int type);
    QString getPSPSPDir();
    QString getPSPAVCDir();
    void makeConnections();
    void initComputerDirs();
    void initPSPDirs();
    QString getComputerSPDir();
    QString getComputerAVCDir();
    bool findVideo(QString title, int type, QList<Video> &list);
    QString findFreeFile(int type);
    
	VideosWidget &m_view;
	QPSPManager *m_application;
    OptionsManager &m_options;
    QList<VideoJob> m_jobList;
    QList<Video> m_videoComputerList;
    QList<Video> m_videoPSPList;
    VideoJobManager *m_jobManager;
    bool m_encoding;

public slots:
    void scanPSPSlot();
    void scanComputerSlot();
    void jobsFinished();
    void updatePSPSize(quint64 size);

private slots:
    void slotProgressLabel(QString newText);
    void slotProgressValue(int newValue);
    void slotProgressMinimum(int newMinimum);
    void slotProgressMaximum(int newMaximum);
};

#endif
