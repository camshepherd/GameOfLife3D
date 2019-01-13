#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H

#include "RulePanelHandler.h"
#include "ControllerPanelHandler.h"
#include "PresetsPanelHandler.h"
#include "MenuPanelHandler.h"

/** Class to handle the running of the GUI. This does not handle the representation of the universe, only the user interaction with the panels governing 
	the rules of it. 
	
	@author Cameron Shepherd
*/
class GUIHandler : public Handler
{

public:

	/** Constructor 1. Default constructor.*/
	GUIHandler();

	/** Constructor 2. Create the GUIHandler with its name.
		@param name: name of the instance
	*/
	GUIHandler(string name);

	/** Constructor 3. Set the name, set up the PanelHandlers and set up event passing connection between this and creator.
		@param name: name of this GUIHandler
		@param creator: name of the creator
		@param _outputQueue: communciation queue for the creator
	*/
	GUIHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> theWindow);
	
	/** Default destructor. Destroy the GUI and delete self. */
	~GUIHandler();
	
	/** Definition of the Handler-specific update function @see Handler.update(). This performs the update functionality for this class.*/
	void updateH();
	
	/** Draw the gui to the window. Call the PanelHandlers' drawPanel() functions sequentially. */
	void draw();

protected:
	
	/** Definition of the Handler-specific update function @see Handler.handleEvent(). This performs the event handling functionality for this class.*/
	void handleEventH(SEvent theEvent);
	
	/** All PanelHandlers part of this gui. */
	unique_ptr<map<string, unique_ptr<PanelHandler>>> _panelHandlers;
	
	/** The gui being managed. */
	shared_ptr<Gui> _gui;
	
	/** The window the _gui is being drawn to. */
	shared_ptr<sf::RenderWindow> _window;
	
	/** State of the GUI. State 1: Main screen. State 2: Options menu.*/
	int state;
	
	/** Change state of the gui.
		@param state: State for the GUI to switch to.
	*/
	void changeState(int state);
	
	/** States of the PanelHandlers.*/
	map<string, bool> panelStates;
	
	/** Set this instance to default state.*/
	void setToDefault();
};

#endif