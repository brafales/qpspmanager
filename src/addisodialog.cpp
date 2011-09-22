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
 
#include "addisodialog.h"

#include <QFileDialog>
#include <QString>

AddISODialog::AddISODialog(QWidget *parent)
:QDialog(parent)
{
    setupUi(this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


QString AddISODialog::getFileName() const
{
    return lineEditFile->text();
}


void AddISODialog::on_pushButtonFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
         tr("Select ISO file"), 
         QDir::homePath(), 
         "ISO (*.iso *.cso)");
    
    if (!file.isNull())
    {
        lineEditFile->setText(file);
    }
}


int AddISODialog::getCompression() const
{
    return comboBoxCompression->currentText().toInt();
}
