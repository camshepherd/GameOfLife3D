#include "GUIHandler.h"

GUIHandler::GUIHandler(): Handler() {
	setToDefault();
}

GUIHandler::GUIHandler(string name) : Handler(name){
	setToDefault();
}

GUIHandler::GUIHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> newWindow) : Handler(name, creator, _outputQueue){
	setName("GUI");
	state = 1;
	_panelHandlers = unique_ptr<map<string, unique_ptr<PanelHandler>>>(new map<string, unique_ptr<PanelHandler>>());
	_window = shared_ptr<RenderWindow>(newWindow);
	_gui = shared_ptr<tgui::Gui>(new Gui(*_window));
	
	_panelHandlers->emplace("Rules", new RulePanelHandler("Rules", name, _eventQueue, newWindow, _gui));
	_panelHandlers->emplace("Controller", new ControllerPanelHandler("Controller", name, _eventQueue, newWindow, _gui));
	// Universe presets panel is not finished so has been removed
	//_panelHandlers->emplace("PresetsUniverse", new PresetsPanelHandler("PresetsUniverse", name, _eventQueue, FT_UNIVERSE, newWindow, _gui));
	_panelHandlers->emplace("PresetsRules", new PresetsPanelHandler("PresetsRules", name, _eventQueue, FT_RULES, newWindow, _gui));
	_panelHandlers->emplace("Menu", new MenuPanelHandler("Menu", name, _eventQueue, newWindow, _gui));
	
	panelStates = map<string, bool>();
	panelStates.emplace("Rules", true);
	panelStates.emplace("Controller", true);
	// Universe presets panel is not finished so has been removed
	//panelStates.emplace("PresetsUniverse", true);
	panelStates.emplace("PresetsRules", true);
	panelStates.emplace("Menu", true);
	changeState(1);
}

GUIHandler::~GUIHandler()
{
	_gui->removeAllWidgets();
}

void GUIHandler::changeState(int new_state) {
	if (new_state == 1) {
		state = 1;
		for (auto handler = _panelHandlers->begin(); handler != _panelHandlers->end(); handler++) {
			if (panelStates[handler->first]) {
				handler->second->setState(true);
			}
		}
		_panelHandlers->at("Menu")->setState(false);
	}
	else if (new_state == 2) {
		state = 2;
		for (auto handler = _panelHandlers->begin(); handler != _panelHandlers->end(); handler++) {
			if (panelStates[handler->first]) {
				handler->second->setState(false);
			}
		}
		_panelHandlers->at("Menu")->setState(true);
	}
	else if (new_state == 3) {
		state = 3;
		for (auto handler = _panelHandlers->begin(); handler != _panelHandlers->end(); handler++) {
			if (panelStates[handler->first]) {
				handler->second->setState(false);
			}
		}
	}
}

void GUIHandler::updateH() {
	for (auto handler = _panelHandlers->begin(); handler != _panelHandlers->end(); handler++) {
		if (panelStates[handler->first]) {
			handler->second->update();
		}
	}
}

void GUIHandler::draw() {
	_gui->draw();
}

void GUIHandler::handleEventH(SEvent theEvent) {
	if (theEvent._sType == SF_EVENT) {
		_gui->handleEvent(theEvent._event);
	}
	else if (theEvent._sType == PANEL_TOGGLE) {
		if (panelStates.count(theEvent._name) == 1) {
			if (panelStates.at(theEvent._name)) {
				panelStates.at(theEvent._name) = false;
			}
			else {
				panelStates.at(theEvent._name) = true;
			}
			_panelHandlers->at(theEvent._name)->setState(panelStates.at(theEvent._name));
		}
	}
	else if (theEvent._sType == STATE_CHANGE) {
		// feasibly in the future other types of state change could occur, hence possibility of different strings to differentiate type
		// of state change

		if (theEvent._name == "1") {
			if (state == 1) {
				changeState(2);
			}
			else {
				changeState(1);
			}
		}
	}
	else if (theEvent._sType == HANDLER_DELETED) {
		try {
			_panelHandlers->erase(theEvent._sender);
		}
		catch (const char& error) {
			cout << "Error things caught: " << error << endl;
		}
		if (_panelHandlers->size() == 0) {
			running = false;
		}
	}
}

void GUIHandler::setToDefault() {
	_panelHandlers = unique_ptr<map<string, unique_ptr<PanelHandler>>>(new map<string,unique_ptr<PanelHandler>>());
	state = 1;
	//do not want it to be forwarding QUEUE_SEND calls
	addDoNotForward(QUEUE_SEND);
	addDoNotForward(WINDOW_CLOSE);
}
