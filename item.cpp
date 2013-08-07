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

#include "item.hpp"

#include <iostream>
using namespace std;

dItem::dItem()
: mX(-1), mY(-1)
{
}

int& dItem::getX()
{
  return mX;
}

int& dItem::getY()
{
  return mY;
}

int dItemWeapon::getType()
{
  return DITEM_WEAPON;
}

dTileModel dItemWeapon_ShortSword::getModel()
{
  return dTileModel('|', DCOLOR_WHITE, 0, 0);
}

string dItemWeapon_ShortSword::getName()
{
  return "short sword";
}

int dItemWeapon_ShortSword::getValue()
{
  return 1;
}

/* 1d6 */
int dItemWeapon_ShortSword::getDamage()
{
  return random(1,6);
}

int dItemWeapon_ShortSword::getAttackBonus()
{
  return 0;
}

int dItemArmor::getType()
{
  return DITEM_ARMOR;
}

dTileModel dItemArmor_LightLeather::getModel()
{
  return dTileModel('[', DCOLOR_BROWN, 0, 0);
}

string dItemArmor_LightLeather::getName()
{
  return "light leather";
}

int dItemArmor_LightLeather::getValue()
{
  return 1;
}

int dItemArmor_LightLeather::getArmorBonus()
{
  return 2;
}

int dItemArmor_LightLeather::getDamageReduction()
{
  return 0;
}

int dItemAction::getType()
{
  return DITEM_ACTION;
}

//value has no meaning for action items, it's just used for weapon/armor equipping
int dItemAction::getValue()
{
  return 0;
}

dTileModel dItemAction_HealingPotion::getModel()
{
  return dTileModel('!', DCOLOR_WHITE, 0, 0);
}

string dItemAction_HealingPotion::getName()
{
  return "healing potion";
}

int dItemAction_HealingPotion::getActionType()
{
  return DACTIONITEM_HEALINGPOTION;
}

void dItemAction_HealingPotion::action(void *data)
{
  dNPC_Player *target = (dNPC_Player*) data;
  target->getStats().getCurHealth() = target->getStats().getMaxHealth();
}

dTileModel dItemAction_LightningScroll::getModel()
{
  return dTileModel('?', DCOLOR_BLUE, 0, 0);
}

string dItemAction_LightningScroll::getName()
{
  return "scroll of lightning bolt";
}

int dItemAction_LightningScroll::getActionType()
{
  return DACTIONITEM_LIGHTNINGSCROLL;
}

void dItemAction_LightningScroll::action(void *data)
{
  //this seemed so much more elegant in my head:
  struct container {
    dMessageSystem *messageSystem; 
    dMap *map;
    dNPC_Player *player;
    int dir; 
  };
  struct container *real_data = (struct container *) data;
  dMessageSystem *messageSystem = real_data->messageSystem;
  dMap *map = real_data->map;
  dNPC_Player *player = real_data->player;
  int dir = real_data->dir;

  int offsetX, offsetY;
  switch(dir)
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
  int x = player->getX() + offsetX, y = player->getY() + offsetY;

  while(x >= 0 && x < 40 && y >=0 && y < 45 && !map->getVisibleTileModel(x,y).isSolid())
    x += offsetX, y += offsetY;

  if(map->getNPC(x,y))
  {
    map->getNPC(x,y)->getStats().getCurHealth() -= random(1,6) + random(1,6) + 4;
    messageSystem->addMessage("Your lightning bolt hits the " + map->getNPC(x,y)->getName() + ".");
  }
  return;
}

