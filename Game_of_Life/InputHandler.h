#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include "Handler.h"
using namespace sf;

/** Direct consumer of all user inputs. This class will forward, as necessary, user inputs to the rest of the program to be actioned elsewhere. 
	This will also do some type-mapping between sf::events and more specific SEvents for better behaviour throughout the rest of the program. 

	@author Cameron Shepherd
*/
class InputHandler : public Handler
{

public:

	/** Constructor 1. Default constructor.*/
	InputHandler();
	
	/** Constructor 2. Sets the name, creates neighbour bond between this and creator, and saves the location of the window to consume events from.
		@param name: name of this InputHandler
		@param creator: name of the creating Handler
		@param _outputQueue: communication queue for the creator
		@param _window: the sf::RenderWindow for events to be consumed from*/
	InputHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window);
	
	/** Default destructor. */
	~InputHandler();
	
	/** Set the window from which events should be consumed. 
		@param theWindow: the sf::RenderWindow for events to be consumed from*/
	void setWindow(weak_ptr<sf::RenderWindow> theWindow);
	
	/** Definition of the Handler-specific update function. @see Handler.update(). This will iterate through all sf::Events and handle each of them. 
	*/
	void updateH();

private:

	/** The window for events to be polled from. */
	shared_ptr<sf::RenderWindow> _window;
	
	/** The position of the mouse in _window. */
	Vector2i mousePosition;
	
	/** The movement of the mouse in the given frame. */
	Vector2i mouseMovement;
	
	/** Multiplier for mouse wheel movement.*/
	int zoomSpeed = 7;
	
	/** Take any non-generic action as a result of reading the message. @see Handler.handleEventH().
		@type theEvent: SEvent
		@param theEvent: The message to be handled
	*/
	void handleEventH(SEvent);
	
	/** Whether the program is in a movement state, meaning that inputs are allowed to move the camera
	*/
	bool worldMoveState = false;

	/** How far the mouse has been scrolled
	*/
	int mouseScroll = 0;

	/** Absolute movement for the camera, rather than rotation around a point
	*/
	Vector3i movement;

	/** Movement of the selected cell, for edit mode(3)
	*/
	Vector3i cellMovement;

};

#endif
