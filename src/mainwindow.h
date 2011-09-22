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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "optionswidget.h"
#include "applicationswidget.h"
#include "backupswidget.h"
#include "videoswidget.h"
#include "isoswidget.h"
#include "helpwidget.h"

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class QPSPManager;

//Main Window controller
class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	//Setters
    void setApplication(QPSPManager *theValue);
	//Getters
	OptionsWidget& getOptionsView();
	BackupsWidget& getBackupsView();
	ApplicationsWidget& getApplicationsView();
	VideosWidget& getVideosView();
	ISOsWidget& getISOsView();
	//PSPInfoWidget& getPSPInfoView();
	HelpWidget& getHelpWidget();
	bool ask(QString message);
	
protected:
    void closeEvent(QCloseEvent *event);

private:
    QPSPManager *m_application;
    Ui::MainWindow m_uiMainWindow;
    OptionsWidget m_options;
	BackupsWidget m_backups;
	ApplicationsWidget m_applications;
    VideosWidget m_videos;
	ISOsWidget m_ISOs;
	//PSPInfoWidget m_PSPInfo;
	HelpWidget m_Help;
	
	void centerListItems();

public slots:
    void on_listMenu_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
};

#endif
