#include "InputManager.h"

void InputManager::RegisterKey(sf::Keyboard::Key key) {
	//Insert the key into the set of registered keys, then put it into the 
	// current and previous key maps. If the key is already registered
	// nothing gets changed.
	m_registeredKeys.insert(key);
	m_currKeyValues[key];
	m_prevKeyValues[key];
}

void InputManager::DeregisterKey(sf::Keyboard::Key key) {
	//Check if the key is in the set, if it is deleted it from the
	// set and the maps.
	auto it = m_registeredKeys.find(key);
	if (it != m_registeredKeys.end()) {
		m_currKeyValues.erase(key);
		m_prevKeyValues.erase(key);
		m_registeredKeys.erase(it);
	}
}

void InputManager::UpdateKeys() {
	//Current becomes previous.
	m_prevKeyValues.swap(m_currKeyValues);
	//Get new current values.
	for (std::set<sf::Keyboard::Key>::iterator it = m_registeredKeys.begin(); it != m_registeredKeys.end(); it++) {
		m_currKeyValues[*it] = sf::Keyboard::isKeyPressed(*it);
	}
}

void InputManager::UpdateMouse(sf::Window &window) {
	m_prevMousePos = m_currMousePos;
	m_prevMouseButtonValues = m_currMouseButtonValues;
	//Get the current mouse position relative to the window.
	m_currMousePos = sf::Mouse::getPosition(window);
	m_currMouseButtonValues[sf::Mouse::Left] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	m_currMouseButtonValues[sf::Mouse::Right] = sf::Mouse::isButtonPressed(sf::Mouse::Right);
}

bool InputManager::isKeyDown(sf::Keyboard::Key key) {
	//Note that if the key is not currently registered this will put it into the map with a default value.
	// Since the key is not registered it won't be updated.
	return m_currKeyValues[key];
}

bool InputManager::isKeyUp(sf::Keyboard::Key key) {
	//Note that if the key is not currently registered this will put it into the map with a default value.
	// Since the key is not registered it won't be updated.
	return !m_currKeyValues[key];
}

bool InputManager::wasKeyPressed(sf::Keyboard::Key key) {
	//Note that if the key is not currently registered this will put it into the map with a default value.
	// Since the key is not registered it won't be updated.
	return !m_prevKeyValues[key] && m_currKeyValues[key];
}

bool InputManager::wasKeyReleased(sf::Keyboard::Key key) {
	//Note that if the key is not currently registered this will put it into the map with a default value.
	// Since the key is not registered it won't be updated.
	return m_prevKeyValues[key] && !m_currKeyValues[key];
}

bool InputManager::isMouseButtonDown(sf::Mouse::Button b) {
	return m_currMouseButtonValues[b];
}

bool InputManager::isMouseButtonUp(sf::Mouse::Button b) {
	return !m_currMouseButtonValues[b];
}

bool InputManager::wasMouseButtonPressed(sf::Mouse::Button b) {
	return !m_prevMouseButtonValues[b] && m_currMouseButtonValues[b];
}

bool InputManager::wasMouseButtonReleased(sf::Mouse::Button b) {
	return m_prevMouseButtonValues[b] && !m_currMouseButtonValues[b];
}

