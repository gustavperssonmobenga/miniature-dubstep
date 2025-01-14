#include "Macros.h"
#include "Event.h"
#include "EventRouter.h"
#include "Client.h"
#include <windows.h>
#include <iostream>
#include <sstream>

int main() {
	initMemoryCheck();
	DEBUG_MESSAGE("Testing shit");

	//std::stringstream ss;
	//ss << 195;
	//std::string text;
	//ss >> text;
	//ss.clear();
	//ss << "Banana";
	//DEBUG_MESSAGE(text << ", " << ss);

	std::string text;
	text += static_cast<unsigned char>(195);
	DEBUG_MESSAGE(static_cast<unsigned char>(195) << ", " << text);

	EventRouter* pEventRouter = EventRouter::getInstance();

	// Add a client
	DEBUG_MESSAGE("Adding client");
	Client* pClient = myNew Client(0);
	pClient->detachAndRun();
	std::cin.get();
	pEventRouter->detachAndRun();

	// Test to create an event and send it
	DEBUG_MESSAGE("Creating First Enemy");
	pEventRouter->pushEvent(myNew Event(Event_EnemyCreate, INVALID_KEY, 0, myNew FixedCoordinateData(20, 10)));

	//DEBUG_MESSAGE("Creating Second Enemy");
	//pEventRouter->pushEvent(myNew Event(Event_EnemyCreate, INVALID_KEY, 0, myNew FixedCoordinateData(10, 10)));

	// Creating 10 enemies
	DEBUG_MESSAGE("Creating Agents");
	for (int i = 0; i < 5; i++)
	{
		pEventRouter->pushEvent(myNew Event(Event_AgentCreate, INVALID_KEY, 0, myNew FixedCoordinateData(9, 8+i)));
		//pEventRouter->pushEvent(myNew Event(Event_AgentCreate, INVALID_KEY, 0, myNew FixedCoordinateData(9, 8+i)));
	}
	//pEventRouter->pushEvent(myNew Event(Event_AgentCreate, INVALID_KEY, 0, myNew FixedCoordinateData(0, 0)));

	std::cin.get();
	SAFE_DELETE(pClient);
	SAFE_DELETE(pEventRouter);

	std::cin.get();
	return 0;
}