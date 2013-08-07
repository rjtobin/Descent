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

#include "inventory.hpp"

dInventory::dInventory()
: mMessageSystem(0), mWeapon(NULL), mArmor(NULL)
{
}

dInventory::dInventory(dMessageSystem *messageSystem)
: mMessageSystem(messageSystem), mWeapon(NULL), mArmor(NULL)
{
}

//delete the inventory and all of the items contained within
dInventory::~dInventory()
{
  delete mWeapon;
  mWeapon = NULL;
  for(deque<dItem*>::iterator i=mItems.begin(); i != mItems.end(); i++)
    if(*i)
    {
      delete (*i);
      *i = NULL;
    }
}

bool dInventory::addItem(dItem *item)
{
  if(item->getType() == DITEM_WEAPON)
  {
    if(!mWeapon)
    {
      mMessageSystem->addMessage("You are now weilding a " + item->getName() + ".");
      mWeapon = (dItemWeapon*) item;
      return true;
    }
    else if(item->getValue() > mWeapon->getValue())
    {
      delete mWeapon;
      mWeapon = (dItemWeapon*) item;
      mMessageSystem->addMessage("You are now weilding a " + item->getName() + ".");
      return true;
    }
    else
      return false;
  }
  else if(item->getType() == DITEM_ARMOR)
  {
    if(!mArmor)
    {
      mMessageSystem->addMessage("You are now wearing " + item->getName() + ".");
      mArmor = (dItemArmor*) item;
      return true;
    }
    else if(item->getValue() > mArmor->getValue())
    {
      mMessageSystem->addMessage("You are now wearing " + item->getName() + ".");
      delete mArmor;
      mArmor = (dItemArmor*) item;
      return true;
    }
    else
      return false;
  }
  else
  {
    //fail safe check... I don't do that very often!
    if(hasItem(item))
      return false;
    //can only carry four items. I could claim this is for realism purposes,
    //but it's really because I didn't add scrolling to the item menu :P
    if(mItems.size() >= 4)
      return false;
    mItems.push_back(item);
    return true;
  }
}

void dInventory::destroyItem(dItem *item)
{
  for(deque<dItem*>::iterator i=mItems.begin(); i!=mItems.end(); i++)
    if(*i == item)
    {
      delete (*i);
      *i = NULL;
      mItems.erase(i);
      return;
    }
}

void dInventory::dropItem(dItem *item)
{
  for(deque<dItem*>::iterator i=mItems.begin(); i!=mItems.end(); i++)
    if(*i == item)
    {
      *i = NULL;
      mItems.erase(i);
      return;
    }
}

bool dInventory::hasItem(dItem *item)
{
  for(deque<dItem*>::iterator i=mItems.begin(); i!=mItems.end(); i++)
    if(*i == item)
      return true;
  return false;
}

deque<dItem*>* dInventory::getItemList()
{
  return &mItems;
}

dItemWeapon* dInventory::getWeapon()
{
  return mWeapon;
}

dItemArmor* dInventory::getArmor()
{
  return mArmor;
}

/*
void dInventory::equipMenu()
{
  //TODO: don't do what this does (you know what I mean)
  if(mWeapon)
    addItem(mWeapon);
  if(mArmor)
    addItem(mArmor);
  mWeapon = NULL;
  mArmor = NULL;

  for(deque<dItem*>::iterator i=mItems.begin(); i!=mItems.end(); i++)
  {
    if(!mWeapon && (*i)->getType() == DITEM_WEAPON)
    {
      mWeapon = (dItemWeapon*) *i;
      mMessageSystem->addMessage("You are now weilding a " + (*i)->getName() + ".");
      //dropItem(*i);
      //return;
    }
    else if(!mArmor && (*i)->getType() == DITEM_ARMOR)
    {
      mArmor = (dItemArmor*) *i;
      mMessageSystem->addMessage("You are now wearing a " + (*i)->getName() + ".");
      //dropItem(*i);
      //return;
    }
  }
  dropItem(mWeapon);
  dropItem(mArmor);
}*/
