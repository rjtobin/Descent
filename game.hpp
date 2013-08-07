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

#ifndef GAME_HPP
#define GAME_HPP

#include "map.hpp"

#include "display.hpp"
#include "npc.hpp"
#include "inventory.hpp"
#include "message.hpp"

class dGame
{
private:
  SimpleConsole *mConsole;
  dMap *mMap;
  dNPC_Player *mPlayer;
  dInventory *mInventory;
  dMessageSystem * mMessageSystem;

public:
  dGame();

  void drawMap();
  void drawHUD();

  int getDirection();
  void itemMenu();

  void play();

  ~dGame();
};

#endif
