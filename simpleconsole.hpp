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

#ifndef SIMPLECONSOLE_HPP
#define SIMPLECONSOLE_HPP

#include <string>
#include <queue>
#include <SDL/SDL.h>
#include "SDL/SDL_ttf.h"

using namespace std;

class SimpleConsole
{
private:
  int mWidth, mHeight;
  bool mIsInit;
  bool mIsKeyPressed[256];

  int mKeyDownTime[256];

  queue<unsigned short int> mKeyQueue;

  SDL_Surface *mScreen;
  TTF_Font *mFont;

public:
  SimpleConsole();
  SimpleConsole(int width, int height);

  bool isKeyPressed(unsigned short int key);

  bool areKeysLeft();
  unsigned short int getKey();

  bool init();
  bool isInit();

  bool refresh();
  bool drawString(int x, int y, string s, int r=255, int g=255, int b=255);
  bool drawCharacter(int x, int y, char c, int r=255, int g=255, int b=255);

  void reset();
  void reset(int x, int y, int width, int height);

  bool handleEvents();
  void wait(int ms);
  unsigned int time();

  void waitForKeyPress();

  ~SimpleConsole();
};

#endif
