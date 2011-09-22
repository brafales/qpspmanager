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


#include <QApplication>
#include "qpspmanager.h" //aplication model
#include "mainwindow.h" //main controller

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qpspmanager);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QPSPManager");
    QCoreApplication::setApplicationName("QPSPManager");
    MainWindow mw;
    QPSPManager application(mw);
    mw.setApplication(&application);
    mw.show();
    return app.exec();
}
