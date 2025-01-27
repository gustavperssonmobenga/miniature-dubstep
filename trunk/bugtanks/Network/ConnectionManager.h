/**
 * @file ConnectionManager.h
 * @author Martin Wex� Olsson <mawm06@student.bth.se>
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

#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_

#include <winsock.h>

#include <list>
#include <pthread.h>

#include "MessageHandler.h"


namespace network
{
/// Header for the messages beeing sent or received.
struct msgHeader_t
{
	int len;
	int type;
};

/**
 * The ConnectionManager manage connections, that is sockets that are connected.
 * For each socket added to this manager a new thread is created that will listen
 * for incoming messages. Each new message is beeing handled by a set of registered
 * message handlers.
 */
class ConnectionManager
{
	/// A datastructure for handling socket connections.
	struct connection_t
	{
		SOCKET soc;
		pthread_t socketListener;
		ConnectionManager *manager;
	};
	struct holdingBuffer_t
	{
		char buffer[24000];
		int bytesHeld;
	};
private:

	std::list<connection_t*> mConnections;
	std::list<MessageHandler*> mMessageHandlers;

	/**
    * This function handles incoming messages from each socket. This is the thread function
	* that is created for each added connection.
	* @param args a pointer to the functions argument.
    */
	static void *socketFunc(void* args);
	

	// Private helper function.
	/**
    * This function sort out each infividual package from the received package buffer and
	* pass them on to be handled by all the registered MessageHandlers.
    */
	void handleMessages(SOCKET src, char *messagebuffer, unsigned int len, holdingBuffer_t *holdingBuffer);

public:

	/**
    * A constructor.
    */
	ConnectionManager();

	/**
    * A destructor.
    */
	virtual ~ConnectionManager();
	
	/**
    * This will add the socket to the manager and create a new thread for handling
	* incoming messages.
    * @param soc a socket that is already connected to a peer.
    */
	void addConnection(SOCKET soc);

	/**
	* This function will close the socket and wait for the socket thread to finish.
	* @param soc is the socket descriptor to the connection you want to remove.
	*/
	void removeConnection(SOCKET soc);
	
	/**
    * Add message handlers to handler incoming messages.
    * @param msgH a pointer to a MessageHandler object.
    */
	void addMessageHandler(MessageHandler *msgH);
	
	/**
    * Removes the specified message handler.
    * @param msgH a pointer to a MessageHandler object.
    */
	void removeMessageHandler(MessageHandler *msgH);

	/**
    * Removes all message handlers.
    */
	void removeAllMessageHandlers();
	
	/**
    * Sends a message to all connections associated with this manager.
    * @param len the length of the message in bytes.
    * @param type the type of the message.
	* @param message the actual message.
    */
	void sendToAll(int len, int type, const char* message);
	
	/**
    * Sends a message to a connections associated with this manager.
	* @param dest the designated socket to send the message to.
    * @param len the length of the message in bytes.
    * @param type the type of the message.
	* @param message the actual message.
    */
	void sendTo(SOCKET dest, int len, int type, const char* message);
	
	/**
    * Get number of connections associated with the manager.
    * @return the number of connections associated with the manager.
    */
	int getNumberOfConnections();

	/**
	* Closes all connections.
	*/
	void closeAll();

private:
	pthread_mutex_t mMutex;
	pthread_attr_t mListenerAttribute;

};
}

#endif
