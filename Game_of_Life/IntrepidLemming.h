#ifndef INTREPIDLEMMING_H
#define INTREPIDLEMMING_H
#include "Handler.h"
#include "RenderHandler.h"
#include "InputHandler.h"
#include "GUIHandler.h"
#include "FileHandler.h"
#include "SimulationHandler.h"
#include <map>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
/**
	The central call and routing point of the program. IntrepidLemming sets up the program, updates components running in its thread and is responsible for closing
	it down correctly.

	@author Cameron Shepherd.
*/
class IntrepidLemming : public Handler
{
public:
	/** Constructor 1. Default Constructor*/
	IntrepidLemming();
	
	/** Default destructor.*/
	~IntrepidLemming();
	
	/** Whether the IntrepidLemming is running currently
	*/
	bool isRunning();
	
	/** Sets the IntrepidLemming instance running
	*/
	void run();

private:

	/** Update step in the main loop. This will draw as necessary and also update all other Handlers operating in the same thread.*/
	void updateH();
	
	/** Draw the screen. This will draw the entire program to _window, but will not update any part of it. */
	void draw();
	
	/** Set the frame rate for the program. This sets the maximum refresh rate for the program. 
		@param frameRate: maximum frame rate for the program.*/
	void setFrameRate(int frameRate);
	
	/** The frame clock. This is utilised to measure time between updates to determine whether the screen needs to be refreshed.*/
	Clock frameClock;
	
	/** All manually updated Handlers. Each of these Handlers is updated each update cycle, and are drawn to the screen if necessary. These Handlers are unable to run in
		their own threads.*/
	unique_ptr<map<string, unique_ptr<Handler>>> _handler;
	
	/** Take any non-generic action as a result of reading the message. @see Handler.handleEventH().
		@param theEvent: The message to be handled
	*/
	void handleEventH(SEvent theEvent);
	
	/** The window the program is running in.*/
	shared_ptr<sf::RenderWindow> _window;
	
	/** Maximum frame rate for the program. */
	float frameRate;
	
	/** Close all detached threads. Make the program ready to be closed.*/
	void finishThreads();
	
	/** Number of concurrent threads supported on the particular system.
	*/
	unsigned int concurrentThreadsSupported;

	/** Gets any OpenGL messages and sends them to standard output. For testing purposes.
		@param numMsgs: Number of messages from the queue to collect and output.
	*/
	void GetFirstNMessages(GLuint numMsgs);
};

#endif
