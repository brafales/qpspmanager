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
#include "pspinfowidget.h"
#include <QPainter>

PSPInfoWidget::PSPInfoWidget(QWidget *parent)
 : QWidget(parent), m_percent(-1)
{
    m_uiPSPInfoWidget.setupUi(this);
}


PSPInfoWidget::~PSPInfoWidget()
{
}


void PSPInfoWidget::setPSPInfo(PSPInfoManager *PSPInfo)
{
    m_PSPInfo = PSPInfo;
}

void PSPInfoWidget::updatePSPInfo(int usedPercent)
{
    QPainter painter(m_uiPSPInfoWidget.framePercentage);
    QRectF percentRect = m_uiPSPInfoWidget.framePercentage->rect();
    percentRect.setHeight((qreal)(percentRect.height() / 100 * (qreal)usedPercent));
    painter.setPen(Qt::blue);
    painter.setBackground(Qt::blue);
    //Paint all the percentage as free
    painter.drawRect(m_uiPSPInfoWidget.framePercentage->rect());
    
    //Paint the used percentage in red
    painter.setPen(Qt::red);
    painter.setBackground(Qt::red);
}