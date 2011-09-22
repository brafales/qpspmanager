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
#include "backupswidget.h"
#include "backupsmanager.h"
#include "savegame.h"

#include <QMessageBox>
#include <QFileDialog>

BackupsWidget::BackupsWidget(QWidget *parent)
 : QWidget(parent)
{
    m_uiBackupsWidget.setupUi(this);
}


BackupsWidget::~BackupsWidget()
{
}


void BackupsWidget::setBackups(BackupsManager *backups)
{
    m_backups = backups;
}

void BackupsWidget::setPSPBackups(QList<Savegame> &list)
{
    emptyListWidget(*(m_uiBackupsWidget.listWidgetPSP));
    for (int i = 0; i < list.size(); i++)
    {
        m_uiBackupsWidget.listWidgetPSP->addItem(new QListWidgetItem(list[i].getIcon(), list[i].getName()));
    }
}


void BackupsWidget::setComputerBackups(QList<Savegame> &list)
{
    emptyListWidget(*(m_uiBackupsWidget.listWidgetComputer));
    for (int i = 0; i < list.size(); i++)
    {
        m_uiBackupsWidget.listWidgetComputer->addItem(new QListWidgetItem(list[i].getIcon(), list[i].getName()));
    }    
}


void BackupsWidget::showErrorMessage(QString message)
{
    QMessageBox::critical(this, tr("Error"), message);
}


bool BackupsWidget::askSaveOverwrite(QString appName)
{
    switch(QMessageBox::question(this, 
        tr("Overwrite"), 
        tr("Savegame %1 already exists, do you wish to overwrite it?").arg(appName),
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


void BackupsWidget::on_pushButtonAddComputer_clicked()
{
    QString file = QFileDialog::getExistingDirectory(this,
         tr("Select Savegame folder"), 
         QDir::homePath(), 
         QFileDialog::ShowDirsOnly);
         
	if (!file.isNull())
	{
		m_backups->addComputerSavegame(file);
	}
}


void BackupsWidget::on_pushButtonAddPSP_clicked()
{
    QString file = QFileDialog::getExistingDirectory(this,
         tr("Select Savegame folder"), 
         QDir::homePath(), 
         QFileDialog::ShowDirsOnly);
         
	if (!file.isNull())
	{
		m_backups->addPSPSavegame(file);
	}
}


void BackupsWidget::on_pushButtonRemoveComputer_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiBackupsWidget.listWidgetComputer->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiBackupsWidget.listWidgetComputer->row(selectedItems.at(i)));
            }
            m_backups->delComputerSavegames(indexes);
        }
    }
}


void BackupsWidget::on_pushButtonRemovePSP_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiBackupsWidget.listWidgetPSP->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiBackupsWidget.listWidgetPSP->row(selectedItems.at(i)));
            }
            m_backups->delPSPSavegames(indexes);
        }
    }
}


void BackupsWidget::on_pushButtonMoveToPSP_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiBackupsWidget.listWidgetComputer->selectedItems();
    if (selectedItems.size() > 0)
    {
        QList<int> indexes;
        //The indexes on the widgetlist is the same as in the model list
        for (int i = 0; i < selectedItems.size(); i++)
        {
            indexes.append(m_uiBackupsWidget.listWidgetComputer->row(selectedItems.at(i)));
        }
        m_backups->moveSavegamesToPSP(indexes);
    }
}


void BackupsWidget::on_pushButtonMoveToComputer_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiBackupsWidget.listWidgetPSP->selectedItems();
    if (selectedItems.size() > 0)
    {
        QList<int> indexes;
        //The indexes on the widgetlist is the same as in the model list
        for (int i = 0; i < selectedItems.size(); i++)
        {
            indexes.append(m_uiBackupsWidget.listWidgetPSP->row(selectedItems.at(i)));
        }
        m_backups->moveSavegamesToComputer(indexes);
    }
}


void BackupsWidget::emptyListWidget(QListWidget &list)
{
    list.clear();
}


bool BackupsWidget::askDelete()
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


void BackupsWidget::setPSPSize(quint64 size)
{
    m_uiBackupsWidget.labelPSP->setText(tr("PSP (%1MB free)").arg(size));
}
