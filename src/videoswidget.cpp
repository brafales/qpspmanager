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
#include "videoswidget.h"
#include "encodevideodialog.h"
#include "videooptions.h"
#include "videosmanager.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>

//TODO: Test
#include <QProgressDialog>

VideosWidget::VideosWidget(QWidget *parent)
:QWidget(parent)
{
    m_uiVideosWidget.setupUi(this);
    setJobStatus(true);
}


VideosWidget::~VideosWidget()
{
}


void VideosWidget::setVideos(VideosManager *videos)
{
    m_videos = videos;
}


void VideosWidget::on_pushButtonNewJob_clicked()
{
    EncodeVideoDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileName = dialog.getFileName();
        QString name = dialog.getName().trimmed();
        int type = dialog.getType();
        int aspect = dialog.getAspectRatio();
        int passes = dialog.getPasses();
        int audio = dialog.getAudio();
        int video = dialog.getVideo();
        m_videos->addVideoJob(fileName, name, passes, type, aspect, audio, video);
    }
}


void VideosWidget::setVideoJobs(QList<VideoJob> &list)
{
    m_uiVideosWidget.tableWidgetJobQueue->clear();
    
    for (int i = 0; i < list.size(); i++)
    {
        QString itemString;
        for (int j = 0; j < 5; j++)
        {
            switch (j)
            {
                case 0:
                    //itemString = QFileInfo(list.at(i).getInputFile()).fileName();
                    //m_uiVideosWidget.tableWidgetJobQueue->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                case 1:
                    itemString += list.at(i).getTitle();
                    //m_uiVideosWidget.tableWidgetJobQueue->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                case 2:
                    itemString += ", " + QString(TYPE_STRINGS[list.at(i).getType()]);
                    //m_uiVideosWidget.tableWidgetJobQueue->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                case 3:
                    itemString += ", " + QString(ASPECT_STRINGS[list.at(i).getAspect()]);
                    //m_uiVideosWidget.tableWidgetJobQueue->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                case 4:
                    itemString += ", " + QString::number(list.at(i).getPasses());
                    //m_uiVideosWidget.tableWidgetJobQueue->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                default:
                    break;
            }
        }
        m_uiVideosWidget.tableWidgetJobQueue->addItem(new QListWidgetItem(itemString));
    }
}

void VideosWidget::setVideosComputer(QList<Video> &list)
{
    m_uiVideosWidget.tableWidgetComputer->clear();
    
    for (int i = 0; i < list.size(); i++)
    {
        QString itemString;
        for (int j = 0; j < 3; j++)
        {
            switch (j)
            {
                case 0:
                    itemString = list.at(i).getName();
                    //m_uiVideosWidget.tableWidgetComputer->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                case 1:
                    if (list.at(i).getType() == TYPE_SP)
                    {
                        itemString += ", SP";
                    }
                    else
                    {
                        itemString += ", AVC";
                    }
                    //m_uiVideosWidget.tableWidgetComputer->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                case 2:
                    itemString += ", " + QString::number(list.at(i).getSize() / (1024 * 1024)) + "MB";
                    break;
                default:
                    break;
            }
        }
        m_uiVideosWidget.tableWidgetComputer->addItem(new QListWidgetItem(itemString));
    }
}


void VideosWidget::setVideosPSP(QList<Video> &list)
{
    m_uiVideosWidget.tableWidgetPSP->clear();
    
    for (int i = 0; i < list.size(); i++)
    {
        QString itemString;
        for (int j = 0; j < 3; j++)
        {
            switch (j)
            {
                case 0:
                    itemString = list.at(i).getName();
                    //m_uiVideosWidget.tableWidgetPSP->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                case 1:
                    if (list.at(i).getType() == TYPE_SP)
                    {
                        itemString += ", SP";
                    }
                    else
                    {
                        itemString += ", AVC";
                    }
                    
                    //m_uiVideosWidget.tableWidgetPSP->setItem(i, j, new QTableWidgetItem(itemString));
                    break;
                case 2:
                    itemString += ", " + QString::number(list.at(i).getSize() / (1024 * 1024)) + "MB";
                    break;
                default:
                    break;
            }
        }
        m_uiVideosWidget.tableWidgetPSP->addItem(new QListWidgetItem(itemString));
    }
}


void VideosWidget::setJobStatus(bool enable)
{
    m_uiVideosWidget.pushButtonNewJob->setEnabled(enable);
    m_uiVideosWidget.pushButtonDeleteJob->setEnabled(enable);
    m_uiVideosWidget.pushButtonStartJob->setEnabled(enable);
    m_uiVideosWidget.pushButtonCopyToComputer->setEnabled(enable);
    m_uiVideosWidget.pushButtonCopyToPSP->setEnabled(enable);
    m_uiVideosWidget.pushButtonRemoveComputer->setEnabled(enable);
    m_uiVideosWidget.pushButtonRemovePSP->setEnabled(enable);
    m_uiVideosWidget.pushButtonCancelJob->setEnabled(!enable);
}


bool VideosWidget::askOverwrite(QString text)
{
    switch(QMessageBox::question(this, 
        tr("Overwrite"), 
        //"Video " + text + " already exists, do you wish to overwrite it?",
        tr("Video %1 already exists, do you wish to overwrite it?").arg(text),
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


bool VideosWidget::askDelete()
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


void VideosWidget::on_pushButtonDeleteJob_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiVideosWidget.tableWidgetJobQueue->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiVideosWidget.tableWidgetJobQueue->row(selectedItems.at(i)));
            }
            m_videos->delJobs(indexes);
        }
    }
}


void VideosWidget::on_pushButtonStartJob_clicked()
{
    m_videos->startJobs();
}


void VideosWidget::on_pushButtonCancelJob_clicked()
{
    m_videos->stopJobs();
}


void VideosWidget::on_pushButtonRemovePSP_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiVideosWidget.tableWidgetPSP->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiVideosWidget.tableWidgetPSP->row(selectedItems.at(i)));
            }
            m_videos->delPSP(indexes);
        }
    }
}


void VideosWidget::on_pushButtonRemoveComputer_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiVideosWidget.tableWidgetComputer->selectedItems();
    if (selectedItems.size() > 0)
    {
        if (askDelete())
        {
            QList<int> indexes;
            //The indexes on the widgetlist is the same as in the model list
            for (int i = 0; i < selectedItems.size(); i++)
            {
                indexes.append(m_uiVideosWidget.tableWidgetComputer->row(selectedItems.at(i)));
            }
            m_videos->delComputer(indexes);
        }
    }
}



void VideosWidget::on_pushButtonCopyToPSP_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiVideosWidget.tableWidgetComputer->selectedItems();
    if (selectedItems.size() > 0)
    {
        QList<int> indexes;
        //The indexes on the widgetlist is the same as in the model list
        for (int i = 0; i < selectedItems.size(); i++)
        {
            indexes.append(m_uiVideosWidget.tableWidgetComputer->row(selectedItems.at(i)));
        }
        m_videos->moveToPSP(indexes);
    }
}


void VideosWidget::on_pushButtonCopyToComputer_clicked()
{
    QList<QListWidgetItem *> selectedItems = m_uiVideosWidget.tableWidgetPSP->selectedItems();
    if (selectedItems.size() > 0)
    {
        QList<int> indexes;
        //The indexes on the widgetlist is the same as in the model list
        for (int i = 0; i < selectedItems.size(); i++)
        {
            indexes.append(m_uiVideosWidget.tableWidgetPSP->row(selectedItems.at(i)));
        }
        m_videos->moveToComputer(indexes);
    }
}


void VideosWidget::showErrorMessage(QString message)
{
    QMessageBox::critical(this, "Error", message);
}


void VideosWidget::setPSPSize(quint64 size)
{
    m_uiVideosWidget.labelPSP->setText(tr("PSP (%1MB free)").arg(size));
}


void VideosWidget::setInfoLabel(QString label)
{
	m_uiVideosWidget.labelJobStatus->setText(label);
}


void VideosWidget::setProgressBarValue(int value)
{
	m_uiVideosWidget.progressBarStatus->setValue(value);
}


void VideosWidget::setProgressBarMinimum(int value)
{
	m_uiVideosWidget.progressBarStatus->setMinimum(value);
}


void VideosWidget::setProgressBarMaximum(int value)
{
	m_uiVideosWidget.progressBarStatus->setMaximum(value);
}
