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
 * This is the class for the most basic of the games enemies
 */

#include "Ant.h"

using namespace logic;

Ant::Ant(double hp,double damage,Sizes size,int aiStyle,float range) :
	Enemy(hp,damage,size,aiStyle,range)
{
	mpRenderObject = gfx::DX10Module::getInstance()->createMeshInstanced("LongRangeTank");
	mpRenderObject->setID(mId);
	mpRenderObject->setName("Ant");
	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpRenderObject)->setScale(D3DXVECTOR3(2.5f, 1.5f, 2.5f));
	mpRenderObject->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
}

Ant::~Ant()
{

}

void Ant::attack()
{
  //if enemy is in range of tank
  //send event to have the enemy perform his attack animation
  //send event to collision checking for a hit
}

void Ant::update(float deltaTime)
{   
	Actor::mPosition.x += (ANT_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (ANT_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (ANT_MOVE_SPEED * Actor::mDirection.z * deltaTime);

	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
}


void Ant::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}