/**
 * @file
 * @author Gustav Persson <gustav@alltomminrov.se>
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
 * Event listener that handles events fired in real time,
 * if needed for two real time processes to communicate,
 * it lets the threads themselves handle the events, by just
 * putting them in a queue.
 * 
 */
#include "RealTimeEventListener.h"

using namespace logic;

bool RealTimeEventListener::handleEvent(const EventData &event)
{
	EventDataPtr pEvent = event.copy();
	gRealTimeEventQueue.push(pEvent);
	return true;
}