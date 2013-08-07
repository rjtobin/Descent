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

#ifndef NPC_HPP
#define NPC_HPP

#include "display.hpp"
#include "map.hpp"
#include "stats.hpp"
#include "message.hpp"
#include "inventory.hpp"

#include "random.hpp"

#include <string>
using namespace std;

class dMap;
class dTileModel;
class dInventory;

class dNPC_Player;

class dNPC
{
private:
  dMap *mMap;
  int mX, mY;
  dStats mStats;
public:
  dNPC();
  dNPC(int x, int y, dMap *map);

  virtual void makeMove(dMessageSystem *message, dNPC_Player *player) = 0;
  virtual dTileModel getModel() = 0;
  virtual string getName() = 0;

  int& getX();
  int& getY();

  dStats& getStats();

  //these two should really be over-ridden in any derived classes:
  virtual int getArmorClass();
  virtual int getSpeedClass();

  virtual int attackRoll();
  virtual int dodgeRoll();

  virtual int getAttackDamage() = 0;

  virtual int getExpGained();

  virtual bool isPlayer();

  virtual void attack(dNPC *target, dMessageSystem *messageSystem);

  dMap* getMap();
  void setMap(dMap *map);
};

class dNPC_Kobold : public dNPC
{
private:
public:
  dNPC_Kobold();
  dNPC_Kobold(int x, int y, dMap *map);

  void makeMove(dMessageSystem *messageSystem, dNPC_Player *player);
  dTileModel getModel();
  string getName();

  int getArmorClass();
  int getSpeedClass();
  int getExpGained();

  int getAttackDamage();
};

class dNPC_Gnoll : public dNPC
{
private:
public:
  dNPC_Gnoll();
  dNPC_Gnoll(int x, int y, dMap *map);

  void makeMove(dMessageSystem *messageSystem, dNPC_Player *player);
  dTileModel getModel();
  string getName();

  int getArmorClass();
  int getSpeedClass();
  int getExpGained();

  int getAttackDamage();
};

class dNPC_Troll : public dNPC
{
private:
public:
  dNPC_Troll();
  dNPC_Troll(int x, int y, dMap *map);

  void makeMove(dMessageSystem *messageSystem, dNPC_Player *player);
  dTileModel getModel();
  string getName();

  int getArmorClass();
  int getSpeedClass();
  int getExpGained();

  int getAttackDamage();
};

class dNPC_Ooze : public dNPC
{
private:
public:
  dNPC_Ooze();
  dNPC_Ooze(int x, int y, dMap *map);

  void makeMove(dMessageSystem *messageSystem, dNPC_Player *player);
  dTileModel getModel();
  string getName();

  int getArmorClass();
  int getSpeedClass();
  int getExpGained();

  int getAttackDamage();
};

class dNPC_Ghost : public dNPC
{
private:
public:
  dNPC_Ghost();
  dNPC_Ghost(int x, int y, dMap *map);

  void makeMove(dMessageSystem *messageSystem, dNPC_Player *player);
  dTileModel getModel();
  string getName();

  int getArmorClass();
  int getSpeedClass();
  int getExpGained();

  int getAttackDamage();
};

class dNPC_EvilDM : public dNPC
{
private:
public:
  dNPC_EvilDM();
  dNPC_EvilDM(int x, int y, dMap *map);

  void makeMove(dMessageSystem *messageSystem, dNPC_Player *player);
  dTileModel getModel();
  string getName();

  int getArmorClass();
  int getSpeedClass();
  int getExpGained();

  int getAttackDamage();
};

class dNPC_Player : public dNPC
{
private:
  dInventory *mInventory;
  int mExperience;
  int mLevel;
public:
  dNPC_Player();
  dNPC_Player(int x, int y, dMap *map, dInventory *inventory);

  bool isPlayer();

  void makeMove(dMessageSystem *messageSystem, dNPC_Player *player);
  dTileModel getModel();
  string getName();

  int getArmorClass();
  int getSpeedClass();

  int getAttackDamage();

  int& getExp();
  int& getLevel();
};

#endif
