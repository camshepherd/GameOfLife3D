#include "MenuPanelHandler.h"

MenuPanelHandler::MenuPanelHandler()
{
	setName("MenuPanelHandler");
}

MenuPanelHandler::MenuPanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui) :
		PanelHandler(name, creator, _outputQueue, _window, _gui) {
	buildPanel();
}

MenuPanelHandler::~MenuPanelHandler()
{
}

void MenuPanelHandler::buildPanel() {
	_thePanel = Panel::create(Layout2d("100%", "100%"));
	_thePanel->setPosition(Layout2d("0%", "0%"));
	_gui->add(_thePanel);
	_thePanel->getRenderer()->setBackgroundColor(tgui::Color::Yellow);
	_thePanel->loadWidgetsFromFile("PanelLayouts/Menu.txt");

	auto thing = _thePanel->get("MenuButtonExit");
	_thePanel->get("MenuButtonBack")->connect("Pressed", &MenuPanelHandler::handleButtonSignal, this);
	_thePanel->get("MenuButtonClose")->connect("Pressed", &MenuPanelHandler::handleButtonSignal, this);
}

void MenuPanelHandler::updatePH() {
};

void MenuPanelHandler::broadcastUpdates() {
	//TODO: broadcasting menu data
}

void MenuPanelHandler::handleEventPH(SEvent theEvent) {
};

void MenuPanelHandler::handleButtonSignal(tgui::Widget::Ptr button, const std::string& signalName) {
	if (_thePanel->getWidgetName(button) == "MenuButtonBack") {
		broadcastMessage(SEvent(name, STATE_CHANGE).name("1"));
	}
	else if (_thePanel->getWidgetName(button) == "MenuButtonClose") {
		broadcastMessage(SEvent(name, WINDOW_CLOSE).target("IntrepidLemming"));
	}
}
