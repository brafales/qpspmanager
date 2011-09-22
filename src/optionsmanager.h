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
#ifndef OPTIONSMANAGER_H
#define OPTIONSMANAGER_H

#include <QObject>
#include <QString>
#include <QSettings>
#include "firmwares.h"

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class OptionsWidget;

//Options Model. Manages general options needed by other parts
//of the program like the temp directory to use, or the location
//of the PSP dir and FFMpeg binary
class OptionsManager : public QObject
{
    Q_OBJECT
    
public:
    OptionsManager(OptionsWidget &view);

    ~OptionsManager();
	
	static const QString OPTIONS_PSPDIR;
	static const QString OPTIONS_BACKUPDIR;
	static const QString OPTIONS_FFMPEG;
	static const QString OPTIONS_FIRMWARE;
	static const QString OPTIONS_THREADS;
	
	void setFirmware(int firmware);
	void setPSPDir(QString PSPDir);
	void setBackupDir(QString backupDir);
	void setFFmpeg(QString ffmpeg);
    void setThreads(int threads);
	
    QString getPSPDir();
    QString getBackupDir();
    QString getFFmpeg();
    int getFirmware();
    int getThreads();
	
    bool isFirmwareSet();
    bool isPSPDirSet();
    bool isBackupDirSet();
    bool isFFmpegSet();
    
    void saveOptions();
    void retrieveOptions();
    bool isOptionsSaved();
    
    void close();

signals:
    void PSPDirChanged();
    void backupsDirChanged();

private:
    void updateGUI();
    
    OptionsWidget &m_view;

	//Actual options
	QString m_PSPDir;
	QString m_backupDir;
	QString m_ffmpeg;
	int m_firmware;
    int m_threads;
    QSettings m_settings;
    bool m_modified;
};

#endif
