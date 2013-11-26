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

#ifndef __BEAM_H__
#define __BEAM_H__

namespace logic
{

/**
 * Subclass to munitions, it holds the beamcannons shot
 */

class Beam : public Munitions
{
public:

/**
 * Constructor
 * @param networkId the network id of the player
 * @param position the beams origin
 * @param direction the beams direction
 * @param efficiency the weapon shooting this ammos efficiency rating
 * @param damage the damage of the weapon shooting this ammo
 */
	Beam(int networkId, utilities::Position position, utilities::Direction direction, int efficiency, float damage);
/**
 * Deconstructor
 */
	~Beam();
/**
 * updates beams position
 * @param deltaTime the time between calls, used in calculating movement
 */
	void update(float deltaTime);
/**
 * handles various events relevant to beam
 * @param eventMessage the event to be handled
 */
	void handleEventId(EventData const &eventMessage);
/**
 * function that returns the name of the actor class
 */
	std::string getName(void) {return "Beam";}

};
}

#endif