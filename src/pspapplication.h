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
 
 #ifndef PSPAPPLICATION_H
 #define PSPAPPLICATION_H

#include <QString>
#include <QPixmap>
#include "firmwares.h"
#include "qpspmanagerpbpexception.h"

 /**
 	@author Bernat Ràfales,,, <bernat@rafales-mulet.com>
 */
 
 //Forward declarators
class QDir;
 
 class PSPApplication
 {
 public:
     enum ApplicationType
      {
          Single = 0, //Single Folder and PBP File
          Multiple = 1 //Multiple folders and PBP Files
      };
      
     PSPApplication(QString path);
     ~PSPApplication();
     
     ApplicationType getType() const;
     QString getPath() const;
     QString getName() const;
     QPixmap getIcon() const;
     static QString getAppName(QString filename);
     static QPixmap getAppIcon(QString filename);
     bool copyToDir(QString dir, int firmware);
     
 private:
     bool isPBPFile(QString filePath);
     bool isPBPSignature(QString file);
     bool isSinglePBP(QString file);
     QString getMultiplePath(QString path);
     bool isSCEMultiple(QDir dir);
     bool isPercentMultiple(QDir dir);
     
     QString m_path; //Path to the single PBP or to the % folder multiple PBP
     ApplicationType m_type;
     QString m_name;
     QPixmap m_icon;
 };
 
 #endif
