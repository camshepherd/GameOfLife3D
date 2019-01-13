#pragma once
#include "Handler.h"
#include "TGUI/TGUI.hpp"
using namespace tgui;

/** Generic class for encapsulating TGUI panels. This class offers generic functions for use by all panels, and offers standard functions to be 
	defined in instances of PanelHandler for standardised handling method of events and signals.

	@author Cameron Shepherd
*/
class PanelHandler : public Handler{

public:

	/** Constructor 1. Default constructor */
	PanelHandler();
	
	/** Constructor 2. Generate the generic panel with its gui and window to draw to, and immediately sets up itself and its creator
		as being neighbours.*/
	PanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui);
	
	/** Default destructor. */
	~PanelHandler();
	
	/** Virtual function to build the panel. This function will build the panel and initialise default settings regards it*/
	virtual void buildPanel() = 0;
	
	/** Set the gui. Store the pointer to the TGUI::Gui for the panel for use by all future draw() and update() functions.*/ 
	void setGui(Gui*);
	
	/** Set the window. Store the pointer to the sfml::RenderWindow for the panel for use by all future draw() and update() functions.*/
	void setWindow(RenderWindow*);
	
	/** Set whether the panel is drawn or not. True for the window being active and viewable. False for the window being inactive and unviewable.*/
	void setState(bool);
	
	/** Draw the panel. This will draw the panel to the stored_window. Does not update the panel.*/
	void drawPanel();
protected:
	
	/** Broadcast all changes from the panel. This function will broadcast to all neighbours all the events yet to be actioned from the panel.*/
	virtual void broadcastUpdates();
	
	/** Handle the signal from the TGUI::EditBox. This will take any necessary action that needs to occur as a result of the callback on the panel's 
		TGUI::EditBox. This function does not need to be defined in any child class in the event that the panel does not contain any EditBoxes.
		@param editBox: The signal sender
		@param signalName: The name of the signal
		@param sentText: The text sent in the callback
	*/
	virtual void handleEditBoxSignal(tgui::Widget::Ptr editBox, const std::string& signalName, String sentText);
	
	/** Handle the signal from the TGUI::Slider. This will take any necessary action that needs to occur as a result of the callback on the panel's
	TGUI::EditBox. This function does not need to be defined in any child class in the event that the panel does not contain any Sliders.
	@param slider: The signal sender
	@param signalName: the name of the signal
	*/
	virtual void handleSliderSignal(tgui::Widget::Ptr slider, const std::string& signalName);
	
	/** Handle the signal from the TGUI::Button. This will take any necessary action that needs to occur as a result of the callback on the panel's
		TGUI::EditBox. This function does not need to be defined in any child class in the event that the panel does not contain any Buttons.
		@param slider: The signal sender
		@param signalName: the name of the signal
	*/
	virtual void handleButtonSignal(tgui::Widget::Ptr button, const std::string& signalName);
	
	/** Handle the signal from the TGUI::ListBox. This will take any necessary action that needs to occur as a result of the callback on the panel's
		TGUI::EditBox. This function does not need to be defined in any child class in the event that the panel does not contain any ListBoxes.
		@param slider: The signal sender
		@param signalName: the name of the signal
	*/
	virtual void handleListBoxSignal(tgui::Widget::Ptr listBox, const std::string& signalName);
	
	/** Definition of the Handler-specific update function @see Handler.update(). This calls the generic update for this PanelHandler, and allows any 
		panel-specific functionality to be performed in updatePH() following the generic behaviour. 
		@param slider: The signal sender
		@param signalName: the name of the signal
	*/
	void updateH();
	
	/** Virtual, update function to be instance-defined to enact instance-specific behaviour.
	This will be called after updateH() generic behaviour is actioned.
	*/
	virtual void updatePH() = 0;
	
	/** Take any action as a result of reading the message. This will take default action, and ensure that all instance actions are taken as well.
		It will call the instance-defined handleEventPH() to ensure that all necessary action is taken.
		@param theEvent: The message to be handled
	*/
	void handleEventH(SEvent theEvent);
	
	/** Virtual, event-handling function to be instance-defined to enact instance-specific behaviour. This will be called after handleEventH() is in order
		to trigger any instance-specific behaviour on receiving the event, to be triggered after default actions taken in handleEventH().
		@param theEvent: The event to be handled
	*/
	virtual void handleEventPH(SEvent theEvent) = 0;
	
	/** The gui the panel operates in. */
	shared_ptr<Gui> _gui;
	
	/** The window the gui operates in. */
	shared_ptr<RenderWindow> _window;
	
	/** The panel that the PanelHandler instance encapsulates. */
	shared_ptr<Panel> _thePanel;
	
	/** State of the panel. True if the panel is active and viewable. False if the panel is inactive and unviewable. */
	bool state;
	
	/** Clock to time update times. This clock should only be used by the update() function as the panel needs to be updated not just with sf::Events but with
		time elapsed between updates. */
	sf::Clock clock;
	
	/** Clock for the timings of flashes for success or failure in the PanelHandler.
	*/
	sf::Clock flashClock;
	
	/** Whether the panel is flashing or not
	*/
	bool flashing;
	
	/** Flash the panel to indicate the result of an operation
		@param successfulness: the result to indicate to the user.
		*/
	void flash(OperationState successfulness);
	
	/** Default colour for the panel background.
	*/
	sf::Color defaultColor;
	
	/** Set the default background colour for the panel
		@param newDefault: the new default colour for the panel background
	*/
	void PanelHandler::setDefaultColor(sf::Color newDefault);

	/** Set the colour of the panel to be its default.
	*/
	void PanelHandler::applyDefaultColor();
};
