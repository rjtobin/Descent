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

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <deque>
using namespace std;

#include "item.hpp"
#include "message.hpp"

class dItem;
class dItemWeapon;
class dItemArmor;

class dInventory
{
private:
  dMessageSystem *mMessageSystem;

  deque<dItem*> mItems;

  dItemWeapon *mWeapon;
  dItemArmor *mArmor;
public:
  dInventory();
  dInventory(dMessageSystem *messageSystem);
  //delete the inventory and all of the items contained within
  ~dInventory();

  bool addItem(dItem *item);
  void destroyItem(dItem *item);
  void dropItem(dItem *item);

  bool hasItem(dItem *item);

  deque<dItem*>* getItemList();

  dItemWeapon* getWeapon();
  dItemArmor* getArmor();

  //void inventoryMenu();
  //void equipMenu();
};

#endif
