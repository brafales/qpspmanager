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

#ifndef VIDEOOPTIONS_H
#define VIDEOOPTIONS_H

static const int ASPECT_SQUARE = 0;
static const int ASPECT_WIDE = 1;

static const char* ASPECT_STRINGS[2] = {"4:3", "16:9"};

static const int TYPE_SP = 0;
static const int TYPE_AVC = 1;
static const int TYPE_FULL = 2;

static const char* TYPE_STRINGS[3] = {"SP (FW 1.5)", "AVC (FW 2.0)", "AVC Full Resolution (FW 3.XX OE)"};

#endif
