#ifndef HANDLER_H
#define HANDLER_H
#include <queue>
#include <memory>
#include "SEvent.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <thread>
#include <algorithm>
#include <chrono>
#include <thread>
#include <memory>
using namespace std;


/** Generic Handler class. This class is the basic building block to represent any component
in the system that could need servicing or to service others. These Handlers will
communicate with each other by sending SEvent messages to each other to allow each instance to
operate asynchronously.

@author Cameron Shepherd
*/
class Handler
{

public:
	
	/***Constructor 1. Default constructor*/
	Handler();

	/** Constructor 2. Constructs a Handler with a name.
		@param name: The name of the constructed Handler.
		*/
	Handler(string name);

	/** Constructor 3. Constructs a Handler with a name, and immediately sets up itself and its creator
		as being neighbours.
		@param name: The name of the constructed Handler
		@param creator: The name of the creator
		@param _outputQueue: The message queue for the creator
	*/
	Handler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue);

	/*** Default destructor. */
	~Handler();

	/** Updates the Handler. This will update the entire instance, calling own default behaviour then calling the 
		inherited-member behaviour through updateH().
		*/
	void update();

	/** Add a new neighbour. This will add the given neighbour to the set of neighbour {link Handlers}s, resulting in broadcasts 
	of messages going out to it.
	@param _queue: the queue open for communication to the Handler
	@param name: the name of the Handler to be added
	*/
	void setNeighbour(weak_ptr<queue<SEvent>> _queue, string name);

	/** Set the name of this Handler. This will only set the internal name, this will not alert other Handlers to the name change
		@param name: the new name for this Handler
	*/
	void setName(string name);

	/** Add the given SType to those not to be forwarded. This will guarantee that only a single cop of the SType will
		exist in the doNotForward vector.
		@param sType: The type of message to be added to the blacklist
	*/
	void addDoNotForward(SType sType);

protected:
	/** Broadcast the given message. This will broadcast the message to all neighbours, except the source of the message. The message sent will 
		have the sender changed to have come from the calling {link Handler} instance.
		@param theEvent: The message to be sent
	*/
	void broadcastMessage(SEvent theEvent);

	/** Send a message only to the specified target. This will send the message to the specified target, as long as it is a neighbour. If the 
		target is not a neighbour then the message will not be sent. The message will be sent as-is without any changes.
		@param theEvent: The message to be sent
		@param target: The recipient of the message
	*/
	void sendMessage(SEvent theEvent, string target);

	/** Take any action as a result of reading the message. This will take default action, and ensure that all instance actions are taken as well.
		It will call the instance-defined handleEventH() to ensure that all necessary action is taken.
		@param theEvent: The message to be handled
	*/
	void handleEvent(SEvent theEvent);

	/** Virtual, event-handling function to be instance-defined to enact instance-specific behaviour. This will be called after handleEvent() is in order
		to trigger any instance-specific behaviour on receiving the event, to be triggered after default actions taken in handleEvent().
		@param theEvent: The event to be handled
	*/
	virtual void handleEventH(SEvent theEvent) = 0;

	/** The message queue containing all reified events to be consumed by this instance.*/
	shared_ptr<queue<SEvent>> _eventQueue;

	/** All neighbouring Handlers.*/
	map<string,weak_ptr<queue<SEvent>>> neighbourQueues;

	/** The name of this Handler instance. */
	string name;

	/** STypes not to be forwarded. */
	vector<SType> doNotForward;

	/** Virtual, update function to be instance-defined to enact instance-specific behaviour. 
		This will be called after update() generic behaviour is actioned.
	*/
	virtual void updateH() = 0;

	/** Whether this Handler instance is running. */ 
	bool running;
	
	/** Whether the Handler is in the process of closing*/
	bool closing;

private:

	/** Indicator for whether the name has been set */
	bool nameSet = false;
};

#endif
