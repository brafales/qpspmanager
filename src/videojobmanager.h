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
#ifndef VIDEOJOBMANAGER_H
#define VIDEOJOBMANAGER_H

#include <QThread>

//Forward declarators
class VideoJob;

class VideoJobManager : public QThread
{
	Q_OBJECT
    
public:
    VideoJobManager(QList<VideoJob> &list, QString ffmpeg);
    
    void stop();
    void setFFmpeg(QString ffmpeg);
    bool succeed();
    

signals:
		void labelChanged(QString text);
		void maximumChanged(int value);
		void minimumChanged(int value);
		void valueChanged(int value);

protected:
    void run();
    
private:
    QList<VideoJob> &m_jobList;
    bool m_stop;
    VideoJob *m_currentJob;
    QString m_ffmpeg;
    QString m_currentLabel;
    bool m_success;
    
private slots:
		void slotLabel(QString label);
		void slotMinimum(int value);
		void slotMaximum(int value);
		void slotValue(int value);
	
};

#endif
