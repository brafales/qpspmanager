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
#ifndef PSPINFOMANAGER_H
#define PSPINFOMANAGER_H

#include <QObject>

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class PSPInfoWidget;
class QPSPManager;
class OptionsManager;


class PSPInfoManager : public QObject
{
    Q_OBJECT
    
public:
    PSPInfoManager(QPSPManager *application, PSPInfoWidget &view, OptionsManager &options);
    ~PSPInfoManager();

private:
    void scanPSP();
    void updatePSPGUI();
    void makeConnections();
    
    PSPInfoWidget &m_view;
    QPSPManager *m_application;
    OptionsManager &m_options;
    
private slots:
    void scanPSPSlot();
};

#endif
