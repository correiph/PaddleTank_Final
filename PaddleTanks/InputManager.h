#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <map>
#include <set>

//Provide convenient access.
#define G_InputManager (InputManager::Instance())

/// <summary>
/// InputManager provides universal access to the state of the mouse and keyboard.
///  the input manager is not event driven - it must be updated each frame so that
/// it can update the current state of the keys, mouse buttons, and mouse positions.
/// To cut down on overhead only keys that have been registered are updated. However
/// to ensure the input manager is not overly slow you can still query a key even
/// if it hasn't been registered. Doing so will result in undefined behaviour as
/// the value cannot be predicted and will not change between frames until the
/// key is properly registered.
/// To allow the input manager to detect the frame of a key press and key release,
/// the input manager tracks the current state of each registered key AND the 
/// previous state.
/// Ensure that UpdateKeys() and UpdateMouse() are called exactly once per tick of the game.
/// If called more than once the previous values will be wiped out, causing wasKeyPressed()
/// and wasKeyReleased() to no longer work.
/// </summary>
/// <author>
/// Kevin Forest 2015
/// </author>
class InputManager {
public:
	static InputManager &Instance() {
		static InputManager instance;
		return instance;
	}
private:
	//Enforce singleton-ness
	InputManager() { };
	//Do not implement!
	InputManager(InputManager const &);
	InputManager operator=(InputManager const &);

public:

	void RegisterKey(sf::Keyboard::Key);
	void DeregisterKey(sf::Keyboard::Key);
	void UpdateKeys();
	void UpdateMouse(sf::Window &window);

	bool isKeyDown(sf::Keyboard::Key);
	bool isKeyUp(sf::Keyboard::Key);
	bool wasKeyPressed(sf::Keyboard::Key);
	bool wasKeyReleased(sf::Keyboard::Key);

	bool isMouseButtonDown(sf::Mouse::Button);
	bool isMouseButtonUp(sf::Mouse::Button);
	bool wasMouseButtonPressed(sf::Mouse::Button);
	bool wasMouseButtonReleased(sf::Mouse::Button);

	sf::Vector2i mousePosition() { return m_currMousePos; }
	sf::Vector2i mouseDelta() { return m_currMousePos - m_prevMousePos; }
	
private:

	std::set<sf::Keyboard::Key> m_registeredKeys;
	std::map<sf::Keyboard::Key, bool> m_currKeyValues;
	std::map<sf::Keyboard::Key, bool> m_prevKeyValues;

	std::map<sf::Mouse::Button, bool> m_currMouseButtonValues;
	std::map<sf::Mouse::Button, bool> m_prevMouseButtonValues;

	sf::Vector2i m_prevMousePos;
	sf::Vector2i m_currMousePos;
};


