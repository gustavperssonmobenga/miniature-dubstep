/**
 * @file
 * @author Gustav Persson <ninjamuff@gmail.com>
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
 * An event listener for the logic component.
 */
#include "../Utilities/Macros.h"

#include "GFXListener.h"
#include "Weapon.h"
#include "InputStates.h"
#include "../GFX//DX10Module.h"
#include "Application.h"

using namespace logic;

GFXListener::GFXListener()
{

}

GFXListener::~GFXListener()
{

}

bool GFXListener::handleEvent(const logic::EventData &mEvent)
{
	mEvents.add(mEvent.copy());
	
	return false;
}

utilities::VectorList<EventDataPtr>& GFXListener::getEventList()
{
	return mEvents;
}