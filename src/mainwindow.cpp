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
#include "mainwindow.h"
#include "qpspmanager.h"
#include <QCloseEvent>
#include <QtDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
 : QMainWindow(parent)
{
    m_uiMainWindow.setupUi(this);

    //Set this window frame as the parent for the subviews
    m_options.setParent(m_uiMainWindow.frame);
	m_backups.setParent(m_uiMainWindow.frame);
	m_applications.setParent(m_uiMainWindow.frame);
    m_videos.setParent(m_uiMainWindow.frame);
	m_ISOs.setParent(m_uiMainWindow.frame);
	//m_PSPInfo.setParent(m_uiMainWindow.frame);
	m_Help.setParent(m_uiMainWindow.frame);
	m_uiMainWindow.stackedWidget->addWidget(&m_options);
	m_uiMainWindow.stackedWidget->addWidget(&m_backups);
	m_uiMainWindow.stackedWidget->addWidget(&m_applications);
	m_uiMainWindow.stackedWidget->addWidget(&m_videos);
	m_uiMainWindow.stackedWidget->addWidget(&m_ISOs);
	//m_uiMainWindow.stackedWidget->addWidget(&m_PSPInfo);
	m_uiMainWindow.stackedWidget->addWidget(&m_Help);
	
	//Select the options tab by default (TODO set it with the options parameter)
	m_uiMainWindow.stackedWidget->setCurrentIndex(0);
	m_uiMainWindow.listMenu->setCurrentRow(0);
	
	//Center the items on the menu
	centerListItems();
}


MainWindow::~MainWindow()
{
}


/**
 * 
 * @param theValue 
 */
void MainWindow::setApplication(QPSPManager *theValue)
{
    m_application = theValue;
}

/**
 * 
 * @param current 
 * @param previous 
 */
void MainWindow::on_listMenu_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
	         current = previous;

	     m_uiMainWindow.stackedWidget->setCurrentIndex(m_uiMainWindow.listMenu->row(current));
}


OptionsWidget& MainWindow::getOptionsView()
{
	return m_options;
}


BackupsWidget& MainWindow::getBackupsView()
{
	return m_backups;
}


ApplicationsWidget& MainWindow::getApplicationsView()
{
	return m_applications;
}


VideosWidget& MainWindow::getVideosView()
{
	return m_videos;
}


ISOsWidget& MainWindow::getISOsView()
{
	return m_ISOs;
}

/*
PSPInfoWidget& MainWindow::getPSPInfoView()
{
	return m_PSPInfo;
}
*/

HelpWidget& MainWindow::getHelpWidget()
{
	return m_Help;
}


void MainWindow::centerListItems()
{
	for (int i = 0; i < m_uiMainWindow.listMenu->count(); i++)
	{
		m_uiMainWindow.listMenu->item(i)->setTextAlignment(Qt::AlignHCenter);
	}
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!m_application->close())
    {
        event->ignore();
    }
}



bool MainWindow::ask(QString message)
{
    switch(QMessageBox::question(this, 
        tr("Exit"), 
        message,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No))
    {
        case QMessageBox::Yes:
            return true;
            break;

        case QMessageBox::No:
            return false;
            break;

        default:
            return false;
    }
}

