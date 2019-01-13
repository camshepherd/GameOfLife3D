#include "SimulationHandler.h"


SimulationHandler::SimulationHandler()
{
	setToDefault();
	broadcastMessage(SEvent(name, RULES_NEW).rules(weak_ptr<Rule>(_theSimulation->getRules()))); 
}

SimulationHandler::SimulationHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue) :
	Handler(name, creator, _outputQueue) {
	setToDefault();
}

SimulationHandler::~SimulationHandler()
{
}

string SimulationHandler::getSimulationString() {
	return _theSimulation->toString();
}

void SimulationHandler::handleEventH(SEvent theEvent) {
	if (theEvent._sType == FRAME_LIMIT) {
		targetFrame = theEvent._time;
	}
	else if (theEvent._sType == FRAME_GET) {
		if (_theSimulation->framesSimulated() > theEvent._time) {
			broadcastMessage(SEvent(name, FRAME).time(theEvent._sType).frame(_theSimulation->getFrame(theEvent._time)));
		}
		else if (theEvent._time > targetFrame) {
			targetFrame = theEvent._time;
		}
	}
	else if (theEvent._sType == RULES_NEW) {
		changeRule(theEvent._rules);
	}
	else if (theEvent._sType == RULES_CHANGE) {
		changeRule(theEvent._rules);
	}
	else if (theEvent._sType == SIMULATION_NEW) {
		//TODO simulation set to new universe layout
		//broadcast universe changed
	}
	else if (theEvent._sType == FILE_WRITE) {			
		// finish off the FILE_WRITE message to be handled elsewhere
		if (theEvent._fileType == FT_RULES) {
			broadcastMessage(theEvent.sender(name).str1(shared_ptr<Rule>(_theSimulation->getRules())->toString()));
		}
		else if (theEvent._fileType == FT_UNIVERSE) {
			broadcastMessage(theEvent.sender(name).str1(_theSimulation->toString(0)));
		}
	}
	else if (theEvent._sType == FILE_OPENED) {
		if (theEvent._fileType == FT_RULES) {
			try {
				if (_theSimulation->changeRule(shared_ptr<Rule>(new Rule(theEvent._str1)))) {
					broadcastMessage(SEvent(name, RULES_NEW).rules(_theSimulation->getRules()));
				}
				else {
					broadcastMessage(SEvent(name, RULES_NEW).rules(_theSimulation->getRules()));
				}
			}
			catch (exception e) {
				cout << "Caught exception" << e.what() << " here" << endl;
				getchar();
			}
		}
		else if (theEvent._fileType == FT_UNIVERSE) {
			//set up a new simulation, replacing it if it has worked
			try {
				if (_theSimulation->fromString(theEvent._str1)) {
					broadcastMessage(SEvent(name, PRESET_LOADED).operationState(OP_SUCCESS).fileType(FT_UNIVERSE));
				}
				else {
					broadcastMessage(SEvent(name, PRESET_LOADED).operationState(OP_FAIL).fileType(FT_UNIVERSE));
				}
			}
			catch(exception e) {
				cout << "Something went fairly wrong: " << e.what() << endl;
				broadcastMessage(SEvent(name, PRESET_LOADED).operationState(OP_FAIL).fileType(FT_UNIVERSE));
			}
		}
	}
	else if (theEvent._sType == WORLD_RESIZE) {
		_theSimulation = unique_ptr<Simulation>(new Simulation(theEvent._intVal));
		targetFrame = 50;
		name = "SimulationHandler";
	}
}

int SimulationHandler::getNumFrames() {
	return _theSimulation->getUniverseSize();
}

void SimulationHandler::updateH() {
	if (closing && running) {
		running = false;
	}
	else if (getNumFrames() < targetFrame) {
		_theSimulation->step();
	}
}

weak_ptr<Rule> SimulationHandler::getRules() {
	return _theSimulation->getRules();
}

void SimulationHandler::setToDefault() {
	_theSimulation = unique_ptr<Simulation>(new Simulation());
	targetFrame = 50;
	name = "SimulationHandler";
	broadcastMessage(SEvent(name, RULES_NEW).rules(_theSimulation->getRules()));
}

void SimulationHandler::changeRule(weak_ptr<Rule> newRules) {
	_theSimulation->changeRule(newRules);
	targetFrame = 50;
	broadcastMessage(SEvent(name, RULES_CHANGED).rules(_theSimulation->getRules()));
	broadcastMessage(SEvent(name, FRAME_LIMIT).time(_theSimulation->framesSimulated()));
	broadcastMessage(SEvent(name, FRAME).time(0).frame(_theSimulation->getFrame(0)));
}

void SimulationHandler::changeStartFrame(stringstream* input_thing) {};

void SimulationHandler::changeStartFrame(weak_ptr<vector<vector<vector<int>>>> startFrame) {};

void SimulationHandler::changeStartFrame(string thingy) {};

void SimulationHandler::run() {
	while (running) {
		update();
	}
	delete this;
}
