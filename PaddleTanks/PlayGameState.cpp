#include "PlayGameState.h"
#include "Map.h"
#include "InputManager.h"
#include <SFML/Graphics.hpp>

#include <iostream>
PlayGameState::~PlayGameState()
{
}

void PlayGameState::Init(int width, int height) {
	m_map = new Map();
	if (!m_map->loadFromFile("assets/Level1.xml")) {
		std::cout << "Error loading map Level1.xml" << std::endl;
		delete m_map;
		m_map = nullptr;
		return;
	}
	G_InputManager.RegisterKey(sf::Keyboard::Escape);
}

void PlayGameState::Cleanup() {
	delete m_map;
	m_map = nullptr;
	G_InputManager.DeregisterKey(sf::Keyboard::Escape);
	
}

void PlayGameState::Update(float delta, GameManager *game) {
	m_map->Update(delta);
	if (G_InputManager.wasKeyReleased(sf::Keyboard::Escape)) {
		//I'd like to add a pause menu at some point.
		game->PopState();
	}
}

void PlayGameState::draw(sf::RenderTarget &target, sf::RenderStates states) const  {
	target.draw(*m_map, states);
}

