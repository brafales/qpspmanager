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


#ifndef QPSPMANAGER_H
#define QPSPMANAGER_H

#include "mainwindow.h"
#include <QObject>

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Main application model
class QPSPManager : public QObject
{
    Q_OBJECT

public:
    QPSPManager(MainWindow &view);
    ~QPSPManager();
    
    QWidget* getMainView();
    
    bool close();

public slots:
    void PSPDirChanged();
    
signals:
    void PSPDirSizeAvailable(quint64 size);

private:
    MainWindow &m_view;
	OptionsManager *m_options;
	ApplicationsManager *m_applications;
    BackupsManager *m_backups;
    VideosManager *m_videos;
    ISOsManager *m_ISOs;
    
    void makeConnections();
};
#endif
