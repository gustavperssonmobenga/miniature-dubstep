/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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
 */


#include "LogicQuadTree.h"
#include <vector>
#include <cmath>
#include "ActorHandler.h"
#include "PlayerInfo.h"

using namespace logic;
using namespace gfx;
using std::vector;

LogicQuadTree* LogicQuadTree::mpsInstance = NULL;

LogicQuadTree::~LogicQuadTree()
{
	mpsInstance = NULL;
}

LogicQuadTree* LogicQuadTree::getInstance()
{
	if(mpsInstance == NULL)
	{
		mpsInstance = myNew LogicQuadTree();
	}

	return mpsInstance;
}

void LogicQuadTree::collisionRay(const D3DXVECTOR3 &origin, const D3DXVECTOR3 &direction, KeyType actorID, KeyType bulletID, bool beam)
{	
	std::vector<CollisionObject*> collisionObjects;
	std::vector<CollisionObject*> sortedCollisionObjects;
	float length = 0.0f;
	mQuadTree.intersection(origin.x, origin.y, origin.z, direction.x, direction.y,
						   direction.z, (float)INT_MAX, &length, actorID, &collisionObjects);
	
	while(collisionObjects.size()>0)
	{
		float minLenght = 1000000.0f;
		int closestObject = 0;

		for(unsigned int i=0; i<collisionObjects.size(); i++)
		{
			if(collisionObjects[i]->getDistance(origin) < minLenght)
			{
				minLenght = collisionObjects[i]->getDistance(origin);
				closestObject = i;
			}
		}

		sortedCollisionObjects.push_back(collisionObjects[closestObject]);
		collisionObjects.erase(collisionObjects.begin() + closestObject);
	}

	for(unsigned int i=0; i<sortedCollisionObjects.size(); i++)
	{
		if(sortedCollisionObjects[i]->getID()!=KEY_GROUND && sortedCollisionObjects[i]->getID()!=INVALID_KEY && sortedCollisionObjects[i]->getID() != KEY_TRIGGER)
		{
			Actor *pActor = mpActorHandler->getActor(sortedCollisionObjects[i]->getID());
			if(pActor != NULL)
			{
				if(pActor->getActorType() == Actor::ActorType_StaticEnvironment)
				{
					break;
				}
				if(pActor->getActorType() != Actor::ActorType_Munition)
				{
					D3DXVECTOR3 pos =  sortedCollisionObjects[i]->getIntersectionPoint();
					safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_ActorCollisionRay(pos,bulletID, sortedCollisionObjects[i]->getID(),actorID)));

					if(!beam)
					{
						break;
					}
				}
			}
		}
	}
}

void LogicQuadTree::collisionObject(KeyType mId, float deltatime)
{
	utilities::Direction newDir, avrgNormal;
	utilities::Position newPos;
	float mX, mY, cX, cY, mAngle, cAngle;
	//bool firstCollision = true;
	avrgNormal = utilities::Direction(0.0f, 0.0f, 0.0f);

	bool trueCollision;
	std::vector<logic::KeyType> mCheckedIds;
	vector<CollisionObject *> collisions;
	Actor *pActor = mpActorHandler->getActor(mId);
	
	gfx::CollisionObject *BV = pActor->getPreCollisionObject();
	if(BV)
	{
		mQuadTree.collisionCheck(BV, &collisions);
		for(vector<CollisionObject *>::iterator BVIter = collisions.begin();
			BVIter != collisions.end(); ++BVIter)
		{
			trueCollision = true;

			Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor((*BVIter)->getID()));
			if((*BVIter)->getID() == KEY_GROUND || (*BVIter)->getID() == BV->getID() || pMunition != NULL)
			{
				continue;
			}
			if((*BVIter)->getID() == KEY_TRIGGER)
			{
				//Send trigger collision event.
				if(pActor->getActorType() == Actor::ActorType_Tank || pActor->getActorType() == Actor::ActorType_Driver)
				{
					safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_TriggerCollision((void *)(*BVIter))));
				}
				continue;
			}
			for(unsigned int i = 0; i < mCheckedIds.size(); i++)
			{
				if(mCheckedIds[i] == (*BVIter)->getID() || (pMunition != NULL && !pMunition->getVisible()))
				{
					trueCollision = false;
				}
			}
			if(trueCollision)
			{   
				utilities::Direction collisionNormal;
				collisionNormal.x = (*BVIter)->getCollisionNormal().x;
				collisionNormal.y = (*BVIter)->getCollisionNormal().y;
				collisionNormal.z = (*BVIter)->getCollisionNormal().z;

				mAngle = pActor->getPreDirection().getXZAngleCounterclockwise();
				cAngle = collisionNormal.getXZAngleCounterclockwise();
				mX = cos(mAngle);
				mY = sin(mAngle);
				cX = cos(cAngle);
				cY = sin(cAngle);
				newDir = utilities::Direction((mX + cX), 0.0f, (mY + cY));
				newDir.normalize();
				if(pActor->getSpeed() >= 0.0f)
				{
					newPos = pActor->getPrePosition() - newDir * pActor->getSpeed() * deltatime;
				}
				else if(pActor->getSpeed() < 0.0f)
				{
					newPos = pActor->getPrePosition() + newDir * pActor->getSpeed() * deltatime;
				}
				pActor->setPosition(newPos);


				char* Flame = "Flame";
				if(pMunition != NULL && pMunition->getName() != Flame)
					pMunition->setVisible(false);
	
				
				safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorCollisionObject(BV->getID(), (*BVIter)->getID(), collisionNormal)));
				mCheckedIds.push_back((*BVIter)->getID());
			}
		}
	}
}

void LogicQuadTree::collisionMunition(KeyType mId)
{
	bool trueCollision;
	std::vector<logic::KeyType> mCheckedIds;
	vector<CollisionObject *> collisions;
	Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mId));
	gfx::InstanceMesh *mesh = dynamic_cast<gfx::InstanceMesh*>(pMunition->getTransformObject());
	if(mesh != NULL || pMunition != NULL || pMunition->getWeaponType() != Weapon::WeaponTypes_Invalid || pMunition->getOwnerType() != Actor::ActorType_Invalid)
	{		
		for(vector<gfx::InstanceSubset *>::const_iterator subsetIter = mesh->getSubsetList().begin();
			subsetIter != mesh->getSubsetList().end(); ++subsetIter)
		{
			gfx::CollisionObject *BV = (*subsetIter)->getBoundingVolume();
			if(BV)
			{
				mQuadTree.collisionCheck(BV, &collisions);
				for(vector<CollisionObject *>::iterator BVIter = collisions.begin();
					BVIter != collisions.end(); ++BVIter)
				{
					trueCollision = true;

					if((*BVIter)->getID() == KEY_GROUND || (*BVIter)->getID() == BV->getID() || (*BVIter)->getID() == KEY_TRIGGER)
					{
						continue;
					}
					Actor::ActorTypes actorType = mpActorHandler->getActor((*BVIter)->getID())->getActorType(); 
					if((actorType != Actor::ActorType_StaticEnvironment && actorType != Actor::ActorType_Enemy && actorType != Actor::ActorType_Tank && actorType != Actor::ActorType_Driver))
					{
						continue;
					}
					//Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(BV->getID()));
					if(pMunition != NULL && pMunition->getOwner() == (*BVIter)->getID())
					{
						continue;
					}
					for(unsigned int i = 0; i < mCheckedIds.size(); i++)
					{
						if(mCheckedIds[i] == (*BVIter)->getID())
						{
							trueCollision = false;
						}
					}
					if(trueCollision)
					{		
						utilities::Direction collisionNormal;
						collisionNormal.x = (*BVIter)->getCollisionNormal().x;
						collisionNormal.y = (*BVIter)->getCollisionNormal().y;
						collisionNormal.z = (*BVIter)->getCollisionNormal().z;

						if(pMunition->getVisible())
							safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorCollisionObject(BV->getID(), (*BVIter)->getID(), collisionNormal)));
						
						pMunition->setVisible(false);

						mCheckedIds.push_back((*BVIter)->getID());
					}
				}
			}
		}
	}
}

void LogicQuadTree::collisionCubeMunition(logic::KeyType mId)
{
	bool trueCollision;
	std::vector<logic::KeyType> mCheckedIds;
	vector<CollisionObject *> collisions;
	Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mId));
	gfx::InstanceShapeCube *cube = dynamic_cast<gfx::InstanceShapeCube*>(pMunition->getTransformObject());
	if(cube != NULL || pMunition != NULL || pMunition->getWeaponType() != Weapon::WeaponTypes_Invalid || pMunition->getOwnerType() != Actor::ActorType_Invalid)
	{		
		gfx::CollisionObject *BV = cube->getBoundingVolume();		
		if(BV)
		{
			mQuadTree.collisionCheck(BV, &collisions);
			for(vector<CollisionObject *>::iterator BVIter = collisions.begin();
				BVIter != collisions.end(); ++BVIter)
			{
				trueCollision = true;

				if((*BVIter)->getID() == KEY_GROUND || (*BVIter)->getID() == BV->getID() || (*BVIter)->getID() == KEY_TRIGGER)
				{
					continue;
				}
				Actor::ActorTypes actorType = mpActorHandler->getActor((*BVIter)->getID())->getActorType(); 
				if((actorType != Actor::ActorType_StaticEnvironment && actorType != Actor::ActorType_Enemy && actorType != Actor::ActorType_Tank && actorType != Actor::ActorType_Driver))
				{
					continue;
				}
				//Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(BV->getID()));
				if(pMunition != NULL && pMunition->getOwner() == (*BVIter)->getID())
				{
					continue;
				}
				for(unsigned int i = 0; i < mCheckedIds.size(); i++)
				{
					if(mCheckedIds[i] == (*BVIter)->getID())
					{
						trueCollision = false;
					}
				}
				if(trueCollision)
				{		
					utilities::Direction collisionNormal;
					collisionNormal.x = (*BVIter)->getCollisionNormal().x;
					collisionNormal.y = (*BVIter)->getCollisionNormal().y;
					collisionNormal.z = (*BVIter)->getCollisionNormal().z;
					if(pMunition->getVisible())
						safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorCollisionObject(BV->getID(), (*BVIter)->getID(), collisionNormal)));

					Actor *pColActor = mpActorHandler->getActor((*BVIter)->getID());
					if(pColActor != NULL 
						&& pMunition->getActorSubType() == Munitions::MunitionType_Pulse 
						&& pColActor->getActorType() == Actor::ActorType_StaticEnvironment)
					{
						pMunition->setVisible(false);
					}

					mCheckedIds.push_back((*BVIter)->getID());
				}
			}
		}
	}
}

void LogicQuadTree::updateQuadTree()
{
	DX10Module *module = DX10Module::getInstance();

	mBVs.clear();
	module->copyBoundingVolumes(&mBVs);
	mQuadTree.update(&mBVs);
}

LogicQuadTree::LogicQuadTree() : mBVs(), mQuadTree(-10000.0f, -10000.0f, 10000.0f, 10000.0f, 6)
{
	mpActorHandler = ActorHandler::getInstance();
}