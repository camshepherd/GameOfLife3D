#include "ControllerPanelHandler.h"
using namespace std;


ControllerPanelHandler::ControllerPanelHandler() : PanelHandler()
{
	//default constructor 
}

ControllerPanelHandler::ControllerPanelHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> _window, weak_ptr<tgui::Gui> _gui) :
		PanelHandler(name, creator, _outputQueue, _window, _gui) {
	//standard constructor
	buildPanel();
}

ControllerPanelHandler::~ControllerPanelHandler()
{
}

void ControllerPanelHandler::buildPanel() {
	// build the panel
	min = 0;
	max = 50;
	frame = 0;
	speed = 1;
	playing = false;
	startFrame = 0;

	_thePanel = Panel::create(Layout2d("100%", "20%"));
	_thePanel->setPosition(Layout2d("0%", "80%"));
	_gui->add(_thePanel);
	_thePanel->getRenderer()->setBackgroundColor(tgui::Color("Blue"));
	_thePanel->loadWidgetsFromFile("PanelLayouts/Controller.txt");
	
	//connect slider signals
	_thePanel->get("ControllerSlideBar")->connect("ValueChanged", &ControllerPanelHandler::handleSliderSignal, this);

	_thePanel->get("ControllerFrameLow")->connect("ReturnKeyPressed", &ControllerPanelHandler::handleEditBoxSignal, this);
	_thePanel->get("ControllerFrameHigh")->connect("ReturnKeyPressed", &ControllerPanelHandler::handleEditBoxSignal, this);
	_thePanel->get("ControllerFrame")->connect("ReturnKeyPressed", &ControllerPanelHandler::handleEditBoxSignal, this);
	_thePanel->get("ControllerSpeedBox")->connect("ReturnKeyPressed", &ControllerPanelHandler::handleEditBoxSignal, this);

	_thePanel->get("ControllerPlay")->connect("Pressed", &ControllerPanelHandler::handleButtonSignal, this);
	_thePanel->get("ControllerFastForward")->connect("Pressed", &ControllerPanelHandler::handleButtonSignal, this);
	_thePanel->get("ControllerSlowDown")->connect("Pressed", &ControllerPanelHandler::handleButtonSignal, this);
}

void ControllerPanelHandler::updatePH() {
	if (playing && frame <= max && frame >= min) {
		//ensure simulation is played correctly
		if (speed >= 0) {
			frame = signed int(startFrame + (speed * timer.getElapsedTime().asSeconds()) + 0.5);
		}
		else {
			frame = signed int(startFrame + (speed * timer.getElapsedTime().asSeconds()) - 0.5);
		}
	}
	else if (frame >= max) {
		frame = max;
		playing = false;
	}
	else if (frame <= min) {
		frame = min;
		playing = false;
	}
	//yes this type casting is horrific but there doesn't seem to be a better way to do it
	//update slider value
	Widget::Ptr* subjectWidget = &(_thePanel->get("ControllerSlideBar"));
	Slider::Ptr subjectSlider = *((Slider::Ptr*)subjectWidget);
	if (!subjectSlider->isFocused()) {
		subjectSlider->setValue(frame);
		subjectSlider->setMinimum(min);
		subjectSlider->setMaximum(max);
	}
	//update minimum frame output
	subjectWidget = &(_thePanel->get("ControllerFrameLow"));
	EditBox::Ptr subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		subjectEditBox->setText(to_string(min));
	}
	
	//update maximum frame output
	subjectWidget = &(_thePanel->get("ControllerFrameHigh"));
	subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		subjectEditBox->setText(to_string(max));
	}
	//update current frame output
	subjectWidget = &(_thePanel->get("ControllerFrame"));
	subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		subjectEditBox->setText(to_string(frame));
	}
	//update current speed
	subjectWidget = &(_thePanel->get("ControllerSpeedBox"));
	subjectEditBox = *((EditBox::Ptr*)subjectWidget);
	if (!subjectEditBox->isFocused()) {
		stringstream stream;
		stream << fixed << setprecision(2) << speed;
		string stringSpeed = stream.str();
		subjectEditBox->setText(stringSpeed);
		subjectEditBox->setInputValidator(EditBox::Validator::Float);
	}
	broadcastUpdates();
};

void ControllerPanelHandler::handleEventPH(SEvent theEvent) {
	//Frame limit has changed somewhere so assumedly the simulation was being viewed is no longer
	if (theEvent._sType == FRAME_LIMIT) {
		max = theEvent._time;
		frame = 0;
	}
}

void ControllerPanelHandler::handleEditBoxSignal(tgui::Widget::Ptr editBox, const std::string& signalName, String theText = "") {
	//assumed that only returnkeypressed will call this function, could change in the future.
	int value = std::stoi(theText.toAnsiString());
	if (_thePanel->getWidgetName(editBox) == "ControllerFrame") {
		if (value > max) {
			frame = max;
		}
		else if (value < min) {
			frame = min;
		}
		else {
			frame = value;
		}
	}
	if (_thePanel->getWidgetName(editBox) == "ControllerFrameLow") {
		if (value >= 0 && value <= max) {
			min = value;
		}
		else if (value >= 0 && value > max) {
			max = value;
			min = value;
			frame = value;
		}		
	}
	if (_thePanel->getWidgetName(editBox) == "ControllerFrameHigh") {
		if (value >= 0 && value >= min) {
			max = value;
		}
		else if (value >= 0 && value < min) {
			min = value;
			max = value;
			frame = value;
		}
	}
	if (_thePanel->getWidgetName(editBox) == "ControllerSpeedBox") {
		speed = std::stof(theText.toAnsiString());
		startFrame = frame;
		timer.restart();
	}
	EditBox::Ptr subjectEditBox = *((EditBox::Ptr*)&editBox);
	subjectEditBox->unfocus();
}

void ControllerPanelHandler::handleSliderSignal(tgui::Widget::Ptr widget, const std::string& signalName) {
	if (signalName == "ValueChanged") {
		Widget::Ptr* subjectWidget = &(_thePanel->get("ControllerSlideBar"));
		Slider::Ptr subjectSlider = *((Slider::Ptr*)subjectWidget);
		frame = subjectSlider->getValue();
	}
}

void ControllerPanelHandler::handleButtonSignal(tgui::Widget::Ptr widget, const std::string& signalName) {
	if (_thePanel->getWidgetName(widget) == "ControllerPlay") {
		if (playing) {
			playing = false;
		}
		else {
			if (speed > 0 || frame > min) {
				playing = true;
				timer.restart();
				startFrame = frame;
			}
		}
	}
	if (_thePanel->getWidgetName(widget) == "ControllerFastForward") {
		speed = speed + 2;
		timer.restart();
		startFrame = frame;
	}
	if (_thePanel->getWidgetName(widget) == "ControllerSlowDown") {
		speed = speed - 2;
		timer.restart();
		startFrame = frame;
	}
}

void ControllerPanelHandler::increaseSpeed() {
	speed = speed * 2;
}

void ControllerPanelHandler::decreaseSpeed() {
	speed = speed / 2;
}

void ControllerPanelHandler::broadcastUpdates() {
	broadcastMessage(SEvent(name, FRAME_GET).time(frame));
	broadcastMessage(SEvent(name, FRAME_LIMIT).time(max));
}
