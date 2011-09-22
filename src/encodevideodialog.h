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
#ifndef ENCODEVIDEODIALOG_H
#define ENCODEVIDEODIALOG_H

#include "ui_encodevideo.h"
#include <QDialog>

class EncodeVideoDialog : public QDialog, private Ui_EncodeVideoDialog
{
    Q_OBJECT
    
public:
    EncodeVideoDialog(QWidget *parent = 0);
    
    QString getFileName() const;
    QString getName() const;
    int getType() const;
    int getAspectRatio() const;
    int getPasses() const;
    int getAudio() const;
    int getVideo() const;

private slots:
    void on_pushButtonFile_clicked();

private:
    QString getTitleFromFile(QString baseName);

};

#endif
