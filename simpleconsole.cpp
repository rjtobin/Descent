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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "simpleconsole.hpp"

SimpleConsole::SimpleConsole()
: mWidth(800), mHeight(600), mScreen(NULL), mIsInit(false), mFont(NULL)
{
}

SimpleConsole::SimpleConsole(int width, int height)
: mWidth(width), mHeight(height), mScreen(NULL), mIsInit(false), mFont(NULL)
{
}

bool SimpleConsole::isKeyPressed(unsigned short int key)
{
  return mIsKeyPressed[key];
}

bool SimpleConsole::areKeysLeft()
{
  return (mKeyQueue.size() > 0);
}

unsigned short int SimpleConsole::getKey()
{
  if(!areKeysLeft())
    return 0;
  unsigned short int ret = mKeyQueue.front();
  mKeyQueue.pop();
  return ret;
}

bool SimpleConsole::init()
{
  for(int i=0; i<256; i++)
    mIsKeyPressed[i] = false;

  if(isInit())
    return false;

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
    return false;
  
  mScreen = SDL_SetVideoMode(mWidth, mHeight, 8, SDL_SWSURFACE);
  if(mScreen == NULL)
    return false;

  if(TTF_Init() == -1)
    return false;

  mFont = TTF_OpenFont("cour.ttf", 16);

  mIsInit = true;
  return true;
}

bool SimpleConsole::isInit()
{
  return mIsInit;
}

bool SimpleConsole::refresh()
{
  SDL_UpdateRect(mScreen, 0, 0, mWidth, mHeight);
  return true;
}

bool SimpleConsole::drawString(int x, int y, string s, int r, int g, int b)
{
  SDL_Color color = {r,g,b};
  SDL_Surface *text;



  text = TTF_RenderText_Solid(mFont, s.c_str(), color);

  if(text == NULL)
  {
    return false;
  }

  SDL_Rect dest;
  dest.x = x, dest.y = y;
  dest.w = dest.h = 0;
  SDL_BlitSurface(text, NULL, mScreen, &dest);
  SDL_FreeSurface(text);
  return true;
}

bool SimpleConsole::drawCharacter(int x, int y, char c, int r, int g, int b)
{
  string s ="";
  s += c;
  return drawString(x, y, s, r, g, b);
}

bool SimpleConsole::handleEvents()
{
  SDL_PumpEvents();
  SDL_Event event;

  if(!(time()%100))
  {
    for(int i=0; i<256; i++)
      if(mIsKeyPressed[i] && time() - mKeyDownTime[i] > 300)
        mKeyQueue.push(i);
  }

  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_ACTIVEEVENT)
    {
      refresh();
    }
    else if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.sym >= 0 && event.key.keysym.sym < 256)
      {
        mIsKeyPressed[event.key.keysym.sym] = true;
        mKeyQueue.push(event.key.keysym.sym);
        mKeyDownTime[event.key.keysym.sym] = time();
      }
    }
    else if(event.type == SDL_KEYUP)
    {
      if(event.key.keysym.sym >= 0 && event.key.keysym.sym < 256)
        mIsKeyPressed[event.key.keysym.sym] = false;
      mKeyDownTime[event.key.keysym.sym] = -1;
    }
  }
  return true;
}

void SimpleConsole::reset()
{
  SDL_FillRect(mScreen, NULL, SDL_MapRGB(mScreen->format, 0,0,0));
}

void SimpleConsole::reset(int x, int y, int width, int height)
{
  SDL_Rect r;
  r.x = x, r.y = y, r.w = width, r.h = height;
  SDL_FillRect(mScreen, &r, SDL_MapRGB(mScreen->format, 0, 0, 0));
}

void SimpleConsole::wait(int ms)
{
  SDL_Delay(ms);
}

unsigned int SimpleConsole::time()
{
  return SDL_GetTicks();
}

void SimpleConsole::waitForKeyPress()
{
  handleEvents();

  SDL_Event event;
  while(!SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENTMASK (SDL_KEYDOWN)))
    SDL_PumpEvents();
  return;
}

SimpleConsole::~SimpleConsole()
{
  TTF_CloseFont(mFont);
  SDL_FreeSurface(mScreen);
  SDL_Quit();
}


