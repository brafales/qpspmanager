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
#include "isoswidget.h"
#include "addisodialog.h"
#include <QMessageBox>
#include <QDebug>
#include "isosmanager.h"


ISOsWidget::ISOsWidget(QWidget *parent)
 : QWidget(parent)
{
    m_uiISOsWidget.setupUi(this);
}


ISOsWidget::~ISOsWidget()
{
}


void ISOsWidget::setISOs(ISOsManager *ISOs)
{
    m_ISOs = ISOs;
}

void ISOsWidget::setPSP(QList< QPair<QString, qint64> > &list)
{
    m_uiISOsWidget.listWidgetPSP->clear();
    for (int i = 0; i < list.size(); i++)
    {
        m_uiISOsWidget.listWidgetPSP->addItem(new QListWidgetItem(list[i].first + ", " + QString::number(list[i].second / (1024 * 1024)) + "MB"));
    }
}


void ISOsWidget::setComputer(QList< QPair<QString, qint64> > &list)
{
    m_uiISOsWidget.listWidgetComputer->clear();
    for (int i = 0; i < list.size(); i++)
    {
        m_uiISOsWidget.listWidgetComputer->addItem(new QListWidgetItem(list[i].first + ", " + QString::number(list[i].second / (1024 * 1024)) + "MB"));
    }
}


void ISOsWidget::showErrorMessage(QString message)
{
    QMessageBox::critical(this, tr("Error"), message);
}


bool ISOsWidget::askOverwrite(QString name)
{
    switch(QMessageBox::question(this, 
        tr("Overwrite"), 
        tr("ISO %1 already exists, do you wish to overwrite it?").arg(name),
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


void ISOsWidget::on_pushButtonAddComputer_clicked()
{
    AddISODialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileName = dialog.getFileName();
        int compression = dialog.getCompression();
        m_ISOs->addComputer(fileName, compression);
    }
}


void ISOsWidget::on_pushButtonAddPSP_clicked()
{
    AddISODialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileName = dialog.getFileName();
        int compression = dialog.getCompression();
        m_ISOs->addPSP(fileName, compression);
    }
}


void ISOsWidget::on_pushButtonRemoveComputer_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiISOsWidget.listWidgetComputer->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiISOsWidget.listWidgetComputer->row(selectedItems.at(i)));
            }
            m_ISOs->delComputer(indexes);
        }
    }
}


void ISOsWidget::on_pushButtonRemovePSP_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiISOsWidget.listWidgetPSP->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiISOsWidget.listWidgetPSP->row(selectedItems.at(i)));
            }
            m_ISOs->delPSP(indexes);
        }
    }
}


void ISOsWidget::on_pushButtonMoveToPSP_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiISOsWidget.listWidgetComputer->selectedItems();
    if (selectedItems.size() > 0)
    {
        QList<int> indexes;
        //The indexes on the widgetlist is the same as in the model list
        for (int i = 0; i < selectedItems.size(); i++)
        {
            indexes.append(m_uiISOsWidget.listWidgetComputer->row(selectedItems.at(i)));
        }
        m_ISOs->moveToPSP(indexes);
    }
}


void ISOsWidget::on_pushButtonMoveToComputer_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiISOsWidget.listWidgetPSP->selectedItems();
    if (selectedItems.size() > 0)
    {
        QList<int> indexes;
        //The indexes on the widgetlist is the same as in the model list
        for (int i = 0; i < selectedItems.size(); i++)
        {
            indexes.append(m_uiISOsWidget.listWidgetPSP->row(selectedItems.at(i)));
        }
        m_ISOs->moveToComputer(indexes);
    }
}


bool ISOsWidget::askDelete()
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


void ISOsWidget::setPSPSize(quint64 size)
{
    m_uiISOsWidget.labelPSP->setText(tr("PSP (%1MB free)").arg(size));
}
