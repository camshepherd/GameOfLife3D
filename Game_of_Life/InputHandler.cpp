#include "InputHandler.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
using namespace sf;
using namespace std;

InputHandler::InputHandler() : Handler("Input"){}

InputHandler::InputHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue, weak_ptr<sf::RenderWindow> arg_window) :
	Handler(name, creator, _outputQueue), _window(arg_window){
	neighbourQueues[creator] = _outputQueue;
}

InputHandler::~InputHandler()
{
}


void InputHandler::updateH() {
	movement = Vector3i(0,0,0);
	cellMovement = Vector3i(0, 0, 0);
	Event theEvent;
	while (_window->pollEvent(theEvent))
	{
		broadcastMessage(SEvent(name, SF_EVENT).event(theEvent));
		if (theEvent.type == Event::Closed)
		{
			broadcastMessage(SEvent(name, WINDOW_CLOSE).target("IntrepidLemming"));
		}
		else if (theEvent.type == Event::Resized) {
			broadcastMessage(SEvent(name, WINDOW_RESIZE).vector2iVal(Vector2i(theEvent.size.height, theEvent.size.width)));
		}
		else if (theEvent.type == Event::MouseWheelMoved) {
			broadcastMessage(SEvent(name, WORLD_ZOOM).floatVal(theEvent.mouseWheel.delta * zoomSpeed));
		}
		else if (theEvent.type == Event::KeyPressed) {
			if (theEvent.key.code == Keyboard::Escape) {
				broadcastMessage(SEvent(name, STATE_CHANGE).name("1"));
			}
			else if (theEvent.key.code == Keyboard::F9) {
				broadcastMessage(SEvent(name, STATE_CHANGE).name("3"));
			}
			else if (theEvent.key.code == Keyboard::F1) {
				broadcastMessage(SEvent(name, PANEL_TOGGLE).name("Controller"));
			}
			else if (theEvent.key.code == Keyboard::F2) {
				broadcastMessage(SEvent(name, PANEL_TOGGLE).name("Rules"));
			}
			else if (theEvent.key.code == Keyboard::F3) {
				broadcastMessage(SEvent(name, PANEL_TOGGLE).name("PresetsRules"));
			} 
			else if (theEvent.key.code == Keyboard::F4) {
				broadcastMessage(SEvent(name, PANEL_TOGGLE).name("PresetsUniverse"));
			}
			else if (theEvent.key.code == Keyboard::F9) {
				worldMoveState = !worldMoveState;
				if (worldMoveState) {
					mousePosition = sf::Vector2i(_window->getSize().x / 2, _window->getSize().y / 2);
					Mouse::setPosition(_window->mapCoordsToPixel(sf::Vector2f(mousePosition)));
					_window->setMouseCursorVisible(false);
				}
				else {
					_window->setMouseCursorVisible(true);
				}
			}
			else if (theEvent.key.code == Keyboard::F10) {
				broadcastMessage(SEvent(name, WORLD_RESET));
			}
			else if (theEvent.key.code == Keyboard::LShift || Mouse::isButtonPressed(Mouse::Button::Middle)) {
				worldMoveState = true;
				if (worldMoveState) {
					mousePosition = sf::Vector2i(_window->getSize().x / 2, _window->getSize().y / 2);
					Mouse::setPosition(_window->mapCoordsToPixel(sf::Vector2f(mousePosition)));
					_window->setMouseCursorVisible(false);
				}
			}
		}
		else if (theEvent.type == Event::KeyReleased) {
			if (theEvent.key.code == Keyboard::LShift) {
				worldMoveState = false;
				_window->setMouseCursorVisible(true);
			}
		}
	}
	if (Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			movement.y += 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			//cout << "Right" << endl;
			movement.y -= 1;
		}
	}
	if (Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			movement.z -= 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			movement.z += 1;
		}
	}
	else {
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			movement.x += 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			movement.x -= 1;
		}


		if (Keyboard::isKeyPressed(Keyboard::W)) {
			cellMovement.y += 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			cellMovement.y -= 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			cellMovement.x += 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			cellMovement.x -= 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Z)) {
			cellMovement.z += 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::X)) {
			cellMovement.z -= 1;
		}
		broadcastMessage(SEvent(name, CELL_SELECT_MOVEMENT).vector3iVal(cellMovement));
	}
	if (Mouse::isButtonPressed(Mouse::Button::Middle) || worldMoveState) {
		_window->setMouseCursorVisible(false);
	}
	else {
		_window->setMouseCursorVisible(true);
	}
	if (worldMoveState || Keyboard::isKeyPressed(sf::Keyboard::LShift) || Mouse::isButtonPressed(Mouse::Button::Middle)) {
		mouseMovement = mousePosition - Mouse::getPosition();
		broadcastMessage(SEvent(name, WORLD_ROTATE).vector2iVal(mouseMovement));
		broadcastMessage(SEvent(name, WORLD_ZOOM).intVal(mouseScroll));
		broadcastMessage(SEvent(name, WORLD_MOVE).vector3iVal(movement));
		mousePosition = sf::Vector2i(_window->mapPixelToCoords(sf::Vector2i(_window->getDefaultView().getCenter() + Vector2f(0.5,0.5))));
		Mouse::setPosition(_window->mapCoordsToPixel(sf::Vector2f(mousePosition)));
	}
}


void InputHandler::setWindow(weak_ptr<sf::RenderWindow> arg_window) {
	_window = shared_ptr<RenderWindow>(arg_window);
}

void InputHandler::handleEventH(SEvent theEvent) {
}
