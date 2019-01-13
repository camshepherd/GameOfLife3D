#include "IntrepidLemming.h"


IntrepidLemming::IntrepidLemming() : 
		Handler("IntrepidLemming")
{
	//addDoNotForward(SF_EVENT);
	// default constructor
	// create the whole program and start it running
	_handler = unique_ptr<map<string, unique_ptr<Handler>>>(new map<string,unique_ptr<Handler>>());


	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;


	_window = shared_ptr<sf::RenderWindow>(new sf::RenderWindow(
		sf::VideoMode::getDesktopMode(), 
		"Intrepid Lemming", 
		//sf::Style::Titlebar | sf::Style::Close,
		sf::Style::Fullscreen,
		settings));

	//_window->setMouseCursorGrabbed(true);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "FAILED TO INIT CORRECTLY" << endl;
	}

	cout << "MAJOR: " << _window->getSettings().majorVersion << endl << "MINOR: " << _window->getSettings().minorVersion << endl;
	sf::Image icon;
	icon.loadFromFile("Resources/gecko2.png");
	_window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	setFrameRate(10);
	_handler->emplace("Simulation", unique_ptr<Handler>(new SimulationHandler("Simulation", name, _eventQueue)));
	_handler->emplace("Input",unique_ptr<Handler>(new InputHandler("Input", name, _eventQueue, _window)));
	_handler->emplace("GUI", unique_ptr<Handler>(new GUIHandler("GUI", name, _eventQueue, _window)));
	_handler->emplace("Render", unique_ptr<Handler>(new RenderHandler("Render", name, _eventQueue, _window)));
	_handler->emplace("Files", unique_ptr<Handler>(new FileHandler("Files", name, _eventQueue)));
	frameClock.restart();
	glEnable(GL_DEBUG_OUTPUT);
	//may return 0 when not able to detect
	concurrentThreadsSupported = std::thread::hardware_concurrency();
	if (concurrentThreadsSupported == 0) {
		concurrentThreadsSupported = 2;
	}
}

void IntrepidLemming::setFrameRate(int theFrameRate) {
	// set the screen refresh rate for the program
	frameRate = theFrameRate;
	//_window->setFramerateLimit(theFrameRate);
}

IntrepidLemming::~IntrepidLemming()
{
	finishThreads();
	_window->close();
}

bool IntrepidLemming::isRunning() {
	// simply return the running flag's value
	return running;
}

void IntrepidLemming::updateH() {
	// update each component
	// update the directly handled handlers
	for (auto pair = _handler->begin(); pair != _handler->end(); pair++) {
		pair->second->update();
	}
	// update every possible frame but only draw the screen at the given frame rate
	if (frameClock.getElapsedTime().asMicroseconds() >= 100000 / frameRate) {
		draw();
		frameClock.restart();
	}
}

void IntrepidLemming::draw() {
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	_window->setActive(true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_window->clear();
	// draw the screen, the gui and the renderer
	((RenderHandler*)(_handler->at("Render")).get())->draw();
	_window->pushGLStates();
	//_window->resetGLStates();
	((GUIHandler*)(_handler->at("GUI")).get())->draw();
	_window->popGLStates();
	_window->display();
	_window->setActive(false);
	// check OpenGL error
	GetFirstNMessages(5);
}


void IntrepidLemming::GetFirstNMessages(GLuint numMsgs)
{
	GLint maxMsgLen = 300;
	//glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);

	std::vector<GLchar> msgData(numMsgs * maxMsgLen * 2);
	std::vector<GLenum> sources(numMsgs * 2);
	std::vector<GLenum> types(numMsgs * 2);
	std::vector<GLenum> severities(numMsgs * 2);
	std::vector<GLuint> ids(numMsgs * 2);
	std::vector<GLsizei> lengths(numMsgs * 2);
	GLuint numFound = glGetDebugMessageLog(1,300, &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);
	//cout << numFound << " message were found" << endl;
	sources.resize(numFound);
	types.resize(numFound);
	severities.resize(numFound);
	ids.resize(numFound);
	lengths.resize(numFound);

	std::vector<std::string> messages;
	messages.reserve(numFound);

	std::vector<GLchar>::iterator currPos = msgData.begin();
	for (size_t msg = 0; msg < lengths.size(); ++msg)
	{
		messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
		currPos = currPos + lengths[msg];
	}
	for (auto message: messages) {
		cout << "OpenGL message: " << message << endl;
	}
}


void IntrepidLemming::handleEventH(SEvent theEvent) {
	if (theEvent._sType == WINDOW_CLOSE) {
		running = false;
		//delete this;
	}
	else if (theEvent._sType == HANDLER_DELETED) {
		try {
			_handler->erase(theEvent._sender);
		}
		catch (const char& error) {
			cout << "Caught error thing: " << error << endl;
		}
		if (_handler->size() == 0) {
			running = false;
		}
	}
}


void IntrepidLemming::run() {
	while (running) {
		update();
	}
	delete this;
}


void IntrepidLemming::finishThreads() {
	//TODO set about ending the threads or something, if there are multiple threads
}
