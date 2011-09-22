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


#include "qpspmanager.h"
#include "optionsmanager.h"
#include "applicationsmanager.h"
#include "backupsmanager.h"
#include "videosmanager.h"
#include "isosmanager.h"
#include "fsusage.h"

#include <QDebug>

QPSPManager::QPSPManager(MainWindow &view)
:m_view(view)
{
	m_options = new OptionsManager(m_view.getOptionsView());
	m_applications = new ApplicationsManager(this, m_view.getApplicationsView(), *m_options);
	m_backups = new BackupsManager(this, m_view.getBackupsView(), *m_options);
    m_videos = new VideosManager(this, m_view.getVideosView(), *m_options);
    m_ISOs = new ISOsManager(this, m_view.getISOsView(), *m_options);
    
    makeConnections();
    PSPDirChanged();
}

QPSPManager::~QPSPManager()
{
	delete m_options;
    delete m_backups;
    delete m_videos;
    delete m_ISOs;
}

QWidget* QPSPManager::getMainView()
{
    return &m_view;
}


void QPSPManager::PSPDirChanged()
{
    if (m_options->isPSPDirSet())
    {
        quint64 pspSize = FSUsage::getFSFree(m_options->getPSPDir());
        qDebug() << pspSize;
        if (pspSize > 0)
        {
            emit PSPDirSizeAvailable(pspSize);
        }
    }
}


void QPSPManager::makeConnections()
{
    connect(m_applications, SIGNAL(PSPDirChanged()), this, SLOT(PSPDirChanged()));
    connect(m_backups, SIGNAL(PSPDirChanged()), this, SLOT(PSPDirChanged()));
    connect(m_videos, SIGNAL(PSPDirChanged()), this, SLOT(PSPDirChanged()));
    connect(m_ISOs, SIGNAL(PSPDirChanged()), this, SLOT(PSPDirChanged()));
}


//Use this to do some checks before closing and return false if you don't want the main window closed
bool QPSPManager::close()
{
    if (m_videos->encoding())
    {
        if (m_view.ask(tr("QPSPManager is still encoding videos, are you sure you want to exit?")))
        {
            m_videos->stopJobs();
            m_options->close();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        m_options->close();
        return true;
    }
}
