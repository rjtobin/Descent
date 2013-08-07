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

#include "map.hpp"

#include <iostream>

dTileModel::dTileModel()
: mLabel(' '), mSolid(true), mColor(DCOLOR_WHITE), mLit(false)
{
}

dTileModel::dTileModel(char label, dColor color, bool solid, bool lit)
: mLabel(label), mColor(color), mSolid(solid), mLit(lit)
{
}

char dTileModel::getLabel()
{
  return mLabel;
}

void dTileModel::setLabel(char label)
{
  mLabel = label;
}

dColor dTileModel::getColor()
{
  return mColor;
}

void dTileModel::setColor(dColor color)
{
  mColor = color;
}

bool dTileModel::isSolid()
{
  return mSolid;
}

void dTileModel::setSolid(bool solid)
{
  mSolid = solid;
}

bool dTileModel::isLit()
{
  return mLit;
}

void dTileModel::setLit(bool lit)
{
  mLit = lit;
}

bool dTileModel::operator==(const dTileModel &rhs)
{
  if(rhs.mLabel == mLabel && rhs.mColor.r == mColor.r && rhs.mColor.g == mColor.g && rhs.mColor.b == mColor.b && rhs.mSolid == mSolid)
    return true;
  return false;
}

bool dTileModel::operator!=(const dTileModel &rhs)
{
  return !(*this == rhs);
}

dTile_Normal::dTile_Normal()
{
}

dTile_Normal::dTile_Normal(dTileModel model)
: mModel(model)
{
}

dTileModel dTile_Normal::getModel()
{
  return mModel;
}

int dTile_Normal::action(int type, dMessageSystem *messageSystem, dNPC *npc, void *data)
{
  return 0;
}

dTile_Door::dTile_Door()
: mOpen(false)
{
}

dTile_Door::dTile_Door(bool open)
: mOpen(open)
{
}

dTileModel dTile_Door::getModel()
{
  if(mOpen)
    return dTileModel('+', DCOLOR_BROWN, 1, 0);
  else
    return dTileModel('.', DCOLOR_BROWN, 0, 0);
}

int dTile_Door::action(int type, dMessageSystem *messageSystem, dNPC *npc, void *data)
{
  if(type == 1 && !mOpen)
  {
    messageSystem->addMessage("You close the door.");
    mOpen = true;
  }
  else if(type == 2 && mOpen)
  {
    messageSystem->addMessage("You open the door.");
    mOpen = false;
  }
}

dTile_Trap::dTile_Trap()
: mActivated(false)
{
}

dTileModel dTile_Trap::getModel()
{
  if(mActivated)
    return dTileModel('^', DCOLOR_GREEN, 0, 0);
  else
    return dTileModel('.', DCOLOR_WHITE, 0, 0);
}

int dTile_Trap::action(int type, dMessageSystem *messageSystem, dNPC *npc, void *data)
{
  if(!mActivated && type == 3)
  {
    mActivated = true;
    messageSystem->addMessage("You fall into the trap!");
    npc->getStats().getCurHealth() -= random(1,3);
  }
}

dMap::dMap()
: mDepth(0)
{
  for(int i=0; i<40; i++)
    for(int j=0; j<45; j++)
    {
      mMap[i][j] = (dTile*) new dTile_Normal();
      mVisible[i][j] = 0;
    }
  for(int i=0; i<3; i++)
    for(int j=0; j<4; j++)
      mOpenTop[i][j] = mOpenBottom[i][j] = false;
}

dMap::~dMap()
{
  for(int i=0; i<40; i++)
    for(int j=0; j<45; j++)
      if(mMap[i][j])
        delete mMap[i][j];
}

//and now things become potentially unreadable... should be commented soon!
void dMap::generateSubBlock(int vertBlock, int horBlock, bool open_top, bool open_bottom)
{
  //we'll need these:
  dTileModel ground('.', DCOLOR_WHITE, 0, 0);
  dTileModel litGround('.', DCOLOR_WHITE, 0, 1);
  dTileModel wall('#', DCOLOR_WHITE, 1, 0);

  //for ease, we'll save the coordinates of this subblock
  int topX = horBlock*10;
  int topY = vertBlock*15;

  //initially fill with walls, we'll carve out the rooms/corridors
  for(int i=topX; i<topX+10; i++)
    for(int j=topY; j<topY+15; j++)
      setMapTile(i, j, wall);

  //decide the number of subblocks... a bit biased towards 2 blocks unfortunately
  int num_subblocks = random(2,2); // :P

  if(num_subblocks == 1)
  {
    // also :P
  }
 
  else
  {
    //first, do the top part
    int num_rooms, size, roomTopX, roomTopY;
    num_rooms = random(0,2);
    switch(num_rooms)
    {
    default:
    case 0:
      if(open_top)
        for(int j=topY; j<topY+7; j++)
          setMapTile(topX+4, j, ground);
      break;
    case 1:
      size = random(3,8);
      //TODO: randomize this placing
      roomTopX = topX + 5 - (size/2);
      roomTopY = topY + 1;
      for(int i=roomTopX; i<roomTopX + size; i++)
        for(int j=roomTopY; j<roomTopY+5; j++)
          setMapTile(i, j, litGround);
      setMapTile(topX+4, roomTopY+5, (dTile*) new dTile_Door(true));
      break;
    case 2:
      //TODO: randomize which room is first
      //carve out entire area, then rebuild the 4 wall segments
      for(int i=topX+1; i<topX+9; i++)
        for(int j=topY+1; j<topY+6; j++)
          setMapTile(i,j,litGround);
      setMapTile(topX+3, topY+1, wall);
      setMapTile(topX+5, topY+1, wall);
      setMapTile(topX+3, topY+2, wall);
      setMapTile(topX+5, topY+2, wall);
      setMapTile(topX+3, topY+3, wall);
      setMapTile(topX+5, topY+3, wall);
      setMapTile(topX+3, topY+5, wall);
      setMapTile(topX+5, topY+5, wall);

      setMapTile(topX+3, topY+4, (dTile*) new dTile_Door(true));
      setMapTile(topX+5, topY+4, (dTile*) new dTile_Door(true));

      //pave the new 'inner' corridor with unlit ground tiles
      for(int j=topY+1; j<topY+7; j++)
        setMapTile(topX+4, j, ground);

      if(!open_top)
      {
        setMapTile(topX+4, topY+1, wall);
        setMapTile(topX+4, topY+2, wall);
        setMapTile(topX+4, topY+3, wall);
      }

      setMapTile(topX+4, topY+6, ground);
      break;
    }

    //then do the bottom part
    num_rooms = random(0,2);
    switch(num_rooms)
    {
    default:
    case 0:
      if(open_bottom)
        for(int j=topY+8; j<topY+15; j++)
          setMapTile(topX+4, j, ground);
      break;
    case 1:
      size = random(3,8);
      //TODO: randomize this placing
      roomTopX = topX + 5 - (size/2);
      roomTopY = topY + 9;
      for(int i=roomTopX; i<roomTopX + size; i++)
        for(int j=roomTopY; j<roomTopY+5; j++)
          setMapTile(i, j, litGround);
      if(open_bottom)
        setMapTile(topX+4, roomTopY+5, (dTile*) new dTile_Door(true));
        //setMapTile(topX+4, roomTopY+5, ground);
      //setMapTile(topX+4, roomTopY-1, ground);
      setMapTile(topX+4, roomTopY-1, (dTile*) new dTile_Door(true));
      break;
    case 2:
      //TODO: randomize which room is first
      //carve out entire area, then rebuild the 4 wall segments
      for(int i=topX+1; i<topX+9; i++)
        for(int j=topY+9; j<topY+14; j++)
          setMapTile(i, j, litGround);

      setMapTile(topX+3, topY+8, wall);
      setMapTile(topX+5, topY+8, wall);
      setMapTile(topX+3, topY+9, wall);
      setMapTile(topX+5, topY+9, wall);
      setMapTile(topX+3, topY+10, wall);
      setMapTile(topX+5, topY+10, wall);
      setMapTile(topX+3, topY+11, wall);
      setMapTile(topX+5, topY+11, wall);
      setMapTile(topX+3, topY+13, wall);
      setMapTile(topX+5, topY+13, wall);
      
      setMapTile(topX+3, topY+12, (dTile*) new dTile_Door(true));
      setMapTile(topX+5, topY+12, (dTile*) new dTile_Door(true));

      //pave the new 'inner' corridor with unlit ground tiles
      for(int j=topY+8; j<topY+14; j++)
        setMapTile(topX+4, topY+13, ground);

      if(open_bottom)
      {
        setMapTile(topX+4, topY+13, ground);
        setMapTile(topX+4, topY+14, ground);
      }
      else
      {
        setMapTile(topX+4, topY+13, wall);
        setMapTile(topX+4, topY+14, wall);
      }
      setMapTile(topX+4, topY+8, ground);
      break;
    }
  }

  if(open_top)
    setMapTile(topX+4, topY, ground);
  if(open_bottom)
    setMapTile(topX+4, topY+7, ground);
}

dNPC* dMap::getNPC(int x, int y)
{
  for(deque<dNPC*>::iterator i=mMonsters.begin(); i!=mMonsters.end(); i++)
    if(*i)
      if((*i)->getX() == x && (*i)->getY() == y)
        return *i;
  return NULL;
}

void dMap::addNPC(dNPC *npc)
{
  mMonsters.push_back(npc);
}

deque<dNPC*>* dMap::getNPCArray()
{
  return &mMonsters;
}

dItem* dMap::getItem(int x, int y)
{
  for(deque<dItem*>::iterator i=mItems.begin(); i != mItems.end(); i++)
    if(*i)
      if((*i)->getX() == x && (*i)->getY() == y)
        return *i;
  return NULL;
}

void dMap::addItem(int x, int y, dItem *item)
{
  item->getX() = x;
  item->getY() = y;
  mItems.push_back(item);
}

void dMap::removeItem(int x, int y)
{
  for(deque<dItem*>::iterator i=mItems.begin(); i != mItems.end(); i++)
    if((*i)->getX() == x && (*i)->getY() == y)
    {
      mItems.erase(i);
      return;
    }
}

void dMap::destroyItem(int x, int y)
{
  for(deque<dItem*>::iterator i=mItems.begin(); i != mItems.end(); i++)
    if((*i)->getX() == x && (*i)->getY() == y)
    {
      delete (*i);
      mItems.erase(i);
      return;
    }
}

int& dMap::getDepth()
{
  return mDepth;
}

dTileModel dMap::getVisibleTileModel(int x, int y)
{
  if(getNPC(x,y))
    return getNPC(x,y)->getModel();
  if(getItem(x,y))
    return getItem(x,y)->getModel();
  if(!mMap[x][y])
    return dTileModel();
  return mMap[x][y]->getModel();
}

dTileModel dMap::getMapTileModel(int x, int y)
{
  if(!mMap[x][y])
    return dTileModel();
  return mMap[x][y]->getModel();
}

dTile* dMap::getMapTile(int x, int y)
{
  return mMap[x][y];
}

void dMap::setMapTile(int x, int y, dTileModel model)
{
  if(mMap[x][y])
    delete mMap[x][y];
  mMap[x][y] = (dTile*) new dTile_Normal(model);
}

void dMap::setMapTile(int x, int y, dTile *tile)
{
  if(mMap[x][y])
    delete mMap[x][y];
  mMap[x][y] = tile;
}

void dMap::generateMap()
{
  for(int i=0; i<4; i++)
    mOpenTop[0][i] = 0;
  
  for(int i=1; i<3; i++)
  {
    for(int j=0; j<4; j++)
      mOpenTop[i][j] = mOpenBottom[i-1][j] = random(0,1);
    if(!mOpenTop[i][0] && !mOpenTop[i][1] && !mOpenTop[i][2] && !mOpenTop[i][3])
      i--;
  }
  
  do
  {
    for(int j=0; j<3; j++)
      mOpenBottom[2][j] = random(0,1);
  } while(!mOpenBottom[2][0] && !mOpenBottom[2][1] && !mOpenBottom[2][2] && !mOpenBottom[2][3]);

  /*for(int i=0; i<4; i++)
  {
    mOpenTop[0][i] = 0;
    mOpenBottom[2][i] = random(0,1);

    for(int j=1; j<3; j++)
      mOpenTop[j][i] = mOpenBottom[j-1][i] = random(0,1);
  }*/
  
  for(int i=0; i<3; i++)
    generateBlock(i);
}

void dMap::generateBlock(int block)
{
  for(int i=0; i<4; i++)
    generateSubBlock(block, i, mOpenTop[block][i], mOpenBottom[block][i]);
  
  //generate middle corridor for whole block
  dTileModel ground('.', DCOLOR_WHITE, 0, 0);
  dTileModel wall('#', DCOLOR_WHITE, 1, 0);
  int startX=100, endX=-1;
  for(int i=1; i<40; i++)
    if(mMap[i][block*15 + 6]->getModel() != wall || mMap[i][block*15 + 8]->getModel() != wall)
    {
      if(i < startX)
        startX = i;
      if(i > endX)
        endX = i;
    }
  for(int i=startX; i<=endX; i++)
    setMapTile(i, block*15 + 7, ground);

  //place monsters
  int x,y,r;
  if(mDepth != DMAX_DEPTH)
  {
    for(int i=0; i<10; i++)
    {
      x = random(1, 39);
      y = random(block*15,block*15 + 15);
      if(getMapTileModel(x,y).getLabel() == '.')
      {
        r = random(1,12);
        if(r < 6)
          addNPC((dNPC*) new dNPC_Kobold(x, y, this));
        else if(r <= 10)
          addNPC((dNPC*) new dNPC_Gnoll(x, y, this));
        else if(r < 12)
          addNPC((dNPC*) new dNPC_Troll(x, y, this));
        else
          addNPC((dNPC*) new dNPC_Ghost(x, y, this));
      }
    }
  }
  else
  {
    do
    {
      x = random(1, 39);
      y = random(block*15,block*15 + 15);
    } while(getVisibleTileModel(x,y).isSolid() ||  getVisibleTileModel(x+1,y).isSolid() || getVisibleTileModel(x-1,y).isSolid() || getVisibleTileModel(x,y-1).isSolid() || getVisibleTileModel(x,y+1).isSolid());
    addNPC((dNPC*) new dNPC_EvilDM(x, y, this));
  }

  //place items (and by items I mean healing potions, the only item a game 
  //could need!)
  for(int i=0; i<10; i++)
  {
    x = random(1, 39);
    y = random(block*15,block*15 + 15);
    if(getMapTileModel(x,y).getLabel() == '.')
    {
      if(random(0,1) == 0)
        addItem(x, y, (dItem*) new dItemAction_HealingPotion());
      else
        addItem(x, y, (dItem*) new dItemAction_LightningScroll());
    }
  }
}

void dMap::shiftUp()
{
  mDepth += 20;
  for(int i=0; i<40; i++)
    for(int j=0; j<30; j++)
    {
      delete mMap[i][j];
      mMap[i][j] = mMap[i][j+15];
      mMap[i][j+15] = NULL;
      mVisible[i][j] = mVisible[i][j+15];
    }

  for(int i=0; i<40; i++)
    for(int j=30; j<45; j++)
      mVisible[i][j] = 0;

  for(deque<dNPC*>::iterator i = mMonsters.begin(); i != mMonsters.end(); i++)
    (*i)->getY() -= 15;

  for(deque<dItem*>::iterator i = mItems.begin(); i != mItems.end(); i++)
  {
    (*i)->getY() -= 15;
    if((*i)->getY() < 0)
    {
      delete (*i);
      *i = NULL;
    }
  }

  while(1)
  {
    deque<dItem*>::iterator i;
    for(i = mItems.begin(); i != mItems.end(); i++)
    {
      if(!(*i))
      {
        mItems.erase(i);
        break;
      }
    }
    if(i == mItems.end())
      break;
  }

  for(int i=0; i<2; i++)
    for(int j=0; j<4; j++)
    {
      mOpenTop[i][j] = mOpenTop[i+1][j];
      mOpenBottom[i][j] = mOpenBottom[i+1][j];
    }

  for(int j=0; j<4; j++)
    mOpenTop[2][j] = mOpenBottom[2][j];

  if(mDepth == DMAX_DEPTH)
    mOpenBottom[2][0] = mOpenBottom[2][1] = mOpenBottom[2][2] = mOpenBottom[2][3] = false;
  else do
  {
    for(int j=0; j<3; j++)
      mOpenBottom[2][j] = random(0,1);
  } while(!mOpenBottom[2][0] && !mOpenBottom[2][1] && !mOpenBottom[2][2] && !mOpenBottom[2][3]);

  generateBlock(2);
}

int dMap::isVisible(int x, int y)
{
  if(x < 0 || x >= 40 || y < 0 || y >= 45)
    return 0;
  return mVisible[x][y];
}

void dMap::markVisible(int x, int y, int range)
{
  if((range == 0) || x < 0 || x >= 40 || y < 0 || y >= 45)
    return;
  mVisible[x][y] = 1;
  if(!mMap[x][y]->getModel().isSolid())
  {
    for(int i=-1; i<=1; i++)
      for(int j=-1; j<=1; j++)
      {
        if(i != 0 || j != 0)
          markVisible(x+i, y+j, range - 1);
	if(mMap[x+i][y+j]->getModel().isLit() && !mVisible[x+i][y+j])
          markVisible(x+i, y+j, 2);
      }
  }
}
