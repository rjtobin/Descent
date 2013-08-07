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

#include "message.hpp"

void dMessageSystem::addWord(string word)
{
  if(mMessages.size() > 0 && word.size() + mMessages.back().size() < mScreenWidth)
    mMessages.back().append(" " + word);
  else
    mMessages.push_back(word);
}

dMessageSystem::dMessageSystem()
: mLastMessage(" ")
{
}

dMessageSystem::dMessageSystem(int width)
: mScreenWidth(width), mLastMessage(" ")
{
}

/* Note that the usage of this function is very particular. It breaks strings into words through spaces. So don't do crazy things with spaces (such as several spaces in a row, or starting the string with a space). */
void dMessageSystem::addMessage(string msg)
{
  int last=0;
  string word;
  for(int i=0; i<msg.length(); i++)
  {
    if(msg[i] == ' ')
    {
      word = msg.substr(last, i-last);
      addWord(word);
      last = i+1;
    }
  }
  word = msg.substr(last, msg.length()-last);
  addWord(word);

}

string dMessageSystem::getMessage()
{
  if(mMessages.size() == 0)
    return mLastMessage;
  mLastMessage = mMessages.front();
  mMessages.pop_front();
  return mLastMessage;
}

bool dMessageSystem::areMessagesWaiting()
{
  return (mMessages.size() != 0);
}
