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

#include "../Utilities/Macros.h"
#include "Munitions.h"

#ifndef __FIRE_H__
#define __FIRE_H__

namespace logic
{

/**
 * Subclass to munitions, it holds fire spit projectiles  
 */

class Fire : public Munitions
{
public:

/**
 * Constructor
 * @param position the fire spits origin
 * @param efficiency the weapon shooting this ammos efficiency rating
 * @param damage the damage of the weapon shooting this ammo
 */
	Fire(utilities::Position position, int efficiency, double damage);
/**
 * Deconstructor
 */
	~Fire();
/**
 * update fire spits position
 * @param deltaTime the time between calls, used to calculate movement
 */
	void update(float deltaTime);

/**
 * handles various events related to fire
 * @param eventMessage the event to be handled
 */
	void handleEventId(EventData const &eventMessage);

};
}

#endif