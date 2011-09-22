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
 
#include "encodevideodialog.h"
#include "videooptions.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>

EncodeVideoDialog::EncodeVideoDialog(QWidget *parent)
:QDialog(parent)
{
    setupUi(this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


QString EncodeVideoDialog::getFileName() const
{
    return lineEditFile->text();
}


QString EncodeVideoDialog::getName() const
{
    return lineEditName->text();
}


int EncodeVideoDialog::getType() const
{
    if (radioButtonTypeSP->isChecked())
    {
        return TYPE_SP;
    }
    else if (radioButtonTypeAVC->isChecked())
    {
        return TYPE_AVC;
    }
    else
    {
        return TYPE_FULL;
    }
}


int EncodeVideoDialog::getAspectRatio() const
{
    if (radioButtonAspectSquare->isChecked())
    {
        return ASPECT_SQUARE;
    }
    else
    {
        return ASPECT_WIDE;
    }
}


int EncodeVideoDialog::getPasses() const
{
    if (radioButtonPasses1->isChecked())
    {
        return 1;
    }
    else
    {
        return 2;
    }
}


int EncodeVideoDialog::getAudio() const
{
    if (radioButtonAudio64->isChecked())
    {
        return 64;
    }
    else
    {
        return 128;
    }
}


int EncodeVideoDialog::getVideo() const
{
    if (radioButtonVideo384->isChecked())
    {
        return 384;
    }
    else if (radioButtonVideo512->isChecked())
    {
        return 512;
    }
    else
    {
        return 768;
    }
}


void EncodeVideoDialog::on_pushButtonFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
         tr("Select video to convert"), 
         QDir::homePath(), 
         "Videos (*.avi *.mpg *.mpeg *.mov *.3gp *.mp4 *.mkv *.vob)");
    
    if (!file.isNull())
    {
        lineEditFile->setText(file);
        lineEditName->setText(getTitleFromFile(QFileInfo(file).completeBaseName()));
    }
}


QString EncodeVideoDialog::getTitleFromFile(QString baseName)
{
    return baseName;
}
