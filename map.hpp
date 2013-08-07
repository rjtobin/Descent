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

#ifndef MAP_HPP
#define MAP_HPP

#include <deque>

#include "display.hpp"
#include "npc.hpp"
#include "random.hpp"
#include "message.hpp"
#include "item.hpp"

//TODO: make sure the open/close values are consistent everywhere; make sure
//      all code uses these constants instead of the actual numbers.
#define DACTION_PLAYEROPEN 1
#define DACTION_PLAYERCLOSE 2
#define DACTION_PLAYERWALK 3

#define DMAX_DEPTH 100

class dNPC;
class dItem;

class dTileModel
{
private:
  char mLabel;
  dColor mColor;
  bool mSolid;
  bool mLit;

public:
  dTileModel();
  dTileModel(char label, dColor color, bool solid, bool lit);

  char getLabel();
  void setLabel(char label);

  dColor getColor();
  void setColor(dColor color);

  bool isSolid();
  void setSolid(bool solid);

  bool isLit();
  void setLit(bool lit);

  bool operator==(const dTileModel &rhs);
  bool operator!=(const dTileModel &rhs);
};

class dTile
{
public:
  virtual dTileModel getModel() = 0;
  virtual int action(int type, dMessageSystem *messageSystem, dNPC *npc, void *data) = 0;
};

class dTile_Normal : public dTile
{
private:
  dTileModel mModel;
public:
  dTile_Normal();
  dTile_Normal(dTileModel model);

  dTileModel getModel();
  int action(int type, dMessageSystem *messageSystem, dNPC *npc, void *data);
};

class dTile_Door : public dTile
{
private:
  bool mOpen;
public:
  dTile_Door();
  dTile_Door(bool open);

  dTileModel getModel();
  int action(int type, dMessageSystem *messageSystem, dNPC *npc, void *data);
};

class dTile_Trap : public dTile
{
private:
  bool mActivated;
public:
  dTile_Trap();

  dTileModel getModel();
  int action(int type, dMessageSystem *messageSystem, dNPC *npc, void *data);
};


class dMap
{
private:
//hard-code dimensions for now, we can't change them without changing a lot of other things too so this isn't so bad
//map[x][y]
  dTile* mMap[40][45];
  int mVisible[40][45];
  deque<dNPC*> mMonsters;
  deque<dItem*> mItems;
  int mDepth;

  void generateBlock(int block);
  void generateSubBlock(int vertBlock, int horBlock, bool open_top, bool open_bottom);

  bool mOpenTop[3][4], mOpenBottom[3][4];

public:
  dMap();
  ~dMap();

  dNPC* getNPC(int x, int y);
  void addNPC(dNPC *npc);
  deque<dNPC*>* getNPCArray();

  dItem* getItem(int x, int y);
  void addItem(int x, int y, dItem *item);
  void removeItem(int x, int y);
  void destroyItem(int x, int y);

  int& getDepth();

  dTileModel getVisibleTileModel(int x, int y);
  dTileModel getMapTileModel(int x, int y);

  dTile* getMapTile(int x, int y);

  void setMapTile(int x, int y, dTileModel model);
  void setMapTile(int x, int y, dTile *tile);

  void generateMap();
  void shiftUp();

  int isVisible(int x, int y);
  void markVisible(int x, int y, int range);
};

#endif
