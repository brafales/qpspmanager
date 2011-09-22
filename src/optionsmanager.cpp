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
#include "optionsmanager.h"
#include "optionswidget.h"
#include "which.h"

#include <QDebug>

const QString OptionsManager::OPTIONS_PSPDIR = "PSPDir";
const QString OptionsManager::OPTIONS_BACKUPDIR = "BackupDir";
const QString OptionsManager::OPTIONS_FFMPEG ="FFmpeg";
const QString OptionsManager::OPTIONS_FIRMWARE = "Firmware";
const QString OptionsManager::OPTIONS_THREADS = "Threads";

OptionsManager::OptionsManager(OptionsWidget &view)
:m_view(view), m_modified(false)
{
	m_view.setOptions(this);
    retrieveOptions();
    
    //For this version we use this firmware only.
    //In the future I'll add support for 1.50 fw
    m_firmware = FIRMWARE_3XX;
    
    if(!isFFmpegSet())
    {
        m_ffmpeg = Which::which("ffmpeg");
    }
        
    saveOptions();    
    updateGUI();
}


OptionsManager::~OptionsManager()
{
}


void OptionsManager::setFirmware(int firmware)
{
    if (m_firmware != firmware)
    {
	    m_firmware = firmware;
        m_modified = true;
    }
}


void OptionsManager::setThreads(int threads)
{
    if (m_threads != threads)
    {
        m_threads = threads;
        m_modified = true;
    }
}


void OptionsManager::setPSPDir(QString PSPDir)
{
    if (m_PSPDir != PSPDir)
    {
	    m_PSPDir = PSPDir;
        emit PSPDirChanged();
        m_modified = true;
    }
}


void OptionsManager::setBackupDir(QString backupDir)
{
    if (m_backupDir != backupDir)
    {
	    m_backupDir = backupDir;
        emit backupsDirChanged();
        m_modified = true;
    }
}


void OptionsManager::setFFmpeg(QString ffmpeg)
{
    if (m_ffmpeg != ffmpeg)
    {
	    m_ffmpeg = ffmpeg;
        m_modified = true;
    }
}


QString OptionsManager::getPSPDir()
{
    return m_PSPDir;
}


QString OptionsManager::getBackupDir()
{
    return m_backupDir;
}


QString OptionsManager::getFFmpeg()
{
    return m_ffmpeg;
}


int OptionsManager::getFirmware()
{
    return m_firmware;
}


int OptionsManager::getThreads()
{
    return m_threads;
}


bool OptionsManager::isFirmwareSet()
{
    return true;
}


bool OptionsManager::isPSPDirSet()
{
    return !m_PSPDir.isEmpty();
}


bool OptionsManager::isBackupDirSet()
{
    return !m_backupDir.isEmpty();
}


bool OptionsManager::isFFmpegSet()
{
    return !m_ffmpeg.isEmpty();
}


//Retrieves the options from the underlying system
void OptionsManager::retrieveOptions()
{
    m_PSPDir = m_settings.value(OPTIONS_PSPDIR, "").toString();
    m_backupDir = m_settings.value(OPTIONS_BACKUPDIR, "").toString();
    m_ffmpeg = m_settings.value(OPTIONS_FFMPEG, "").toString();
    m_firmware = m_settings.value(OPTIONS_FIRMWARE, "0").toInt();
    m_threads = m_settings.value(OPTIONS_THREADS, "1").toInt();
}


//Saves the options to the underlying system
void OptionsManager::saveOptions()
{
    m_settings.setValue(OPTIONS_PSPDIR, m_PSPDir);
    m_settings.setValue(OPTIONS_BACKUPDIR, m_backupDir);
    m_settings.setValue(OPTIONS_FFMPEG, m_ffmpeg);
    m_settings.setValue(OPTIONS_FIRMWARE, m_firmware);
    m_settings.setValue(OPTIONS_THREADS, m_threads);
    m_modified = false;
}


void OptionsManager::updateGUI()
{
    m_view.setPSPDir(m_PSPDir);
    m_view.setBackupDir(m_backupDir);
    m_view.setFFmpeg(m_ffmpeg);
    //m_view.setFirmware(m_firmware);
    m_view.setThreads(m_threads);
}


bool OptionsManager::isOptionsSaved()
{
    return (!m_modified);
}


void OptionsManager::close()
{
    if (m_modified)
    {
        if (m_view.askSave())
        {
            saveOptions();
        }
    }
}
