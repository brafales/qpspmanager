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
#ifndef ISOSWINDOW_H
#define ISOSWINDOW_H

#include <QDialog>
#include "ui_isoswidget.h"

/**
	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
*/

//Forward declarators
class ISOsManager;

//Backups Controller
class ISOsWidget : public QWidget
{
Q_OBJECT
public:
    ISOsWidget(QWidget *parent = 0);
    ~ISOsWidget();

    void setISOs(ISOsManager *ISOs);
        
    void setPSP(QList< QPair<QString, qint64> > &list);
    void setComputer(QList< QPair<QString, qint64> > &list);
    
    void showErrorMessage(QString message);
    bool askOverwrite(QString name);
    void setPSPSize(quint64 size);
    
private slots:
    void on_pushButtonAddComputer_clicked();
    void on_pushButtonAddPSP_clicked();
    void on_pushButtonRemoveComputer_clicked();
    void on_pushButtonRemovePSP_clicked();
    void on_pushButtonMoveToPSP_clicked();
    void on_pushButtonMoveToComputer_clicked();

private:
    bool askDelete();
    
    ISOsManager *m_ISOs;
    Ui::ISOsWidget m_uiISOsWidget;
};

#endif
