/**
 * @file
 * @author Dennis J�nsson <zelebsel@gmail.com>
 * @version 1.0
 * Copyright (�) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 * 
 * @section DESCRIPTION
 *
 * This is the class for the Black Widow
 */

#include "BlackWidow.h"
#include <iostream>

using namespace std;
using namespace logic;


BlackWidow::BlackWidow(double hp,double damage,Sizes size,int aiStyle,int range) :
	Enemy(hp,damage,size,aiStyle,range)
{
	mFire = false;
	mpEnemyWeapon = myNew AcidSpit(damage, 1.0f,range);
	mpEnemyWeapon->setOwner(this->getKey());
}

BlackWidow::~BlackWidow()
{
	SAFE_DELETE(mpEnemyWeapon);
}

void BlackWidow::attack()
{
 //if enemy is in range of tank
   mpEnemyWeapon->fireWeapon();	
}

void BlackWidow::update(float deltaTime)
{
	if(mFire)
	{	
		attack();
	}
	Actor::mPosition.x += (SPIDER_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (SPIDER_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (SPIDER_MOVE_SPEED * Actor::mDirection.z * deltaTime);	
}

void BlackWidow::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}