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
#ifndef BACKUPSMANAGER_H
#define BACKUPSMANAGER_H

#include <QObject>
#include <QList>
#include "savegame.h"

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class BackupsWidget;
class QPSPManager;
class OptionsManager;

//Backups Model. Manages the applications in both the
//computer and the PSP.
class BackupsManager : public QObject
{
    Q_OBJECT
    
public:
    BackupsManager(QPSPManager *application, BackupsWidget &view, OptionsManager &options);
    void addComputerSavegame(QString file);
    void addPSPSavegame(QString file);
    void delComputerSavegames(QList<int> list);
    void delPSPSavegames(QList<int> list);
    void moveSavegamesToPSP(QList<int> list);
    void moveSavegamesToComputer(QList<int> list);

    ~BackupsManager();

private:
    void scanComputer();
    void scanPSP();
    void updateComputerGUI();
    void updatePSPGUI();
    void scanDir(QString dir, QList<Savegame> &list);
    QString getSavegameDirectory(QString PSPDirectory);
    void makeConnections();
    void initComputerDir();
    void initPSPDir();
    QString getComputerDir();
    
    BackupsWidget &m_view;
    QPSPManager *m_application;
    OptionsManager &m_options;
    
    QList<Savegame> m_saveComputerList;
    QList<Savegame> m_savePSPList;
    
signals:
    void PSPDirChanged();

public slots:
    void scanPSPSlot();
    void scanComputerSlot();
    void updatePSPSize(quint64 size);
};

#endif
