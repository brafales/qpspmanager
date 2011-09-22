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
#include "applicationswidget.h"
#include "pspapplication.h"
#include "applicationsmanager.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

ApplicationsWidget::ApplicationsWidget(QWidget *parent)
 : QWidget(parent)
{
    m_uiApplicationsWidget.setupUi(this);
}


ApplicationsWidget::~ApplicationsWidget()
{
}


void ApplicationsWidget::setApplications(ApplicationsManager *applications)
{
    m_applications = applications;
}

void ApplicationsWidget::setPSPApplications(QList<PSPApplication> &list)
{
    emptyListWidget(*(m_uiApplicationsWidget.listWidgetAppsPSP));
    for (int i = 0; i < list.size(); i++)
    {
        m_uiApplicationsWidget.listWidgetAppsPSP->addItem(new QListWidgetItem(list[i].getIcon(), list[i].getName()));
    }
}


void ApplicationsWidget::setComputerApplications(QList<PSPApplication> &list)
{
    emptyListWidget(*(m_uiApplicationsWidget.listWidgetAppsComputer));
    for (int i = 0; i < list.size(); i++)
    {
        m_uiApplicationsWidget.listWidgetAppsComputer->addItem(new QListWidgetItem(list[i].getIcon(), list[i].getName()));
    }
}

void ApplicationsWidget::on_pushButtonAddAppComputer_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
         tr("Select PBP location"), 
         QDir::homePath(), 
         "");
         
	if (!file.isNull())
	{
		m_applications->addComputerApplication(file);
	}
}


void ApplicationsWidget::on_pushButtonAddAppPSP_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
         tr("Select PBP location"), 
         QDir::homePath(), 
         "");
         
	if (!file.isNull())
	{
		m_applications->addPSPApplication(file);
	}
}


void ApplicationsWidget::showErrorMessage(QString message)
{
    QMessageBox::critical(this, tr("Error"), message);
}


bool ApplicationsWidget::askAppOverwrite(QString appName)
{
    switch(QMessageBox::question(this, 
        tr("Overwrite"), 
        tr("Application %1 already exists, do you wish to overwrite it?").arg(appName),
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

void ApplicationsWidget::emptyListWidget(QListWidget &list)
{
    list.clear();
}


void ApplicationsWidget::on_pushButtonDelAppComputer_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiApplicationsWidget.listWidgetAppsComputer->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiApplicationsWidget.listWidgetAppsComputer->row(selectedItems.at(i)));
            }
            m_applications->delComputerApplications(indexes);
        }
    }
}


void ApplicationsWidget::on_pushButtonDelAppPSP_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiApplicationsWidget.listWidgetAppsPSP->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiApplicationsWidget.listWidgetAppsPSP->row(selectedItems.at(i)));
            }
            m_applications->delPSPApplications(indexes);
        }
    }
}


void ApplicationsWidget::on_pushButtonMoveAppToPSP_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiApplicationsWidget.listWidgetAppsComputer->selectedItems();
    if (selectedItems.size() > 0)
    {
        QList<int> indexes;
        //The indexes on the widgetlist is the same as in the model list
        for (int i = 0; i < selectedItems.size(); i++)
        {
            indexes.append(m_uiApplicationsWidget.listWidgetAppsComputer->row(selectedItems.at(i)));
        }
        m_applications->moveApplicationsToPSP(indexes);
    }
}


void ApplicationsWidget::on_pushButtonMoveAppToComputer_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiApplicationsWidget.listWidgetAppsPSP->selectedItems();
    if (selectedItems.size() > 0)
    {
        QList<int> indexes;
        //The indexes on the widgetlist is the same as in the model list
        for (int i = 0; i < selectedItems.size(); i++)
        {
            indexes.append(m_uiApplicationsWidget.listWidgetAppsPSP->row(selectedItems.at(i)));
        }
        m_applications->moveApplicationsToComputer(indexes);
    }
}


bool ApplicationsWidget::askDelete()
{
    switch(QMessageBox::question(this, 
        tr("Delete"), 
        tr("Are you sure you want to delete the selected items?"),
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


void ApplicationsWidget::setPSPSize(quint64 size)
{
    m_uiApplicationsWidget.labelPSP->setText(tr("PSP (%1MB free)").arg(size));
}
