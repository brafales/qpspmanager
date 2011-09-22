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

#include "videojob.h"
#include "videooptions.h"

#include <QLabel>
#include <QProgressBar>
#include <QDir>
#include <QDebug>
#include <QProcess>

VideoJob::VideoJob(QString inputFile, 
    QString outputFile, 
    QString title, 
    int passes, 
    int type, 
    int aspectRatio, 
    int threads, 
    int audio, 
    int video)
:m_inputFile(inputFile),
m_outputFile(outputFile),
m_title(title),
m_passes(passes),
m_type(type),
m_aspectRatio(aspectRatio),
m_progressSet(false),
m_durationSet(false),
m_fpsSet(false),
m_threads(threads),
m_audio(audio),
m_video(video),
m_durationRx("(\\d\\d:\\d\\d:\\d\\d\\.\\d)"),
m_fpsRx("(\\d+\\.\\d+\\sfps\\([a-z]\\))"),
m_frameStepRx("(frame=\\s*\\d+.*)")
{
}


VideoJob::VideoJob(const VideoJob& v)
:QObject(),
m_inputFile(v.getInputFile()),
m_outputFile(v.getOutputFile()),
m_title(v.getTitle()),
m_passes(v.getPasses()),
m_type(v.getType()),
m_aspectRatio(v.getAspect()),
m_progressSet(false),
m_durationSet(false),
m_fpsSet(false),
m_threads(v.getThreads()),
m_audio(v.getAudio()),
m_video(v.getVideo()),
m_durationRx("(\\d\\d:\\d\\d:\\d\\d\\.\\d)"),
m_fpsRx("(\\d+\\.\\d+\\sfps\\([a-z]\\))"),
m_frameStepRx("(frame=\\s*\\d+.*)")
{
}



const VideoJob& VideoJob::operator=(const VideoJob& v)
{
    m_inputFile = v.getInputFile();
    m_outputFile = v.getOutputFile();
    m_title = v.getTitle();
    m_passes = v.getPasses();
    m_type = v.getType();
    m_aspectRatio = v.getAspect();
    m_threads = v.getThreads();
    m_audio = v.getAudio();
    m_video = v.getVideo();
    return *this;
}


bool VideoJob::run(QString ffmpeg)
{
    m_ffmpeg = new QProcess;
    m_ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    m_ffmpeg->setWorkingDirectory(QDir::temp().absolutePath());

    connect(m_ffmpeg, SIGNAL(readyReadStandardError()), this, SLOT(readOutput()));
    connect(m_ffmpeg, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(m_ffmpeg, SIGNAL(readyRead()), this, SLOT(readOutput()));

    QString command = ffmpeg;
    
    QStringList encoder_params;
    
    if(m_type == TYPE_FULL)
    {
        qDebug() << QString("-acodec aac -ab %1k -vcodec h264 -b %2k -ar 48000 -coder 1 -s 480x272 -r 30000/1001 -f psp").arg(QString::number(m_audio), QString::number(m_video));
        //encoder_params << QString("-acodec aac -ab 128k -vcodec h264 -b 512k -ar 48000 -mbd 2 -coder 1 -vlevel 21 -cmp 2 -subcmp 2 -s 480x272 -r 30000/1001 -f psp -flags loop -trellis 2 -partitions parti4x4+parti8x8+partp4x4+partp8x8+partb8x8").split(' ');
        encoder_params << QString("-acodec aac -ab %1k -vcodec h264 -b %2k -ar 48000 -coder 1 -s 480x272 -r 30000/1001 -f psp").arg(QString::number(m_audio), QString::number(m_video)).split(' ');
        //encoder_params << QString("-acodec aac -ab 96 -vcodec h264 -b 512kb -ar 48000 -s 480x272 -r 30000/1001 -f psp -threads 2").split(' ');
    }
    else if (m_type == TYPE_AVC)
    {
        if (m_aspectRatio == ASPECT_SQUARE)
        {
            //encoder_params << QString("-acodec aac -ab 128k -vcodec h264 -b 512k -ar 48000 -mbd 2 -coder 1 -vlevel 21 -cmp 2 -subcmp 2 -s 320x240 -r 30000/1001 -f psp -flags loop -trellis 2 -partitions parti4x4+parti8x8+partp4x4+partp8x8+partb8x8").split(' ');
            encoder_params << QString("-acodec aac -ab %1k -vcodec h264 -b %2k -ar 48000 -coder 1 -s 320x240 -r 30000/1001 -f psp").arg(QString::number(m_audio), QString::number(m_video)).split(' ');
            //encoder_params << QString("-acodec aac -ab 96 -vcodec h264 -b 512kb -ar 48000 -s 320x240 -r 30000/1001 -f psp -threads 2").split(' ');
        }
        else
        {
            //encoder_params << QString("-acodec aac -ab 128k -vcodec h264 -b 512k -ar 48000 -mbd 2 -coder 1 -vlevel 21 -cmp 2 -subcmp 2 -s 368x208 -r 30000/1001 -f psp -flags loop -trellis 2 -partitions parti4x4+parti8x8+partp4x4+partp8x8+partb8x8").split(' ');
            encoder_params << QString("-acodec aac -ab %1k -vcodec h264 -b %2k -ar 48000 -coder 1 -s 368x208 -r 30000/1001 -f psp").arg(QString::number(m_audio), QString::number(m_video)).split(' ');
            //encoder_params << QString("-acodec aac -ab 96 -vcodec h264 -b 512kb -ar 48000 -s 368x208 -r 30000/1001 -f psp -threads 2").split(' ');
        }
    }
    else //SP
    {
        if (m_aspectRatio == ASPECT_SQUARE)
        {
            encoder_params << QString("-vcodec xvid -s 320x240 -r 29.97 -qscale 10 -acodec aac -ac 2 -ar 24000 -ab 64k -f psp").split(' ');
        }
        else
        {
            encoder_params << QString("-vcodec xvid -s 368x208 -r 29.97 -qscale 10 -acodec aac -ac 2 -ar 24000 -ab 64k -f psp").split(' ');
        }
    }
    
    if (m_passes == 1)
    {
        //ffmpeg -y -i "$1" -title "$title" -timestamp "$time_stamp" $encoder_params "$file_mp4" 2>"$file_log" &
        QStringList parameters;
        parameters << "-y";
        parameters << "-i";
        parameters << m_inputFile;
        parameters << "-title";
        parameters << m_title;
        parameters << encoder_params;
        if (m_threads > 1)
        {
            parameters << "-threads";
            parameters << QString::number(m_threads);
        }
        parameters << m_outputFile;
        
        emit labelChanged(QFileInfo(m_inputFile).fileName());
        
        m_ffmpeg->start(command, parameters);
        
        if (!m_ffmpeg->waitForStarted())
        {
            qDebug() << "Error starting process" << m_inputFile << m_outputFile;
            qDebug() << "Error was:" << m_ffmpeg->error();
            delete m_ffmpeg;
            return false;
        }

         qDebug() << "Process ffmpeg started" << m_inputFile << m_outputFile;
        m_ffmpeg->waitForFinished(-1);
        qDebug() << "Process ffmpeg ended with status" << m_ffmpeg->exitCode();
        m_durationSet = false;
        m_fpsSet = false;
        m_progressSet = false;
        int exitCode = m_ffmpeg->exitCode();
        delete m_ffmpeg;
        
        createThumbnail(ffmpeg);

        emit valueChanged(0);

        return (exitCode == 0);
    }
    else
    {
        //ffmpeg -y -i "$1" -title "$title" -timestamp "$time_stamp" $encoder_params "$file_mp4" 2>"$file_log" &
        QStringList parameters1;
        QStringList parameters2;
        parameters1 << "-y";
        parameters1 << "-i";
        parameters1 << m_inputFile;
        parameters1 << "-title";
        parameters1 << m_title;
        parameters1 << encoder_params;
        if (m_threads > 1)
        {
            parameters1 << "-threads";
            parameters1 << QString::number(m_threads);
        }
        parameters1 << "-pass";
        parameters1 << "1";
        parameters1 << m_outputFile;
        parameters2 << "-y";
        parameters2 << "-i";
        parameters2 << m_inputFile;
        parameters2 << "-title";
        parameters2 << m_title;
        parameters2 << encoder_params;
        if (m_threads > 1)
        {
            parameters2 << "-threads";
            parameters2 << QString::number(m_threads);
        }
        parameters2 << "-pass";
        parameters2 << "2";
        parameters2 << m_outputFile;
        
        QString initialText;
        
        emit labelChanged(QFileInfo(m_inputFile).fileName() + " pass 1/2");
        
        m_ffmpeg->start(command, parameters1);
        
        if (!m_ffmpeg->waitForStarted())
        {
            qDebug() << "Error starting process" << m_inputFile << m_outputFile;
            qDebug() << "Error was:" << m_ffmpeg->error();
            delete m_ffmpeg;
            return false;
        }

        emit valueChanged(0);
        
        qDebug() << "Process ffmpeg started" << m_inputFile << m_outputFile;
        m_ffmpeg->waitForFinished(-1);
        qDebug() << "Process ffmpeg ended with status" << m_ffmpeg->exitCode();
        m_durationSet = false;
        m_fpsSet = false;
        m_progressSet = false;
        int exitCode = m_ffmpeg->exitCode();
        if (exitCode != 0)
        {
            delete m_ffmpeg;
            return false;
        }
        delete m_ffmpeg;
        
        emit valueChanged(0);
        
        m_ffmpeg = new QProcess;
        m_ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
        m_ffmpeg->setWorkingDirectory(QDir::temp().absolutePath());

        connect(m_ffmpeg, SIGNAL(readyReadStandardError()), this, SLOT(readOutput()));
        connect(m_ffmpeg, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));

        emit labelChanged(QFileInfo(m_inputFile).fileName() + " pass 2/2");

        m_ffmpeg->start(command, parameters2);       
        
        if (!m_ffmpeg->waitForStarted())
        {
            qDebug() << "Error starting process" << m_inputFile << m_outputFile;
            qDebug() << "Error was:" << m_ffmpeg->error();
            delete m_ffmpeg;
            return false;
        }

        qDebug() << "Process ffmpeg started" << m_inputFile << m_outputFile;
        m_ffmpeg->waitForFinished(-1);
        qDebug() << "Process ffmpeg ended with status" << m_ffmpeg->exitCode();
        
				emit valueChanged(0);
        
        m_durationSet = false;
        m_fpsSet = false;
        m_progressSet = false;
        exitCode = m_ffmpeg->exitCode();
        delete m_ffmpeg;
        
        if (exitCode == 0)
        {
            createThumbnail(ffmpeg);
        }
        
        return (exitCode == 0);
    }
}

void VideoJob::readOutput()
{
		//qDebug() << "Data ready to be read";
    QString lineRead = m_ffmpeg->readAllStandardOutput();
    //qDebug() << lineRead;

    int pos = m_durationRx.indexIn(lineRead);
    if (pos > -1) 
    {
    //if (m_durationRx.exactMatch(lineRead))
    //{
        if (m_durationSet)
        {
            return;
        }
        QStringList duration = m_durationRx.cap(0).split(':');
        int hours = duration[0].toInt();
        int minutes = duration[1].toInt();
        double seconds = duration[2].toDouble();
        m_duration = hours*3600 + minutes*60 + seconds;
        //qDebug() << lineRead;
        qDebug() << hours << minutes << seconds << m_duration;
        if (m_duration != 0)
        {
            m_durationSet = true;
        }
    }
    //if (m_fpsRx.exactMatch(lineRead))
    //{
    pos = m_fpsRx.indexIn(lineRead);
    if (pos > -1)
    {
        //lineRead is like "xx.xx fps(x)"
        QString fps = m_fpsRx.cap(0).left(m_fpsRx.cap(0).length() - QString(" fps(c)").length());

        m_fps = fps.toDouble();
        qDebug() << m_fps;
        if (m_fps != 0)
        {
            qDebug() << "Output fps:" << m_fps;
            if (m_durationSet)
            {
								int total = (int)(m_duration * m_fps);
                qDebug() << "Total frames" << total;
                emit minimumChanged(0);
                emit maximumChanged(total);
                m_progressSet = true;
            }
        }
    }
    //if (m_frameStepRx.exactMatch(lineRead))
    //{
    pos = m_frameStepRx.indexIn(lineRead);
    if (pos > -1)
    {
        if (!m_progressSet)
        {
            return;
        }
        QRegExp frameNumberRx("(\\d+)");
        pos = frameNumberRx.indexIn(m_frameStepRx.cap(0));
        if (pos > -1)
        {
						emit valueChanged(frameNumberRx.cap(0).toInt());
        }
    }
}

void VideoJob::kill()
{
    if (m_ffmpeg)
    {
        m_ffmpeg->kill();
        QFile::remove(m_outputFile);
    }
}


QString VideoJob::getInputFile() const
{
    return m_inputFile;
}


QString VideoJob::getOutputFile() const
{
    return m_outputFile;
}


QString VideoJob::getTitle() const
{
    return m_title;
}


int VideoJob::getType() const
{
    return m_type;
}


int VideoJob::getPasses() const
{
    return m_passes;
}


int VideoJob::getAspect() const
{
    return m_aspectRatio;
}


int VideoJob::getThreads() const
{
    return m_threads;
}


int VideoJob::getAudio() const
{
    return m_audio;
}


int VideoJob::getVideo() const
{
    return m_video;
}

void VideoJob::createThumbnail(QString ffmpeg)
{
    //ffmpeg -y -i "$1" -f image2 -ss 5 -vframes 1 -r 1 -s 160x120 -an "$file_thm" 2>/dev/null;
    
    m_ffmpeg = new QProcess;
    m_ffmpeg->setWorkingDirectory(QDir::temp().absolutePath());
    
    QString command = ffmpeg;
    
    QStringList parameters;
    parameters << "-y";
    parameters << "-i";
    parameters << m_inputFile;
    parameters << "-f";
    parameters << "image2";
    parameters << "-ss";
    parameters << "5";
    parameters << "-vframes";
    parameters << "1";
    parameters << "-r";
    parameters << "1";
    parameters << "-s";
    parameters << "160x120";
    parameters << "-an";
    
    QString thumbnailFile = QFileInfo(m_outputFile).absolutePath() + "/" + QFileInfo(m_outputFile).completeBaseName() + ".THM";
    
    parameters << thumbnailFile;
    
    m_ffmpeg->start(command, parameters);
    
    if (!m_ffmpeg->waitForStarted())
    {
         qDebug() << "Error starting process THUMBNAIL";
         qDebug() << "Error was:" << m_ffmpeg->error();
         m_durationSet = false;
    		 m_fpsSet = false;
    		 m_progressSet = false;
    		 delete m_ffmpeg;
    		 return;
    }

    qDebug() << "Process ffmpeg started";
    m_ffmpeg->waitForFinished(-1);
    qDebug() << "Process ffmpeg ended with status" << m_ffmpeg->exitCode();
    m_durationSet = false;
    m_fpsSet = false;
    m_progressSet = false;
    //int exitCode = m_ffmpeg->exitCode();
    delete m_ffmpeg;
}
