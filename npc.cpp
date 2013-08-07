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

#include "npc.hpp"

dNPC::dNPC()
{
  mMap = NULL;
  mX = mY = -1;
}

dNPC::dNPC(int x, int y, dMap *map)
: mX(x), mY(y), mMap(map)
{
}

int& dNPC::getX()
{
  return mX;
}

int& dNPC::getY()
{
  return mY;
}

dStats& dNPC::getStats()
{
  return mStats;
}

int dNPC::getArmorClass()
{
  return 0;
}

int dNPC::getSpeedClass()
{
  return 0;
}

int dNPC::attackRoll()
{
  return random(1,20) + getStats().getStrengthMod();
}

int dNPC::dodgeRoll()
{
  return random(1,20) + getStats().getSpeedMod();
}

int dNPC::getExpGained()
{
  return 0;
}

bool dNPC::isPlayer()
{
  return false;
}

void dNPC::attack(dNPC *target, dMessageSystem *messageSystem)
{
  string hitMessage, dodgeMessage, missMessage;
  if(target->isPlayer() && !this->isPlayer())
  {
    hitMessage = "The " + getName() + " hits you.";
    dodgeMessage = "You dodge the " + getName() + ".";
    missMessage = "The " + getName() + " misses you.";
  }
  else if(!target->isPlayer() && !this->isPlayer())
  {
    hitMessage = "The " + getName() + " hits the " + target->getName() + ".";
    dodgeMessage = "The " + target->getName() + " dodges the " + getName() + ".";
    missMessage = "The " + getName() + " misses the " + target->getName() + ".";
  }
  else if(!target->isPlayer() && isPlayer())
  {
    hitMessage = "You hit the " + target->getName() + ".";
    dodgeMessage = "The " + target->getName() + " dodges your blow.";
    missMessage = "You miss the " + target->getName() + ".";
  }
  else
  {
    return;
  }

  if(attackRoll() > target->getArmorClass())
  {
    if(getSpeedClass() > target->dodgeRoll())
    {
      messageSystem->addMessage(hitMessage);
      target->getStats().getCurHealth() -= getAttackDamage();
    }
    else
      messageSystem->addMessage(dodgeMessage);
  }
  else
  {
    messageSystem->addMessage(missMessage);
  }
}

dMap* dNPC::getMap()
{
  return mMap;
}

void dNPC::setMap(dMap *map)
{
  mMap = map;
}

dNPC_Kobold::dNPC_Kobold()
{
  getStats() = dStats(7,7,7,7);
}

dNPC_Kobold::dNPC_Kobold(int x, int y, dMap *map)
: dNPC(x,y,map)
{
  getStats() = dStats(7,7,7,7);
}

void dNPC_Kobold::makeMove(dMessageSystem *messageSystem, dNPC_Player *mPlayer)
{
  int move = random(0,4);

  int x = getX(), y = getY();


  if(getMap()->getNPC(x-1, y) && getMap()->getNPC(x-1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x-1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x+1, y) && getMap()->getNPC(x+1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x+1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y-1) && getMap()->getNPC(x, y-1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y-1), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y+1) && getMap()->getNPC(x, y+1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y+1), messageSystem);
    return;
  }
  
  if((abs((x - mPlayer->getX()) + (y - mPlayer->getY())) < 10.0))
  {
    if(mPlayer->getX() < x)
      x--;
    else if(mPlayer->getX() > x)
      x++;
    else if(mPlayer->getY() < y)
      y--;
    else if(mPlayer->getY() > y)
      y++;
  }
  else
  {
    switch(move)
    {
      case 0:
      default:
        break;
      case 1:
        x--;
        break;
      case 2:
        x++;
        break;
      case 3:
        y--;
        break;
      case 4:
        y++;
        break;
    }
  }
  if(!getMap()->getVisibleTileModel(x,y).isSolid())
    getX() = x, getY() = y;
}

dTileModel dNPC_Kobold::getModel()
{
  return dTileModel('k', DCOLOR_GREEN, 1, 0);
}

string dNPC_Kobold::getName()
{
  return "Kobold";
}

int dNPC_Kobold::getArmorClass()
{
  return 6;
}

int dNPC_Kobold::getSpeedClass()
{
  return 12;
}

int dNPC_Kobold::getExpGained()
{
  return 2;
}

int dNPC_Kobold::getAttackDamage()
{
  return 1;
}

dNPC_Gnoll::dNPC_Gnoll()
{
  getStats() = dStats(10,5,7,9);
}

dNPC_Gnoll::dNPC_Gnoll(int x, int y, dMap *map)
: dNPC(x,y,map)
{
  getStats() = dStats(10,5,7,9);
}

void dNPC_Gnoll::makeMove(dMessageSystem *messageSystem, dNPC_Player *mPlayer)
{
  int move = random(0,4);

  int x = getX(), y = getY();


  if(getMap()->getNPC(x-1, y) && getMap()->getNPC(x-1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x-1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x+1, y) && getMap()->getNPC(x+1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x+1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y-1) && getMap()->getNPC(x, y-1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y-1), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y+1) && getMap()->getNPC(x, y+1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y+1), messageSystem);
    return;
  }
  
  if((abs((x - mPlayer->getX()) + (y - mPlayer->getY())) < 10.0))
  {
    if(mPlayer->getX() < x)
      x--;
    else if(mPlayer->getX() > x)
      x++;
    else if(mPlayer->getY() < y)
      y--;
    else if(mPlayer->getY() > y)
      y++;
  }
  else
  {
    switch(move)
    {
      case 0:
      default:
        break;
      case 1:
        x--;
        break;
      case 2:
        x++;
        break;
      case 3:
        y--;
        break;
      case 4:
        y++;
        break;
    }
  }
  if(!getMap()->getVisibleTileModel(x,y).isSolid())
    getX() = x, getY() = y;
}

dTileModel dNPC_Gnoll::getModel()
{
  return dTileModel('g', DCOLOR_BROWN, 1, 0);
}

string dNPC_Gnoll::getName()
{
  return "Gnoll";
}

int dNPC_Gnoll::getArmorClass()
{
  return 6;
}

int dNPC_Gnoll::getSpeedClass()
{
  return 12;
}

int dNPC_Gnoll::getExpGained()
{
  return 4;
}

int dNPC_Gnoll::getAttackDamage()
{
  return 1;
}

dNPC_Ooze::dNPC_Ooze()
{
  getStats() = dStats(20,5,12,25);
}

dNPC_Ooze::dNPC_Ooze(int x, int y, dMap *map)
: dNPC(x,y,map)
{
  getStats() = dStats(20,5,12,25);
}

void dNPC_Ooze::makeMove(dMessageSystem *messageSystem, dNPC_Player *mPlayer)
{
  int move = random(0,4);

  int x = getX(), y = getY();


  if(getMap()->getNPC(x-1, y) && getMap()->getNPC(x-1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x-1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x+1, y) && getMap()->getNPC(x+1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x+1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y-1) && getMap()->getNPC(x, y-1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y-1), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y+1) && getMap()->getNPC(x, y+1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y+1), messageSystem);
    return;
  }
  
  if((abs((x - mPlayer->getX()) + (y - mPlayer->getY())) < 10.0))
  {
    if(mPlayer->getX() < x)
      x--;
    else if(mPlayer->getX() > x)
      x++;
    else if(mPlayer->getY() < y)
      y--;
    else if(mPlayer->getY() > y)
      y++;
  }
  else
  {
    switch(move)
    {
      case 0:
      default:
        break;
      case 1:
        x--;
        break;
      case 2:
        x++;
        break;
      case 3:
        y--;
        break;
      case 4:
        y++;
        break;
    }
  }
  if(!getMap()->getVisibleTileModel(x,y).isSolid())
    getX() = x, getY() = y;
}

dTileModel dNPC_Ooze::getModel()
{
  return dTileModel('O', DCOLOR_WHITE, 1, 0);
}

string dNPC_Ooze::getName()
{
  return "Ooze";
}

int dNPC_Ooze::getArmorClass()
{
  return 15;
}

int dNPC_Ooze::getSpeedClass()
{
  return 5;
}

int dNPC_Ooze::getExpGained()
{
  return 9;
}

int dNPC_Ooze::getAttackDamage()
{
  return 7;
}

dNPC_EvilDM::dNPC_EvilDM()
{
  getStats() = dStats(12,12,12,40);
}

dNPC_EvilDM::dNPC_EvilDM(int x, int y, dMap *map)
: dNPC(x,y,map)
{
  getStats() = dStats(12,12,12,40);
}

void dNPC_EvilDM::makeMove(dMessageSystem *messageSystem, dNPC_Player *mPlayer)
{
  int move = random(0,4);

  int x = getX(), y = getY();


  if(getMap()->getNPC(x-1, y) && getMap()->getNPC(x-1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x-1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x+1, y) && getMap()->getNPC(x+1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x+1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y-1) && getMap()->getNPC(x, y-1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y-1), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y+1) && getMap()->getNPC(x, y+1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y+1), messageSystem);
    return;
  }

  if(random(0,3) == 1)
  {
    if(!getMap()->getVisibleTileModel(x+1,y).isSolid() && !getMap()->getNPC(x+1,y))
    {
      getMap()->addNPC((dNPC*) new dNPC_Ooze(x+1,y,getMap()));
      if(getMap()->isVisible(x,y))
        messageSystem->addMessage("An ooze is spawned!.");
    }
    else if(!getMap()->getVisibleTileModel(x-1,y).isSolid() && !getMap()->getNPC(x-1,y))
    {
      getMap()->addNPC((dNPC*) new dNPC_Ooze(x-1,y,getMap()));
      if(getMap()->isVisible(x,y))
        messageSystem->addMessage("An ooze is spawned!.");
    }
    else if(!getMap()->getVisibleTileModel(x,y+1).isSolid() && !getMap()->getNPC(x,y+1))
    {
      getMap()->addNPC((dNPC*) new dNPC_Ooze(x,y+1,getMap()));
      if(getMap()->isVisible(x,y))
        messageSystem->addMessage("An ooze is spawned!.");
    }
    else if(!getMap()->getVisibleTileModel(x,y-1).isSolid() && !getMap()->getNPC(x,y-1))
    {
      getMap()->addNPC((dNPC*) new dNPC_Ooze(x,y-1,getMap()));
      if(getMap()->isVisible(x,y))
        messageSystem->addMessage("An ooze is spawned!.");
    }
  }

}

dTileModel dNPC_EvilDM::getModel()
{
  return dTileModel('D', DCOLOR_RED, 1, 0);
}

string dNPC_EvilDM::getName()
{
  return "Sadistic DM";
}

int dNPC_EvilDM::getArmorClass()
{
  return 15;
}

int dNPC_EvilDM::getSpeedClass()
{
  return 15;
}

int dNPC_EvilDM::getExpGained()
{
  return 42;
}

int dNPC_EvilDM::getAttackDamage()
{
  return 1;
}

dNPC_Troll::dNPC_Troll()
{
  getStats() = dStats(20,10,12,35);
}

dNPC_Troll::dNPC_Troll(int x, int y, dMap *map)
: dNPC(x,y,map)
{
  getStats() = dStats(20,10,12,35);
}

void dNPC_Troll::makeMove(dMessageSystem *messageSystem, dNPC_Player *mPlayer)
{
  int move = random(0,4);

  int x = getX(), y = getY();


  if(getMap()->getNPC(x-1, y) && getMap()->getNPC(x-1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x-1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x+1, y) && getMap()->getNPC(x+1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x+1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y-1) && getMap()->getNPC(x, y-1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y-1), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y+1) && getMap()->getNPC(x, y+1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y+1), messageSystem);
    return;
  }
  
  if((abs((x - mPlayer->getX()) + (y - mPlayer->getY())) < 10.0))
  {
    if(mPlayer->getX() < x)
      x--;
    else if(mPlayer->getX() > x)
      x++;
    else if(mPlayer->getY() < y)
      y--;
    else if(mPlayer->getY() > y)
      y++;
  }
  else
  {
    switch(move)
    {
      case 0:
      default:
        break;
      case 1:
        x--;
        break;
      case 2:
        x++;
        break;
      case 3:
        y--;
        break;
      case 4:
        y++;
        break;
    }
  }
  if(!getMap()->getVisibleTileModel(x,y).isSolid())
    getX() = x, getY() = y;
}

dTileModel dNPC_Troll::getModel()
{
  return dTileModel('T', DCOLOR_YELLOW, 1, 0);
}

string dNPC_Troll::getName()
{
  return "troll";
}

int dNPC_Troll::getArmorClass()
{
  return 16;
}

int dNPC_Troll::getSpeedClass()
{
  return 10;
}

int dNPC_Troll::getExpGained()
{
  return 10;
}

int dNPC_Troll::getAttackDamage()
{
  return random(1,6);
}

dNPC_Ghost::dNPC_Ghost()
{
  getStats() = dStats(10,15,12,8);
}

dNPC_Ghost::dNPC_Ghost(int x, int y, dMap *map)
: dNPC(x,y,map)
{
  getStats() = dStats(10,15,12,8);
}

void dNPC_Ghost::makeMove(dMessageSystem *messageSystem, dNPC_Player *mPlayer)
{
  int move = random(0,4);

  int x = getX(), y = getY();


  if(getMap()->getNPC(x-1, y) && getMap()->getNPC(x-1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x-1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x+1, y) && getMap()->getNPC(x+1, y)->isPlayer())
  {
    attack(getMap()->getNPC(x+1, y), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y-1) && getMap()->getNPC(x, y-1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y-1), messageSystem);
    return;
  }
  else if(getMap()->getNPC(x, y+1) && getMap()->getNPC(x, y+1)->isPlayer())
  {
    attack(getMap()->getNPC(x, y+1), messageSystem);
    return;
  }
  
  if((abs((x - mPlayer->getX()) + (y - mPlayer->getY())) < 10.0))
  {
    if(mPlayer->getX() < x)
      x--;
    else if(mPlayer->getX() > x)
      x++;
    else if(mPlayer->getY() < y)
      y--;
    else if(mPlayer->getY() > y)
      y++;
  }
  else
  {
    switch(move)
    {
      case 0:
      default:
        break;
      case 1:
        x--;
        break;
      case 2:
        x++;
        break;
      case 3:
        y--;
        break;
      case 4:
        y++;
        break;
    }
  }
  //if(!getMap()->getVisibleTileModel(x,y).isSolid())
    getX() = x, getY() = y;
}

dTileModel dNPC_Ghost::getModel()
{
  return dTileModel('G', DCOLOR_WHITE, 1, 0);
}

string dNPC_Ghost::getName()
{
  return "ghost";
}

int dNPC_Ghost::getArmorClass()
{
  return 12;
}

int dNPC_Ghost::getSpeedClass()
{
  return 12;
}

int dNPC_Ghost::getExpGained()
{
  return 8;
}

int dNPC_Ghost::getAttackDamage()
{
  return 2;
}

dNPC_Player::dNPC_Player()
: mExperience(0), mLevel(1)
{
  getStats() = dStats(10,10,10,10);
}

dNPC_Player::dNPC_Player(int x, int y, dMap *map, dInventory *inventory)
: dNPC(x,y,map), mInventory(inventory), mExperience(0), mLevel(1)
{
  //TODO: do these stats properly (somehow)
  getStats() = dStats(10,10,10,10);
}

bool dNPC_Player::isPlayer()
{
  return true;
}

void dNPC_Player::makeMove(dMessageSystem *messageSystem, dNPC_Player *player)
{
}

dTileModel dNPC_Player::getModel()
{
  return dTileModel('@', DCOLOR_BLUE, 1, 0);
}

string dNPC_Player::getName()
{
  return "Player";
}

int dNPC_Player::getArmorClass()
{
  return getStats().getConstitutionMod() + (mInventory->getArmor() ? mInventory->getArmor()->getArmorBonus() : 0);
}

int dNPC_Player::getSpeedClass()
{
  return 20 + getStats().getSpeedMod();
}

int dNPC_Player::getAttackDamage()
{
  if(mInventory->getWeapon())
    return mInventory->getWeapon()->getDamage() + ((getStats().getStrengthMod() > 0) ? getStats().getStrengthMod() : 0);
  else //player is unarmed, a punch counts for 1
    return 1;
}

int& dNPC_Player::getExp()
{
  return mExperience;
}

int& dNPC_Player::getLevel()
{
  return mLevel;
}
