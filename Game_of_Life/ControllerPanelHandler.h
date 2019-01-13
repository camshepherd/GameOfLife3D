#pragma once

#include "PanelHandler.h"
#include <sstream>
#include <iomanip>
/** PanelHandler for the controller panel.
	@author Cameron Shepherd
*/
class ControllerPanelHandler : public PanelHandler
{
public:
	/** Constructor 1. Default constructor. */
	ControllerPanelHandler();
	/** Constructor 2. Set the name, set up the Panel and set up event passing connection between this and creator.
		@param name: name of this instance
		@param creator: name of the creator
		@param _outputQueue: communciation queue for the creator
	*/
	ControllerPanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui);
	/** Default destructor. */
	~ControllerPanelHandler();
	/** Build the panel. Read the panel in from the preset file, link the TGUI::Widget callbacks to their handlers and set up the default settings.*/
	void buildPanel();
	/** Instance-specific update function. This function will be performed after the generic update functionality @see PanelHandler.updateH().*/
	void updatePH();
	/** Instance-specific TGUI::EditBox handler. This will perform any functionality as necessary as a result of the signal.*/
	void handleEditBoxSignal(tgui::Widget::Ptr editBox, const std::string& signalName, String sentText);
	/** Instance-specific TGUI::EditBox handler. This will perform any functionality as necessary as a result of the signal.*/
	void handleSliderSignal(tgui::Widget::Ptr slider, const std::string& signalName);
	/** Instance-specific TGUI::EditBox handler. This will perform any functionality as necessary as a result of the signal.*/
	void handleButtonSignal(tgui::Widget::Ptr button, const std::string& signalName);
	//* Increase the speed of playback. This will increase the real-time velocity of playback, including from 0.*/
	void increaseSpeed();
	//* Decrease the speed of playback. This will increase the real-time velocity of playback, including from 0.*/
	void decreaseSpeed();
private:
	/** Broadcast any changes that any other Handlers would need. The instance-specific broadcasting of all necessary updates from this specific panel.
	@see PanelHandler.broadcastUpdates.*/
	void broadcastUpdates();
	/** Instance-specific event handling. This event handling will be performed after all generic event handling @see PanelHandler.handleEventH.
		@param theEvent: The message to be handled.
	*/
	void handleEventPH(SEvent theEvent);
	/** Clock for the controller. Used to ensure that real-time actions are able to be performed.*/
	Clock timer;
	/** Start frame of current playback.*/
	int startFrame;
	/** Current frame to be displayed.*/
	int frame;
	/** Current minimum frame for the slider.*/
	int min;
	/** Current maximum frame for the slider. */
	int max;
	/** Whether playback is active. True: playing. False: stopped.*/
	bool playing;
	/** The real-time speed of playback. Units: frames per second.*/
	float speed;
};

