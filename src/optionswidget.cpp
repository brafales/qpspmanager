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
#include "optionswidget.h"
#include "optionsmanager.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

//TODO: for testing only
#include "which.h"


OptionsWidget::OptionsWidget(QWidget *parent)
 : QWidget(parent)
{
    m_uiOptionsWidget.setupUi(this);
    m_uiOptionsWidget.comboBoxFirmware->hide();
    m_uiOptionsWidget.labelFirmware->hide(); 
}


OptionsWidget::~OptionsWidget()
{
}


void OptionsWidget::setOptions(OptionsManager *options)
{
    m_options = options;
}


void OptionsWidget::on_pushButtonPSPDirBrowse_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, 
	    tr("Select PSP directory"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        
	if (!dir.isNull())
	{
		m_options->setPSPDir(dir);
	}
	
	//set the dir in the view
	m_uiOptionsWidget.lineEditPSPDir->setText(dir);
}

/*
void OptionsWidget::on_pushButtonPSPDirDetect_clicked()
{
	
}
*/

void OptionsWidget::on_pushButtonBackupDir_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, 
	    tr("Select local directory to backup files"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        
	if (!dir.isNull())
	{
		m_options->setBackupDir(dir);
	}
	
	//set the dir in the view
	m_uiOptionsWidget.lineEditBackupDir->setText(dir);
}


void OptionsWidget::on_pushButtonFFmpegBrowse_clicked()
{
	QString file = QFileDialog::getOpenFileName(this,
         tr("Select FFmpeg location"), 
         QDir::homePath(), 
         "");
         
	if (!file.isNull())
	{
		m_options->setFFmpeg(file);
	}
	
	//set the dir in the view
	m_uiOptionsWidget.lineEditFFmpeg->setText(file);
}

void OptionsWidget::on_comboBoxFirmware_currentIndexChanged(int index)
{
	//The firmware enum on the manager is the same as the indexes on the combobox
	m_options->setFirmware(index);
}


void OptionsWidget::on_pushButtonOptionsCancel_clicked()
{
    m_options->retrieveOptions();
}


void OptionsWidget::on_pushButtonOptionsSave_clicked()
{
    m_options->saveOptions();
}


void OptionsWidget::setFirmware(int firmware)
{
    if (m_uiOptionsWidget.comboBoxFirmware->currentIndex() != firmware)
    {
        m_uiOptionsWidget.comboBoxFirmware->setCurrentIndex(firmware);
    }
}


void OptionsWidget::setPSPDir(QString PSPDir)
{
    if (m_uiOptionsWidget.lineEditPSPDir->text() != PSPDir)
    {
        m_uiOptionsWidget.lineEditPSPDir->setText(PSPDir);
    }
}


void OptionsWidget::setBackupDir(QString backupDir)
{
    if (m_uiOptionsWidget.lineEditBackupDir->text() != backupDir)
    {
        m_uiOptionsWidget.lineEditBackupDir->setText(backupDir);
    }
}


void OptionsWidget::setFFmpeg(QString FFmpeg)
{
    if(m_uiOptionsWidget.lineEditFFmpeg->text() != FFmpeg)
    {
        m_uiOptionsWidget.lineEditFFmpeg->setText(FFmpeg);
    }
}

void OptionsWidget::showErrorMessage(QString message)
{
    QMessageBox::critical(this, tr("Error"), message);
}


void OptionsWidget::on_spinBoxFFmpeg_valueChanged(int i)
{
    m_options->setThreads(i);
}


void OptionsWidget::setThreads(int threads)
{
    if (m_uiOptionsWidget.spinBoxFFmpeg->value() != threads)
    {
        m_uiOptionsWidget.spinBoxFFmpeg->setValue(threads);        
    }
}


bool OptionsWidget::askSave()
{
    switch(QMessageBox::question(this, 
        tr("Save"), 
        tr("Do you want to save the changes in the options before quitting?"),
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
