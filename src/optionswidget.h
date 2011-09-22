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
#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QWidget>
#include "ui_optionswidget.h"

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class OptionsManager;

//Options Controller
class OptionsWidget : public QWidget
{
    Q_OBJECT
    
public:
    OptionsWidget(QWidget *parent = 0);
    ~OptionsWidget();

    void setOptions(OptionsManager *options);
    void setFirmware(int firmware);
    void setPSPDir(QString PSPDir);
    void setBackupDir(QString backupDir);
    void setFFmpeg(QString FFmpeg);
    void setThreads(int threads);
    void showErrorMessage(QString message);
    bool askSave();

private slots:
	void on_pushButtonPSPDirBrowse_clicked();
	//void on_pushButtonPSPDirDetect_clicked();
	void on_pushButtonBackupDir_clicked();
	void on_pushButtonFFmpegBrowse_clicked();
	void on_comboBoxFirmware_currentIndexChanged(int index);
    void on_pushButtonOptionsCancel_clicked();
    void on_pushButtonOptionsSave_clicked();
    void on_spinBoxFFmpeg_valueChanged(int i);

private:    
    OptionsManager *m_options;
    Ui::OptionsWidget m_uiOptionsWidget;
};

#endif
