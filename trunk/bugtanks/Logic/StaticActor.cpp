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

#include "StaticActor.h"

#ifdef USE_AI
#include "../Ai/AiFactory.h"
#endif

using namespace logic;

StaticActor::StaticActor(utilities::Position pos, utilities::Direction dir, int networkId, const utilities::MapCoordinate& size) : Actor(pos, dir, networkId, size)
{
}

StaticActor::~StaticActor()
{
}