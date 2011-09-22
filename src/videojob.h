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
#ifndef VIDEOJOB_H
#define VIDEOJOB_H

#include <QString>
#include <QObject>
#include <QRegExp>

//Forward declarators
class QLabel;
class QProgressBar;
class QProcess;

class VideoJob : public QObject
{
    Q_OBJECT

public:
    VideoJob(QString inputFile, 
        QString outputFile, 
        QString title, 
        int passes, 
        int type, 
        int aspectRatio, 
        int threads, 
        int audio, 
        int video);
        
    VideoJob(const VideoJob& v);
    const VideoJob& operator=(const VideoJob& v);
    
    bool run(QString ffmpeg);
    void kill();

    void createThumbnail(QString ffmpeg);

    QString getInputFile() const;
    QString getOutputFile() const;
    QString getTitle() const;
    int getType() const;
    int getPasses() const;
    int getAspect() const;
    int getThreads() const;
    int getAudio() const;
    int getVideo() const;
    
signals:
		void labelChanged(QString label);
		void valueChanged(int value);
		void maximumChanged(int value);
		void minimumChanged(int value);
	
private:
    QString m_inputFile;
    QString m_outputFile;
    QString m_title;
    int m_passes;
    int m_type;
    int m_aspectRatio;
    QProcess *m_ffmpeg;
    bool m_progressSet;
    bool m_durationSet;
    bool m_fpsSet;
    double m_duration;
    double m_fps;
    int m_threads;
    int m_audio;
    int m_video;
    QRegExp m_durationRx;
    QRegExp m_fpsRx;
    QRegExp m_frameStepRx;

private slots:
    void readOutput();

};

#endif
