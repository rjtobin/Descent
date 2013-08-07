
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

#include "stats.hpp"

dStats::dStats()
: mStrength(0), mSpeed(0), mConstitution(0), mMaxHealth(0), mCurHealth(0)
{
}

dStats::dStats(int strength, int speed, int constitution, int health)
: mStrength(strength), mSpeed(speed), mConstitution(constitution), mMaxHealth(health), mCurHealth(health)
{
}

int& dStats::getStrength()
{
  return mStrength;
}

int& dStats::getSpeed()
{
  return mSpeed;
}

int& dStats::getConstitution()
{
  return mConstitution;
}

int dStats::getStrengthMod()
{
  return (mStrength-10)/2;
}

int dStats::getSpeedMod()
{
  return (mSpeed-10)/2;
}

int dStats::getConstitutionMod()
{
  return (mConstitution-10)/2;
}

int& dStats::getMaxHealth()
{
  return mMaxHealth;
}

int& dStats::getCurHealth()
{
  return mCurHealth;
}
