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

#include "videojobmanager.h"
#include "videojob.h"
#include "filemanager.h"

#include <QLabel>
#include <QProgressBar>
#include <QDebug>

VideoJobManager::VideoJobManager(QList<VideoJob> &list, QString ffmpeg)
:m_jobList(list), m_stop(false), m_ffmpeg(ffmpeg), m_success(true)
{
}


void VideoJobManager::run()
{
    m_stop = false;
    int i = 1;
    int size = m_jobList.size();
    while (m_jobList.size() > 0)
    {
        if (m_stop)
        {
            qDebug() << "run killed";
            emit finished();
            break;
        }
        
        VideoJob v = m_jobList.first();
        m_currentJob = &v;
        
        connect(&v, SIGNAL(labelChanged(QString)), this, SLOT(slotLabel(QString)));
        connect(&v, SIGNAL(minimumChanged(int)), this, SLOT(slotMinimum(int)));
        connect(&v, SIGNAL(maximumChanged(int)), this, SLOT(slotMaximum(int)));
        connect(&v, SIGNAL(valueChanged(int)), this, SLOT(slotValue(int)));
        
        qDebug() << QString("Encoding item %1 of %2: ").arg(i).arg(size);
        emit labelChanged(tr("Encoding item %1 of %2: ").arg(i).arg(size));
        m_currentLabel = tr("Encoding item %1 of %2: ").arg(i).arg(size);
        if (v.run(m_ffmpeg))
        {
            m_jobList.takeFirst();
            i++;
        }
        else
        {
            qDebug() << QString("Error encoding item %1").arg(i);
            FileManager::deleteFile(v.getOutputFile());
            m_success = false;
            emit finished();
            break;
        }
        
    }
    qDebug() << "run ended";
    emit finished();
}


bool VideoJobManager::succeed()
{
    return m_success;
}


void VideoJobManager::stop()
{
    m_stop = true;
    m_currentJob->kill();
}


void VideoJobManager::setFFmpeg(QString ffmpeg)
{
		m_ffmpeg = ffmpeg;
}		


void VideoJobManager::slotLabel(QString label)
{
		emit labelChanged(m_currentLabel + label);
}


void VideoJobManager::slotMinimum(int value)
{
		emit minimumChanged(value);
}


void VideoJobManager::slotMaximum(int value)
{
		emit maximumChanged(value);
}


void VideoJobManager::slotValue(int value)
{
		emit valueChanged(value);
}
