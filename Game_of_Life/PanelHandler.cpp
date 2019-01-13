#include "PanelHandler.h"

PanelHandler::PanelHandler() : Handler()
{
	
}

PanelHandler::PanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui) :
	Handler(name, creator, _outputQueue),_gui(_gui),_window(_window){
	state = true;
	clock.restart();
	setName(name);
	flashing = false;
}

void PanelHandler::setWindow(RenderWindow* window) {
	_window = shared_ptr<RenderWindow>(window);
}

PanelHandler::~PanelHandler()
{
}

void PanelHandler::setGui(Gui* arg_gui) {
	_gui = shared_ptr<tgui::Gui>(arg_gui);
}

void PanelHandler::setState(bool arg_state) {
	if (!arg_state) {
		_thePanel->hide();
		_thePanel->disable();
		_thePanel->getRenderer()->setOpacity(0);
	}
	else {
		_thePanel->show();
		_thePanel->enable();
		_thePanel->getRenderer()->setOpacity(100);
	}
}

void PanelHandler::drawPanel() {
	//_thePanel->draw(*_window, sf::RenderStates::Default);
}

void PanelHandler::handleEditBoxSignal(tgui::Widget::Ptr editBox, const std::string& signalName, String sentText) {
	cout << " Generic handleEditBoxSignal by " << name << endl;
}
void PanelHandler::handleSliderSignal(tgui::Widget::Ptr slider, const std::string& signalName) {
	cout << " Generic handleSliderSignal by " << name << endl;
}
void PanelHandler::handleButtonSignal(tgui::Widget::Ptr button, const std::string& signalName) {
	cout << " Generic handleButtonSignal by " << name << endl;
}
void PanelHandler::handleListBoxSignal(tgui::Widget::Ptr listBox, const std::string& signalName) {
	cout << " Generic handleListBoxSignal by " << name << endl;
}
void PanelHandler::broadcastUpdates() {
	cout << "Generic broadcastUpdate called from " << name << endl;
}

void PanelHandler::updateH() {
	if (closing && running) {
		running = false;
	}
	else if(running){
		for (auto widget : _thePanel->getWidgets()) {
			widget->update(clock.getElapsedTime());
		}
		clock.restart();
		broadcastUpdates();
		if (flashing) {
			if (int(flashClock.getElapsedTime().asMilliseconds()) > 2000) {
				applyDefaultColor();
			}
		}
		updatePH();
	}
}

void PanelHandler::handleEventH(SEvent theEvent) {
	handleEventPH(theEvent);
}

void PanelHandler::flash(OperationState successfulness) {
	flashing = true;
	flashClock.restart();
	if (successfulness == OP_SUCCESS) {
		_thePanel->getRenderer()->setBackgroundColor(sf::Color::Green);
	}
	else if (successfulness == OP_FAIL) {
		_thePanel->getRenderer()->setBackgroundColor(sf::Color::Red);
	}
}

void PanelHandler::setDefaultColor(sf::Color theColor) {
	defaultColor = sf::Color(theColor);
}

void PanelHandler::applyDefaultColor() {
	_thePanel->getRenderer()->setBackgroundColor(sf::Color::Cyan);
	flashing = false;
}
