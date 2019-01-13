#pragma once
#include "PanelHandler.h"
#include <iostream>
using namespace tgui;
/** PanelHandler to govern the rules panel
	@author Cameron Shepherd
*/
class RulePanelHandler : public PanelHandler{
public:
	
	/** Constructor 1. Default constructor.*/
	RulePanelHandler();
	
	/** Constructor 2. Store _window and _gui, create the panel and open communication panel between this and the creator.
		@param name: Name of the instance
		@param creator: Name of the creator
		@param _outputQueue: Communication queue for the creator
		@param _window: The window the _gui operates in
		@param _gui: The gui the panel operates in
	*/
	RulePanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui);
	
	/** Default destructor.*/
	~RulePanelHandler();
	
	/** Build the panel. Load the panel from the relevant layout file and link the signals.*/
	void buildPanel();
	
	/** Instance-specific update behaviour. This functionality will occur after the generic update behaviour, @see panelHandler.updateH().*/
	void updatePH();
	
	/** Handle the signal from an EditBox. Take relevant action as a result of the callback.
		@param editBox: The subject EditBox
		@param signalName: The name of the sent signal
		@param theText: The sent text
	*/
	void handleEditBoxSignal(tgui::Widget::Ptr editBox, const std::string& signalName, String theText);

private:
	
	/** Broadcast all pending updates. Broadcast to all neighbours the new updates.*/
	void broadcastUpdates();
	
	/** Instance-specific event handling behaviour. This functionality will occur after the generic event-handling behaviour, @see PanelHandler.handleEventH().
		@param theEvent: The event to be handled.
	*/
	void handleEventPH(SEvent theEvent);
	
	/** Active rules for the panel.*/
	shared_ptr<Rule> _theRules;
	
	/** Whether anything has changed since the last update. True: changed. False: unchanged.*/
	bool changed;

	/** Whether the dimensions of the universe changed since the last update. True: changed. False: unchanged.*/
	bool dimensionsChanged;

	/** Length of an edge in the universe in cell count
	*/
	int simDimension;
};
