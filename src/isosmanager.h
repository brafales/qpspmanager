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
#ifndef ISOSMANAGER_H
#define ISOSMANAGER_H

#include <QObject>
#include <QList>
#include <QString>

//Forward declarators
class ISOsWidget;
class QPSPManager;
class OptionsManager;

class ISOsManager : public QObject
{
    Q_OBJECT
    
public:
    ISOsManager(QPSPManager *application, ISOsWidget &view, OptionsManager &options);
    void addComputer(QString file, int compression);
    void addPSP(QString file, int compression);
    void delComputer(QList<int> list);
    void delPSP(QList<int> list);
    void moveToPSP(QList<int> list);
    void moveToComputer(QList<int> list);

    ~ISOsManager();

private:
    void scanComputer();
    void scanPSP();
    void updateComputerGUI();
    void updatePSPGUI();
    void scanDir(QString dir, QList<QString> &list);
    void makeConnections();
    void initComputerDir();
    void initPSPDir();
    QString getPSPDir();
    QString getComputerDir();
    
    ISOsWidget &m_view;
    QPSPManager *m_application;
    OptionsManager &m_options;
    
    QList<QString> m_computerList;
    QList<QString> m_PSPList;
    
signals:
    void PSPDirChanged();
    
public slots:
    void scanPSPSlot();
    void scanComputerSlot();
    void updatePSPSize(quint64 size);       
};

#endif
