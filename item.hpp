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

#ifndef ITEM_HPP
#define ITEM_HPP

#define DITEM_MISC 0
#define DITEM_WEAPON 1
#define DITEM_ARMOR 2
#define DITEM_ACTION 3

#define DACTIONITEM_HEALINGPOTION 1
#define DACTIONITEM_LIGHTNINGSCROLL 2

#include "map.hpp"

#include <string>
using namespace std;

#include "random.hpp"

class dTileModel;

class dItem
{
private:
  //unfortunate, but I don't see any easy way around this, so oh well:
  int mX;
  int mY;
public:
  dItem();

  virtual int getValue() = 0;
  virtual int getType() = 0;
  virtual dTileModel getModel() = 0;
  virtual string getName() = 0;

  //see above...
  int& getX();
  int& getY();
};

class dItemWeapon : public dItem
{
private:
public:
  int getType();

  virtual int getDamage() = 0;
  virtual int getAttackBonus() = 0;
};

class dItemWeapon_ShortSword : public dItemWeapon
{
private:
public:
  dTileModel getModel();
  string getName();
  int getValue();

  int getDamage();
  int getAttackBonus();
};

class dItemArmor : public dItem
{
private:
public:
  int getType();
  deque<dItem*>* getItemList();
  virtual int getArmorBonus() = 0;
  virtual int getDamageReduction() = 0;
};

class dItemArmor_LightLeather : public dItemArmor
{
private:
public:
  dTileModel getModel();
  string getName();
  int getValue();

  int getArmorBonus();
  int getDamageReduction();
};

class dItemAction : public dItem
{
private:
public:
  int getType();
  int getValue();

  virtual int getActionType() = 0;

  virtual void action(void *data) = 0;
};

class dItemAction_HealingPotion : public dItemAction
{
  dTileModel getModel();
  string getName();

  int getActionType();
  void action(void *data);
};

class dItemAction_LightningScroll : public dItemAction
{
  dTileModel getModel();
  string getName();

  int getActionType();
  void action(void *data);
};

#endif
