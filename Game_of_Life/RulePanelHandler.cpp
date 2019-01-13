#include "RulePanelHandler.h"
using namespace std;
RulePanelHandler::~RulePanelHandler()
{

}

RulePanelHandler::RulePanelHandler() {
	name = "RulePanelHandler";
	cout << "made new rulepanelhandler" << endl;
};

RulePanelHandler::RulePanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui) :
		PanelHandler(name, creator, _outputQueue, _window, _gui) {
	buildPanel();
}

void RulePanelHandler::buildPanel() {
	changed = false;
	_thePanel = Panel::create(Layout2d("20%", "20%"));
	_thePanel->setPosition(Layout2d("0%", "5%"));
	_gui->add(_thePanel);
	simDimension = 9;
	_theRules = shared_ptr<Rule>(new Rule());
	_thePanel->getRenderer()->setBackgroundColor(tgui::Color("Green"));
	_thePanel->loadWidgetsFromFile("PanelLayouts/Rules.txt");

	_thePanel->get("RulesFl")->connect("ReturnKeyPressed", &RulePanelHandler::handleEditBoxSignal, this);

	_thePanel->get("RulesFu")->connect("ReturnKeyPressed", &RulePanelHandler::handleEditBoxSignal, this);

	_thePanel->get("RulesEl")->connect("ReturnKeyPressed", &RulePanelHandler::handleEditBoxSignal, this);

	_thePanel->get("RulesEu")->connect("ReturnKeyPressed", &RulePanelHandler::handleEditBoxSignal, this);

	_thePanel->get("Size")->connect("ReturnKeyPressed", &RulePanelHandler::handleEditBoxSignal, this);
}

void RulePanelHandler::updatePH() {
	changed = false;
	dimensionsChanged = false;
	//deal withFl
	Widget::Ptr* subjectWidget = &(_thePanel->get("RulesFl"));
	EditBox::Ptr subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		subjectEditBox->setText(to_string(_theRules->Fl));
	}

	//deal with Fu
	subjectWidget = &(_thePanel->get("RulesFu"));
	subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		subjectEditBox->setText(to_string(_theRules->Fu));
	}

	//deal with El
	subjectWidget = &(_thePanel->get("RulesEl"));
	subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		subjectEditBox->setText(to_string(_theRules->El));
	}

	//deal with Eu
	subjectWidget = &(_thePanel->get("RulesEu"));
	subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		subjectEditBox->setText(to_string(_theRules->Eu));
	}

	//deal with Size
	subjectWidget = &(_thePanel->get("Size"));
	subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		subjectEditBox->setText(to_string(simDimension));
	}
	broadcastUpdates();
};


void RulePanelHandler::handleEventPH(SEvent theEvent) {
	if (theEvent._sType == RULES_NEW){
		try {
			shared_ptr<Rule> ruleThing = shared_ptr<Rule>(theEvent._rules);
			_theRules = ruleThing;
		}
		catch (const char* e) {
			cout << "Caught exception: " << e << endl;
		}
	}
};

void RulePanelHandler::handleEditBoxSignal(tgui::Widget::Ptr widget, const std::string& signalName, String theText) {
	//	WARNING:*******ASSUMING THAT THE ONLY CALLBACK COULD BE RETURN KEY PRESSED*******
	if (theText == "") {
		theText = "0";
	}
	int value = std::stoi(theText.toAnsiString());

	// make sure that the value is actually new
	if (to_string(_thePanel->getWidgetName(widget)).substr(0,4) == "Rule" && value != _theRules->getRule(_thePanel->getWidgetName(widget).substr(5, 2))) {
		if (value < 0) {
			value = 0;
		}
		else if (value > 26) {
			value = 26;
		}

		if (_thePanel->getWidgetName(widget) == "RulesEl") {
			if (value >= 0) {
				_theRules->changeRule("El", value);
				changed = true;
			}
		}
		else if (_thePanel->getWidgetName(widget) == "RulesEu") {
			if (value >= 0) {
				_theRules->changeRule("Eu", value);
				changed = true;
			}
		}
		else if (_thePanel->getWidgetName(widget) == "RulesFl") {
			if (value >= 0) {
				_theRules->changeRule("Fl", value);
				changed = true;
			}
		}
		else if (_thePanel->getWidgetName(widget) == "RulesFu") {
			if (value >= 0) {
				_theRules->changeRule("Fu", value);
				changed = true;
			}
		}
	}
	else if (_thePanel->getWidgetName(widget) == "Size") {
		dimensionsChanged = true;
		simDimension = value;
	}
	EditBox::Ptr subjectEditBox = *((EditBox::Ptr*)&widget);
	subjectEditBox->unfocus();
	if (to_string(_thePanel->getWidgetName(widget)).substr(4) == "Rule") {
		subjectEditBox->setText(to_string(_theRules->getRule(_thePanel->getWidgetName(widget).substr(5, 2))));
	}
}

void RulePanelHandler::broadcastUpdates() {
	if (changed) {
		broadcastMessage(SEvent(name, RULES_NEW).rules(_theRules));
		changed = false;
	}
	if (dimensionsChanged) {
		broadcastMessage(SEvent(name, WORLD_RESIZE).intVal(simDimension));
	}
}
