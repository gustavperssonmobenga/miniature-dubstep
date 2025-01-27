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
*/

#ifndef __EVENT_MSG_HANDLER_H__
#define __EVENT_MSG_HANDLER_H__

#include "MessageHandler.h"
#include "IdManager.h"

namespace network
{

/**
* Handles incoming event messages.
*/
class EventMsgHandler : public MessageHandler
{
public:
	/**
	* Constructor. Empty, just starts instance.
	*/
	EventMsgHandler();

	/**
	* Handles incoming network message. Creates a stream and processes the information,
	* creates an event and sends it over to the event manager.
	* @param src The source socket.
	* @param len The length of the message
	* @param type The message type (Should be MessageType_Event, otherwise not interesting)
	* @param message The incoming message as a c-string.
	*/
	void handleMessage(SOCKET src, int len, int type, const char * message);
	
private:
};
}

#endif