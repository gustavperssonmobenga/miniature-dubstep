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
 */

#ifndef __FIREANT_H__
#define __FIREANT_H__

#include "../Utilities/Macros.h"
#include "Enemy.h"

namespace logic
{

/**
 * This is the class for fire ant
 */

class FireAnt:public Enemy
{
public:
/**
 * Constructor
 * @param networkId the network id of the player
 * @param hp the enemies healthpoints
 * @param damage the amount of damage this enemy does
 * @param size the size of the enemy, used in damage calculations
 * @param range how long range the unit have, incase of melee unit the range is a low value, probably between 0 and 1
 * @param pos the position of this actor
 * @param dir the direction of this actor
 */
	FireAnt(int networkId, utilities::Position pos, utilities::Direction dir);
/**
 * Destructor
 */
	~FireAnt();
/**
 * Function that is called when an enemy wants to attack a driver, this is likely an event from AI
 */
	void attack();
/**
 * update position for fire ant
 * @param deltaTime time between calls, used to calculate movement
 */
	void update(float deltaTime);
/**
 * handles various events that is relevant for this class
 * @param eventMessage event to be handled
 */
	void handleEventId(EventData const &eventMessage);
/**
 * function that returns the name of the actor class
 */
	std::string getName(void) {return "FireAnt";}
	static std::string getEnemyName(void) {return "FireAnt";}
	static std::string getEnemyModelName(void) {return "Ant";}
};
}

#endif