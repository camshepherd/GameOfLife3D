#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "SFML/Window.hpp"
#include "Rule.h"
#include <queue>


using namespace sf;
using namespace std;

/** Message type. Sent with the SEvent as a header to indicate the sent data and the behaviour that needs to be taken as a result of receiving it.*/
enum SType { STATE_CHANGE, PANEL_TOGGLE,
	WINDOW_CLOSE, WINDOW_RESIZE, SF_EVENT, 
	WORLD_MOVE, WORLD_ZOOM, WORLD_ROTATE, WORLD_RESET, WORLD_RESIZE,
	FRAME, FRAME_GET, FRAME_LIMIT, 
	RULES,  RULES_NEW, RULES_GET,
	RULES_CHANGE, RULES_CHANGED, 
	FILE_WRITE, FILE_WRITTEN, 
	FILE_WRITE2, FILE_WRITTEN2, FILE_NAMES_GET2, FILE_NAMES2,
	FILE_OPEN, FILE_OPENED, FILE_FINISHED,
	FILE_DELETE, FILE_DELETED,
	FILE_NAMES, FILE_NAMES_GET, 
	SIMULATION_NEW,
	QUEUE_SEND,
	HANDLER_DELETED,
	PRESET_LOADED,
	CELL_SELECT_MOVEMENT,
};

/** Type of the subject. Either Rules or Universe, to offer standard referencing of file access and generic Presets Handler.*/
enum FileType {FT_UNIVERSE, FT_RULES};

/** Successfulness of the operation. Used to offer standardised response to a request.*/
enum OperationState { OP_SUCCESS, OP_FAIL};

/** A basic, scalable event passing system for communication between Handlers in the program. 
	@author Cameron Shepherd
*/
struct SEvent {
	
	/** Constructor 1. Stores the given data. Arbitrary numbers of additional variable calls may be added to this main constructor.
		@param sender: The name of the sender
		@param sType: The type of the event
	*/
	SEvent(string sender, SType sType) : _sender(sender), _sType(sType) {}

	// Optional arguments
	// ('this' is returned for chaining)
	/** Define a sender for the SEvent.
		@param sender: the name of the sender
	*/
	SEvent& sender(string sender) { _sender = sender; return *this; }

	/** Add a generic string to the SEvent.
		@param str1: the string to be added.
	*/
	SEvent& str1(const string& str1) { _str1 = str1; return *this; }

	/** Include a fileType variable for the SEvent.
		@param fileType: fileType to be included.
	*/
	SEvent& fileType(FileType fileType) { _fileType = fileType; return *this; }

	/** Include a time/time period in the SEvent.
		@param time: the time to be included.
	*/
	SEvent& time(int time) { _time = time; return *this; }

	/** Include a generic integer value.
		@param intVal: the integer value to be included.
	*/
	SEvent& intVal(int intVal) { _intVal = intVal; return *this; }

	/** Include a generic float value.
		@param floatVal: the value to be included.
	*/
	SEvent& floatVal(float floatVal) { _floatVal = floatVal; return *this; }

	/** Include a Rules object.
		@param rules: pointer to the rules to be included.
	*/
	SEvent& rules(weak_ptr<Rule> rules) { _rules = rules; return *this; }

	/** Include a stringstream object.
		@param stream: the stringstream to be included.
	*/
	SEvent& stream(stringstream* stream) { _stream = stream; return *this; }

	/** Include a frame with the SEvent.
		@param frame: the frame to be included.
	*/
	SEvent& frame(weak_ptr<vector<vector<vector<int>>>> frame) { _frame = frame; return *this; }

	/** Include an operation state.
		@param operationState: The operation result to be included.
	*/
	SEvent& operationState(OperationState operationState) {_operationState = operationState; return *this;}

	/** Include an SFML event.
		@param event: the event to be included
	*/
	SEvent& event(sf::Event event) { _event = event; return *this; }

	/** Include a size 2 integer vector.
		@param vectoriVal: The vector to be included.
	*/
	SEvent& vector2iVal(Vector2i vectoriVal) { _vectoriVal = vectoriVal; return *this; }

	/** Include a size 3 integer vector.
		@param The vector to be included.
	*/
	SEvent& vector3iVal(Vector3i vector3iVal) { _vector3iVal = vector3iVal; return *this; }
	
	/** Include a specific target Handler for the SEvent.
		@param target: the name of the target Handler.
	*/
	SEvent& target(string target) { _target = target; return *this; }

	/** Include an event queue.
		@param theQueue: The event queue to be included.
	*/
	SEvent& queue(weak_ptr<queue<SEvent>> theQueue) { _queue = theQueue; return *this; }

	/** Include a generic string of vectors with the SEvent.
		@param vectorString: the string of vectors to be included.
	*/
	SEvent& vectorString(vector<string> vectorString) { _vectorString = vectorString; return *this; }

	SEvent& name(string name) { _name = name; return *this; }

	/**Type of the SEvent. Required.*/
	SType _sType;
	/** Name of the sender. Required.*/
	string _sender;
	
	//Optional
	
	/**Generic string. Optional.*/
	string _str1;
	
	/** Frame-based time. Optional.*/
	int _time = 0;
	
	/** Generic integer val. Optional.*/
	int _intVal = 0;
	
	/** User event. Optional.*/
	sf::Event _event = sf::Event();
	
	/** Single frame address. Optional.*/
	weak_ptr<vector<vector<vector<int>>>> _frame;
	
	/** Rules for governing the Simulation. Optional.*/
	weak_ptr<Rule> _rules;
	
	/** Pointer to a string's stream. Optional.*/
	stringstream* _stream;
	
	/** Subject's file type. Optional.*/
	FileType _fileType = FT_UNIVERSE;
	
	/** Generic 2-dimensional integer vector. Optional.*/
	Vector2i _vectoriVal;
	
	/** Generic 3-dimensional integer vector. Optional.*/
	Vector3i _vector3iVal;
	
	/** Generic floating point number. Optional.*/
	float _floatVal;
	
	/** Successfulness of the operation. Optional.*/
	OperationState _operationState = OP_SUCCESS;
	
	/** Communication queue. Optional.*/
	weak_ptr<std::queue<SEvent>> _queue;
	
	/** Successfulness of the operation. Optional.*/
	bool _success = true; 
	
	/** Pointer to a vector of strings. Optional.*/
	vector<string> _vectorString;

	/** The target for this SEvent. Optional.*/
	string _target;

	string _name;
};
