#pragma once
#include "PanelHandler.h"
/** PanelHandler for the Menu panel.
	@author Cameron Shepherd
*/
class MenuPanelHandler : public PanelHandler
{

public:

	/** Constructor 1. Default constructor.*/
	MenuPanelHandler();
	
	/** Constructor 2. Store the _window and _gui pointers, create the panel and set up communication channel between this and the creator.*/
	MenuPanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui);
	
	/** Default destructor.*/
	~MenuPanelHandler();
	
	/** Build the panel. Read the panel in from the preset file, link the TGUI::Widget callbacks to their handlers and set to the default settings.*/
	void buildPanel();
	
	/** Instance-specific update method. This functionality is called after generic update behaviour, @see PanelHandler.updateH().*/
	void updatePH();

private:

	/** Broadcast all relevent updates. This function will only broadcast changes from the last update. This broadcasts to all neighbours.*/ 
	void broadcastUpdates();
	
	/** Instance-specific event handling. This event handling behaviour will occur after the generic event-handling behaviour, @see PanelHandler.handleEventH.*/
	void handleEventPH(SEvent);
	
	/** Instance-specific button callback handling.
		@param button: the button that sent the signal
		@param signalName: the name of the signal
	*/
	void handleButtonSignal(tgui::Widget::Ptr button, const std::string& signalName);
};
