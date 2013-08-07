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

#include "game.hpp"

//#include <iostream>
#include <sstream>
using namespace std;

dGame::dGame()
{
  mConsole = new SimpleConsole(800, 600);
  mMap = new dMap();
  mMessageSystem = new dMessageSystem(50);
  mInventory = new dInventory(mMessageSystem);
  mPlayer = new dNPC_Player(-1,-1,mMap,mInventory);

  mConsole->init();
}

void dGame::drawMap()
{
  dTileModel curTile;

  int topLeftY = (mPlayer->getY() > 15) ? mPlayer->getY() - 15 : 0;
  if(topLeftY + 30 >= 45)
    topLeftY = 45 - 30;

  for(int i=0; i<40; i++)
    for(int j=0; j<30; j++)
    {
      if(mMap->isVisible(i,j+topLeftY))
      {
        curTile = mMap->getVisibleTileModel(i,j+topLeftY);
        mConsole->drawCharacter(i*16, j*16, curTile.getLabel(), curTile.getColor().r, curTile.getColor().g, curTile.getColor().b);
      }
    }
}

void dGame::drawHUD()
{
  string health, str, strMod, speed, speedMod, con, conMod, ac, dc, level, exp, depth;

  stringstream stream;
  stream << mPlayer->getStats().getCurHealth();
  stream << "/" << mPlayer->getStats().getMaxHealth();
  health = stream.str();

  stream.str("");
  stream << "Str: " << mPlayer->getStats().getStrength() << " +" << mPlayer->getStats().getStrengthMod();
  str = stream.str();

  stream.str("");
  stream << "Dex: " << mPlayer->getStats().getSpeed() << " +" << mPlayer->getStats().getSpeedMod();
  speed = stream.str();

  stream.str("");
  stream << "Con: " << mPlayer->getStats().getConstitution() << " +" << mPlayer->getStats().getConstitutionMod();
  con = stream.str();

  stream.str("");
  stream << "AC: " << mPlayer->getArmorClass();
  ac = stream.str();

  stream.str("");
  stream << "DC: " << mPlayer->getSpeedClass();
  dc = stream.str();

  stream.str("");
  stream << "Level: " << mPlayer->getLevel();
  level = stream.str();

  stream.str("");
  stream << "Exp: " << mPlayer->getExp();
  exp = stream.str();

  stream.str("");
  stream << "Depth: " << mMap->getDepth();
  depth = stream.str();

  mConsole->drawString(634, 20, "Health: " + health );
  mConsole->drawString(634, 40, str);
  mConsole->drawString(634, 60, speed);
  mConsole->drawString(634, 80, con);
  mConsole->drawString(634, 110, ac);
  mConsole->drawString(634, 130, dc);
  mConsole->drawString(634, 160, level);
  mConsole->drawString(634, 180, exp);
  mConsole->drawString(634, 215, "Weapon:");
  mConsole->drawString(644, 232, mInventory->getWeapon() ? mInventory->getWeapon()->getName() : "");
  mConsole->drawString(634, 255, "Armor:");
  mConsole->drawString(644, 272, mInventory->getArmor() ? mInventory->getArmor()->getName() : "");
  mConsole->drawString(634, 302, depth);

  string message;
  if(!mMessageSystem->areMessagesWaiting())
  {
    mConsole->drawString(20, 520, mMessageSystem->getMessage());
    return;
  }
  while(mMessageSystem->areMessagesWaiting())
  {
    //draw new message
    message = mMessageSystem->getMessage();
    mConsole->drawString(20, 520, message);
    if(!mMessageSystem->areMessagesWaiting())
      break;
    mConsole->drawString(20, 546, "MORE");
    mConsole->refresh();
    mConsole->handleEvents();
    mConsole->waitForKeyPress();

    //clear message area
    mConsole->reset(20, 520, 780, 40);
  }
}

// right - 0, up - 1, left - 2, down - 3
// you know you've done too much maths when right, up, left, down
// is an intuitive order
int dGame::getDirection()
{
  unsigned short int key;
  while(!mConsole->isKeyPressed(SDLK_ESCAPE))
  {
    mConsole->handleEvents();
    if(!mConsole->areKeysLeft())
      continue;
    key = mConsole->getKey();
    if(key == 'a')
      return 2;
    if(key == 's')
      return 3;
    if(key == 'd')
      return 0;
    if(key == 'w')
      return 1;
  }
  return -1;
}

void dGame::itemMenu()
{

  //holy template arguments batman! :o
  vector< deque<dItem*>::iterator > actionItems;

  for(deque<dItem*>::iterator i = mInventory->getItemList()->begin(); i != mInventory->getItemList()->end(); i++)
    if((*i)->getType() == DITEM_ACTION)
      actionItems.push_back(i);

  //if there are no items, no need to show the menu
  if(actionItems.size() == 0)
    return;

  int counter=0, cur=0, key;
  dItemAction *item;
  while(mConsole->handleEvents() && !mConsole->isKeyPressed(SDLK_ESCAPE))
  {
    counter = 0;
    mConsole->reset(0,0,300,100);
    for(vector< deque<dItem*>::iterator >::iterator i = actionItems.begin(); i != actionItems.end(); i++, counter++)
      mConsole->drawString(20, 10+counter*20, (**i)->getName());
    mConsole->drawString(0, 10+cur*20, "->");
    mConsole->refresh();

    while(mConsole->areKeysLeft())
      mConsole->getKey();
    while(1)
    {
      while(mConsole->handleEvents() && !mConsole->areKeysLeft())
        ;
      key = mConsole->getKey();
      if(key == SDLK_ESCAPE)
        break;
      else if(key == '\r')
      {
        item = (dItemAction*) *(actionItems[cur]);
        if(item->getActionType() == DACTIONITEM_HEALINGPOTION)
          item->action((void*) mPlayer);
        else if(item->getActionType() == DACTIONITEM_LIGHTNINGSCROLL)
        {
          struct container {
            dMessageSystem *messageSystem; 
            dMap *map;
            dNPC_Player *player;
            int dir; 
          };

          struct container data;
          data.messageSystem = mMessageSystem;
          data.map = mMap;
          data.player = mPlayer; 
          data.dir = getDirection();
          item->action((void*) &data);
        }
        else
          return;
        mInventory->destroyItem(item);
        return;
      }
      else if(key == 'd')
      {
        mInventory->destroyItem(*(actionItems[cur]));
        return;
      }
      else if(key == 's')
        cur = (cur == actionItems.size()-1) ? 0 : cur+1;
      else if(key == 'w')
        cur = (cur == 0) ? actionItems.size()-1 : cur-1;
      else
        continue;
      break;
    }
  }

  /* These two loop tidy up keypresses so strange things don't happen when
  we return. The necessity for the *mess* is one of the biggest issues
  in the code atm, and is a huge TODO if I'm want to maintain this code. */
  while(mConsole->handleEvents() && mConsole->isKeyPressed(SDLK_ESCAPE))
    ;
  while(mConsole->areKeysLeft())
    mConsole->getKey();

  return;
}

void dGame::play()
{
  dTileModel ground('.', DCOLOR_WHITE, 0, 0);
  dTileModel wall('#', DCOLOR_WHITE, 1, 0);

  for(int i=0; i<40; i++)
    for(int j=0; j<45; j++)
      mMap->setMapTile(i, j, ground);

  mMap->generateMap();

  //add player
  for(int i=1; mPlayer->getX() == -1 && i<45; i++)
    for(int j=1; mPlayer->getX() == -1 && j<40; j++)
      if(mMap->getMapTileModel(j,i) == ground)
      {
        mPlayer->getX() = j;
        mPlayer->getY() = i;
        mMap->addNPC((dNPC*) mPlayer);
      }

  //add stuff 
  dNPC_Kobold *a = new dNPC_Kobold(mPlayer->getX(), mPlayer->getY()+2, mMap);
  mMap->addNPC((dNPC*) a);
  mMap->setMapTile(mPlayer->getX()+1, mPlayer->getY(), (dTile*) new dTile_Trap());
  dItemWeapon_ShortSword *sword = new dItemWeapon_ShortSword();
  mMap->addItem(mPlayer->getX(), mPlayer->getY()+1, (dItem*) sword);
  dItemArmor_LightLeather *leather = new dItemArmor_LightLeather();
  mMap->addItem(mPlayer->getX()+1, mPlayer->getY()+1, (dItem*) leather);

  //initial visibility
  mMap->markVisible(mPlayer->getX(), mPlayer->getY(), 3);

  int x,y;
  while(!mConsole->isKeyPressed(SDLK_ESCAPE))
  {

    //if player is dead, add message to that effect
    if(mPlayer->getStats().getCurHealth() < 0)
      mMessageSystem->addMessage("You die.");

    //draw:
    mConsole->reset();
    drawMap();
    drawHUD();
    mConsole->refresh();

    // :(
    if(mPlayer->getStats().getCurHealth() < 0)
    {
      mConsole->waitForKeyPress();
      return;
    }

    //get input & apply move
    unsigned short int key;
    x = mPlayer->getX(), y = mPlayer->getY();
    while(!mConsole->isKeyPressed(SDLK_ESCAPE))
    {
      mConsole->handleEvents();

      if(!mConsole->areKeysLeft())
        continue;

      key = mConsole->getKey();

      if(key == 'a')
        x--;
      else if(key == 'd')
        x++;
      else if(key == 'w')
        y--;
      else if(key == 's')
        y++;
      else if(key == 'c')
      {
        int direction = getDirection();
        if(direction == -1)
          break;
        int offsetX, offsetY;
        switch(direction)
        {
        default:
        case 0:
          offsetX = 1, offsetY = 0;
          break;
        case 1:
          offsetX = 0, offsetY = -1;
          break;
        case 2:
          offsetX = -1, offsetY = 0;
          break;
        case 3:
          offsetX = 0, offsetY = 1;
          break;
        }
        mMap->getMapTile(mPlayer->getX()+offsetX, mPlayer->getY()+offsetY)->action(1, mMessageSystem, mPlayer, NULL);
      }
      else if(key == 'i')
        itemMenu();
      else if(key == 'o')
      {
        int direction = getDirection();
        if(direction == -1)
          break;
        int offsetX, offsetY;
        switch(direction)
        {
        default:
        case 0:
          offsetX = 1, offsetY = 0;
          break;
        case 1:
          offsetX = 0, offsetY = -1;
          break;
        case 2:
          offsetX = -1, offsetY = 0;
          break;
        case 3:
          offsetX = 0, offsetY = 1;
          break;
        }
        mMap->getMapTile(mPlayer->getX()+offsetX, mPlayer->getY()+offsetY)->action(2, mMessageSystem, mPlayer, NULL);
      }
      break;
    }

    if(!mMap->getVisibleTileModel(x,y).isSolid())
    {
      mPlayer->getX() = x;
      mPlayer->getY() = y;

      if(mMap->getItem(x,y))
      {
        if(mInventory->addItem(mMap->getItem(x,y)))
          mMap->removeItem(x,y);
      }

      mMap->getMapTile(x, y)->action(3, mMessageSystem, mPlayer, NULL);

      //regenerate if necessary
      if(mPlayer->getY() >= 30)
        mMap->shiftUp();
    }
    else if(mMap->getNPC(x,y))
    {
      mPlayer->attack(mMap->getNPC(x,y), mMessageSystem);
    }

    //line of sight
    mMap->markVisible(mPlayer->getX(), mPlayer->getY(), 3);

    //move opponents
    deque<deque<dNPC*>::iterator> toDelete;
    deque<dNPC*> *npc = mMap->getNPCArray();
    for(deque<dNPC*>::iterator i = npc->begin(); i != npc->end(); i++)
    {
      if((*i != mPlayer && (*i)->getStats().getCurHealth() < 0) || (*i)->getY() < 0)
      {
        if((*i)->getStats().getCurHealth() < 0)
        {
          mMessageSystem->addMessage("The " + (*i)->getName() + " dies.");
          if((*i)->getName() == "Sadistic DM")
          {
            mConsole->reset();
            mConsole->drawString(350, 100, "Fin");
            //drawHUD();
            //drawMap();
            mConsole->refresh();
            while(mConsole->areKeysLeft())
              mConsole->getKey();
            mConsole->wait(1000);
            mConsole->getKey();
            return;
          }
          mPlayer->getExp() += (*i)->getExpGained();
          while(mPlayer->getLevel() <= mPlayer->getExp()/10)
          {
            mPlayer->getLevel()++;
            //TODO: increase stats
            mPlayer->getStats().getStrength()+=2;
            mPlayer->getStats().getSpeed()+=2;
            mPlayer->getStats().getConstitution()+=2;
            mPlayer->getStats().getCurHealth() = mPlayer->getStats().getMaxHealth()+=3;
          }
        }
        toDelete.push_back(i);
      }
      else
      {
        if((*i)->dodgeRoll() > 5)
          (*i)->makeMove(mMessageSystem, mPlayer);
      }
    }

    for(deque<deque<dNPC*>::iterator>::iterator i=toDelete.begin(); i != toDelete.end(); i++)
    {
      delete *(*i);
      mMap->getNPCArray()->erase(*i);
    }

    //pause briefly
    mConsole->wait(80);
  }
}

dGame::~dGame()
{
  delete mConsole;
  delete mMap;
  delete mPlayer;
  delete mMessageSystem;
  delete mInventory;
}
