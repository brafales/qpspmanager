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
#include "pspinfomanager.h"
#include "pspinfowidget.h"
#include "optionsmanager.h"
#include "filemanager.h"
#include "qpspmanager.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

PSPInfoManager::PSPInfoManager(QPSPManager *application, PSPInfoWidget &view, OptionsManager &options)
:m_view(view), m_application(application), m_options(options)
{
	m_view.setPSPInfo(this);
    makeConnections();
    scanPSP();
    updatePSPGUI();
}


PSPInfoManager::~PSPInfoManager()
{
}


void PSPInfoManager::scanPSP()
{
    
}


void PSPInfoManager::scanPSPSlot()
{
    scanPSP();
    updatePSPGUI();
}


void PSPInfoManager::updatePSPGUI()
{
}

void PSPInfoManager::makeConnections()
{
    connect(&m_options, SIGNAL(PSPDirChanged()), this, SLOT(scanPSPSlot()));
}