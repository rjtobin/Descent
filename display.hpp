/***************************************************************************
 *   Copyright (C) 2009 by Joshua Tobin                                    *
 *   tobinrj@tcd.ie                                                        *
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

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "simpleconsole.hpp"

#define DCOLOR_WHITE  ((dColor){255,255,255})
#define DCOLOR_BLACK  ((dColor){  0,  0,  0})
#define DCOLOR_RED    ((dColor){188, 30, 71})
#define DCOLOR_GREEN  ((dColor){  0,146, 70})
#define DCOLOR_BLUE   ((dColor){  0,129,205})
#define DCOLOR_YELLOW ((dColor){254,194,  0})

#define DCOLOR_BROWN  ((dColor){204,102,  0})

typedef struct {
  int r,g,b;
} dColor;

#endif
