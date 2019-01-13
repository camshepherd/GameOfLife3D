#pragma once
#include "PanelHandler.h"

/** PanelHandler for both Rules and Universe presets panels. 
	@author Cameron Shepherd
*/

class PresetsPanelHandler : public PanelHandler{
public:

	/** Constructor 1. Default constructor.*/
	PresetsPanelHandler();
	
	/** Constructor 2. Store the _window and _gui, create the panel and create communication channel between this and creator.
		@param name: Name of the instance
		@param creator: Name of the creator
		@param _outputQueue: Communication queue for the creator
		@param _window: The window the _gui operates in
		@param _gui: The gui the panel operates in
	*/
	PresetsPanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, FileType fileType, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui);
	
	/** Default destructor*/
	~PresetsPanelHandler();
	
	/** Build the panel. Load the panel from the relevant layout file and link the signals.*/
	void buildPanel();
	
	/** Instance-specific update functionality. This update functionality will occur after the generic update functionality, @see PanelHandler.updateH. */
	void updatePH();

private:

	/** Broadcast all pending updates. This will send out all new and relevant changes to the presets panel.*/
	void broadcastUpdates();
	
	/** Instance-specific event handling functionality. These actions will occur after the generic event-handling functionality, @see PanelHandler.handleEventH.
		@param theEvent: The event to be handled.
	*/
	void handleEventPH(SEvent);

	/** The type of file that this presets panel handler deals with*/
	FileType fileType;
	
	/** IHandling of button signals.
		@param widget: the widget sending the signal
		@param signalName: the name of the signal
	*/
	void handleButtonSignal(tgui::Widget::Ptr widget, const std::string& signalName);

	/** Update the displayed file names in the ListBox to match the given set
		@param fileNames: The file names that should be displayed in the ListBox
	*/
	void updateFileNames(vector<string>* fileNames);

	/** Handling of EditBox signals.
		@param editbox: the widget sending the signal
		@param signalName: the name of the signal
		@param theText: the contents of the editbox sent with the signal
	*/
	void handleEditBoxSignal(tgui::Widget::Ptr editBox, const std::string& signalName, String theText);

};
