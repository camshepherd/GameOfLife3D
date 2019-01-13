#include "Handler.h"

Handler::Handler()
{
	_eventQueue = shared_ptr<queue<SEvent>>(new queue<SEvent>());
	running = true;
	closing = false;
	doNotForward = vector<SType>();

	addDoNotForward(HANDLER_DELETED);
	addDoNotForward(QUEUE_SEND);
	addDoNotForward(WINDOW_CLOSE);
	addDoNotForward(FILE_WRITTEN);
	//default constructor
}

Handler::Handler(string _name) : 
		Handler() {
	name = _name;
	nameSet = true;
}


Handler::Handler(string name,string creator, weak_ptr<queue<SEvent>> _outputQueue) : 
		Handler(name) {
	neighbourQueues[creator] = _outputQueue;
	//create the thing
	sendMessage(SEvent(name, QUEUE_SEND).queue(weak_ptr<queue<SEvent>>(_eventQueue)),creator);
} 


Handler::~Handler()
{
	/*OPERATION:
	stop running
	instruct neighbours to destruct
	delete everything
	*/
	cout << name << " has been deleted" << endl;
}


void Handler::setName(string newName) {
	if (!nameSet) {
		name = newName;
		nameSet = true;
	}
}

void Handler::sendMessage(SEvent theEvent, string target) {
	//send the event only to the target
	if (neighbourQueues.count(target) > 0) {
		// if the target actually exists then send it the message
		neighbourQueues[target].lock()->push(theEvent.target(target));
	}
}


void Handler::broadcastMessage(SEvent theEvent) {
	// Use this function to create a new message and to forward on an existing message
	bool fineToBroadcast = true;
	if (theEvent._sType == QUEUE_SEND) {
		fineToBroadcast = false;
	}
	// make sure that there is no problem with broadcasting the message
	/*for (auto _type : doNotForward) {
		if (theEvent._sender != name && (find(doNotForward.begin(), doNotForward.end(), _type) != doNotForward.end())) {
			fineToBroadcast = false;
		}
	}*/
	if (fineToBroadcast) {
		string originalSender = theEvent._sender;
		SEvent newEvent = theEvent;
		newEvent._sender = name;
		for (auto pair : neighbourQueues) {
			if (pair.first != originalSender) {
				// forward the message, but from self to make sure it doesn't receive the same message back
				pair.second.lock()->push(newEvent);
			}
		}
	}
}


void Handler::setNeighbour(weak_ptr<queue<SEvent>> _queue, string name) {
	neighbourQueues[name] = _queue;
}

void Handler::addDoNotForward(SType sType) {
	// if given sType is not found in doNotForward then add it to the vector
	if (find(doNotForward.begin(), doNotForward.end(), sType) == doNotForward.end()) {
		doNotForward.push_back(sType);
	}
}

void Handler::update() {
	while (!_eventQueue->empty()) {
		SEvent currEvent = _eventQueue->front();
		broadcastMessage(currEvent);
		if (_eventQueue->size() > 0) {
			handleEvent(currEvent);
			_eventQueue->pop();
		}
	}
	updateH();
}

void Handler::handleEvent(SEvent theEvent) {
	if (theEvent._sType == QUEUE_SEND) {
		if (theEvent._target == name) {
			setNeighbour(theEvent._queue, theEvent._sender);
		}
	}
	else if (theEvent._sType == HANDLER_DELETED) {
		try {
			neighbourQueues.erase(theEvent._sender);
		}
		catch (const char& e) {
			cout << "Caught error thing: " << e << endl;
		}
	}
	handleEventH(theEvent);
}
