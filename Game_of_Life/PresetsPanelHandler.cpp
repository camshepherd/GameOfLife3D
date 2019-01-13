#include "PresetsPanelHandler.h"
using namespace std;


PresetsPanelHandler::PresetsPanelHandler()
{
	name = "PresetsPanelHandler";
}

PresetsPanelHandler::PresetsPanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, FileType fileType, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui) :
		PanelHandler(name, creator, _outputQueue, _window, _gui), fileType(fileType) {
	buildPanel();
}

PresetsPanelHandler::~PresetsPanelHandler()
{
}

void PresetsPanelHandler::buildPanel() {
	if (fileType == FT_UNIVERSE) {
		_thePanel = Panel::create(Layout2d("20%", "20%"));
		_thePanel->setPosition(Layout2d("0%", "30%"));
	}
	else if (fileType == FT_RULES) {
		_thePanel = Panel::create(Layout2d("20%", "20%"));
		_thePanel->setPosition(Layout2d("75%", "30%"));
	}
	_thePanel->loadWidgetsFromFile("PanelLayouts/PresetsPanel.txt");
	_thePanel->getRenderer()->setBackgroundColor(tgui::Color("Red"));
	_gui->add(_thePanel);
	
	setDefaultColor(sf::Color::Cyan);
	applyDefaultColor();
	Widget::Ptr* subjectWidget = &(_thePanel->get("Title"));
	Label::Ptr titleWidget = *((Label::Ptr*)subjectWidget);
	titleWidget->setHorizontalAlignment(Label::HorizontalAlignment::Center);
	if (fileType == FT_UNIVERSE) {
		titleWidget->setText("Universe Presets");
	}
	else if (fileType == FT_RULES) {
		titleWidget->setText("Rules Presets");
	}
	subjectWidget = &(_thePanel->get("theListbox"));
	ListBox::Ptr listBox = *((ListBox::Ptr*)subjectWidget);
	broadcastMessage(SEvent(name, FILE_NAMES_GET2).fileType(fileType));
	//link up the widgets' signals
	_thePanel->get("saveName")->connect("ReturnKeyPressed", &PresetsPanelHandler::handleEditBoxSignal, this);
	_thePanel->get("buttonSave")->connect("Pressed", &PresetsPanelHandler::handleButtonSignal, this);
	_thePanel->get("buttonLoad")->connect("Pressed", &PresetsPanelHandler::handleButtonSignal, this);
}

void PresetsPanelHandler::updatePH() {
};

void PresetsPanelHandler::broadcastUpdates() {
	//TODO: something (if necessary)
}

void PresetsPanelHandler::handleEventPH(SEvent theEvent) {
	Widget::Ptr* subjectWidget = &(_thePanel->get("theListBox"));
	ListBox::Ptr subjectListBox = *((ListBox::Ptr*)subjectWidget);
	if (theEvent._sType == SF_EVENT) {
		if (theEvent._event.key.code == Keyboard::Delete) {
			if (subjectListBox->isFocused()) {
				broadcastMessage(SEvent(name, FILE_DELETE).name(subjectListBox->getSelectedItem()).fileType(fileType));
			}
		}
		else if (theEvent._event.key.code == Keyboard::Return) {
			if (subjectListBox->isFocused()) {
				broadcastMessage(SEvent(name, FILE_OPEN).name(subjectListBox->getSelectedItem()).fileType(fileType));
			}
		}
	}
	else if (theEvent._fileType == fileType && (theEvent._sType == FILE_DELETED || theEvent._sType == FILE_OPENED || theEvent._sType == FILE_WRITTEN2 || theEvent._sType == PRESET_LOADED)) {
		flash(theEvent._operationState);
	}
	else if (theEvent._sType == FILE_NAMES2 && theEvent._fileType == fileType) {
		updateFileNames(&theEvent._vectorString);
	}
};


void PresetsPanelHandler::handleButtonSignal(tgui::Widget::Ptr widget, const std::string& signalName) {
	Widget::Ptr* subjectWidget = &(_thePanel->get("theListBox"));
	ListBox::Ptr subjectListBox = *((ListBox::Ptr*)subjectWidget);
	string widgetName = _thePanel->getWidgetName(widget);
	// save/load get the data from Simulation
	if (widgetName == "buttonLoad") {
		if (subjectListBox->getSelectedItem() != "") {
			broadcastMessage(SEvent(name, FILE_OPEN).name(subjectListBox->getSelectedItem()).fileType(fileType));
		}
	}
	else if (widgetName == "buttonSave") {
		subjectWidget = &(_thePanel->get("saveName"));
		EditBox::Ptr saveName = *((EditBox::Ptr*)subjectWidget);
		string text = saveName->getText();
		//cout << "saveName: " << endl;
		if (name != "") {
			//broadcast partially completed file writing call, to be finished and sent off by whichever Handler deals with it
			broadcastMessage(SEvent(name, FILE_WRITE2).name(text).fileType(fileType));
		}
		else {
			flash(OP_FAIL);
		}
	}
}


void PresetsPanelHandler::updateFileNames(vector<string>* fileNames) {
	Widget::Ptr* subjectWidget = &(_thePanel->get("theListBox"));
	ListBox::Ptr subjectListBox = *((ListBox::Ptr*)subjectWidget);
	vector<String> items = subjectListBox->getItems();
	for (auto iter : *fileNames ) {
		if (!(find(items.begin(), items.end(), String(iter)) != items.end())) {
			subjectListBox->addItem(String(iter));
		}
	}
	for (auto iter : items) {
		if (!(find(fileNames->begin(), fileNames->end(), String(iter)) != fileNames->end())) {
			subjectListBox->removeItem(String(iter));
		}
	}
}


void PresetsPanelHandler::handleEditBoxSignal(tgui::Widget::Ptr editBox, const std::string& signalName, String theText) {
	//ASSUMED THAT THE ONLY CALLBACK WILL BE THE ENTER KEY BEING RETURNED
	if (_thePanel->getWidgetName(editBox) == "saveName") {
		broadcastMessage(SEvent(name, FILE_WRITE2).name(theText).fileType(fileType));
	}
}
