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
#ifndef APPLICATIONSMANAGER_H
#define APPLICATIONSSMANAGER_H

#include <QObject>
#include <QList>
#include "pspapplication.h"

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class ApplicationsWidget;
class QPSPManager;
class OptionsManager;

//Applications Model. Manages the applications in both the
//computer and the PSP.
class ApplicationsManager : public QObject
{
    Q_OBJECT
    
public:
    ApplicationsManager(QPSPManager *application, ApplicationsWidget &view, OptionsManager &options);
    void addComputerApplication(QString file);
    void addPSPApplication(QString file);
    void delComputerApplications(QList<int> list);
    void delPSPApplications(QList<int> list);
    void moveApplicationsToPSP(QList<int> list);
    void moveApplicationsToComputer(QList<int> list);

    ~ApplicationsManager();

signals:
    void PSPDirChanged();

private:
    void scanComputer();
    void scanPSP();
    void updateComputerGUI();
    void updatePSPGUI();
    void scanDir(QString dir, QList<PSPApplication> &list);
    QString getAppDirectory(QString PSPDirectory, int firmware);
    bool findApp(QList<PSPApplication> &list, QString path);
    void makeConnections();
    void initComputerDir();
    void initPSPDir();
    QString getComputerDir();
    
    ApplicationsWidget &m_view;
    QPSPManager *m_application;
    OptionsManager &m_options;
    
    QList<PSPApplication> m_appComputerList;
    QList<PSPApplication> m_appPSPList;
    
public slots:
    void scanPSPSlot();
    void scanComputerSlot();
    void updatePSPSize(quint64 size);
};

#endif
