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
#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include "ui_videoswidget.h"
#include "videojob.h"
#include "video.h"


/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class VideosManager;

class VideosWidget : public QWidget
{
Q_OBJECT
public:
    VideosWidget(QWidget *parent = 0);
    ~VideosWidget();
    
    void setVideos(VideosManager *videos);
    void setVideoJobs(QList<VideoJob> &list);
    void setVideosComputer(QList<Video> &list);
    void setVideosPSP(QList<Video> &list);
    void setJobStatus(bool enable);
    bool askOverwrite(QString text);
    void setInfoLabel(QString label);
    void setProgressBarValue(int value);
    void setProgressBarMinimum(int value);
    void setProgressBarMaximum(int value);
    void showErrorMessage(QString message);
    void setPSPSize(quint64 size);

private slots:
    void on_pushButtonNewJob_clicked();
    void on_pushButtonDeleteJob_clicked();
    void on_pushButtonStartJob_clicked();
    void on_pushButtonCancelJob_clicked();
    void on_pushButtonRemovePSP_clicked();
    void on_pushButtonRemoveComputer_clicked();
    void on_pushButtonCopyToPSP_clicked();
    void on_pushButtonCopyToComputer_clicked();
    bool askDelete();

private:
    VideosManager *m_videos;
    Ui::VideosWidget m_uiVideosWidget;

};

#endif
