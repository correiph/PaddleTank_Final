#include "GameManager.h"
#include "GameState.h"

#include "../TextureAtlas.h"
#include "../AssetManager.h"
#include "../InputManager.h"
#include "../Messaging/MessageDispatcher.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

void GameManager::Init(std::string const &title, int width, int height,
						 unsigned int bpp, bool fullscreen)
{
	//Create a video mode and check if that video mode is legal (if full screen is requested).
	sf::VideoMode vmode(width, height, bpp);
	sf::Uint32 style = sf::Style::Default;
	if (fullscreen) {
		//If the selected mode is not valid we will fail to create a window.
		// to ensure this doesn't happen, we will search the list of valid
		// video modes for full screen and pick the one that matches our request
		// the closest. The rules are: match width, then match height (doesn't currently care about bpp).
		if (!vmode.isValid()) {
			std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
			vmode = modes[0];
			for (std::size_t i = 0; i < modes.size(); i++) {
				sf::VideoMode &m = modes[i];
				if (m.width == width) {
					if (m.height == height) {
						vmode = modes[i];
						break;
					}
					else {
						//Take it but keep looking
						vmode = modes[i];
					}
				}
			}
		}
		style = sf::Style::Fullscreen;
	}
	//Create the window using the specified video mode and style.
	m_window = new sf::RenderWindow(vmode, title, style);
	//Set the flags.
	m_fullscreen = fullscreen;
	m_running = true;

	//Turn off repeated events when a key is held down (pressing and holding a key will generate a single event, rather than
	// a continuous stream of events as long as the key is held down).
	m_window->setKeyRepeatEnabled(false);
	//Turn on V-sync to prevent out of controlled/unthrottled drawing.
	m_window->setVerticalSyncEnabled(true);

	//Load some globally used assets.
	if (!G_AtlasManager.LoadAsset("assets/sheet_tanks.xml") ||
		!G_FontManager.LoadAsset("assets/Orbitron Bold.ttf") ||
		!G_FontManager.LoadAsset("assets/DroidSansMono.ttf")) 
	{
		std::cout << "Error loading one or more assets." << std::endl;
	}

	std::cout << "CGameEngine Init" << std::endl;
}

void GameManager::Cleanup()
{
	// cleanup any remaining states
	while (!m_states.empty()) {
		m_states.back()->Cleanup();
		m_states.pop_back();
	}

	// switch back to windowed mode so other 
	// programs won't get accidentally resized
	if ( m_fullscreen ) {
		m_window->create(sf::VideoMode(), "", sf::Style::Default);
	}
	//Delete the window.
	delete m_window;
	m_window = nullptr;

	std::cout << "CGameEngine Cleanup\n" << std::endl;
		
}

void GameManager::ChangeState(GameState *state)
{
	// cleanup the current state
	if (!m_states.empty()) {
		m_states.back()->Cleanup();
		m_states.pop_back();
	}

	// store and init the new state
	m_states.push_back(state);
	m_states.back()->Init(m_window->getSize().x, m_window->getSize().y);
}

void GameManager::PushState(GameState *state)
{
	// pause current state
	if (!m_states.empty()) {
		m_states.back()->Pause();
	}

	// store and init the new state
	m_states.push_back(state);
	m_states.back()->Init(m_window->getSize().x, m_window->getSize().y);
}

void GameManager::PopState()
{
	// cleanup the current state
	if (!m_states.empty()) {
		m_states.back()->Cleanup();
		m_states.pop_back();
	}

	// resume previous state
	if (!m_states.empty()) {
		m_states.back()->Resume();
	}
}


void GameManager::Run()
{
	//Create a gameclock and set the times.
	sf::Clock gameClock;
	float delta = 1.0f / 60.0f; //60 frames per second.
	float gameTime = 0.0f;
	float currentTime = 0.0f;
	//Main gameplay loop.
	while (m_running && !m_states.empty()) {
		//Poll for window events. Needs to be called
		//regardless of whether or not you process the
		//events.
		// Note that your game should rely on
		//  sf::Keyboard and sf::Mouse classes for 
		//  keypress data.
		sf::Event ev;
		while (m_window->pollEvent(ev)) {
			//Respond to the close event.
			if (ev.type == sf::Event::Closed) {
				m_running = false;
			}
		}

		//TODO: Add some frameskip to get more consistant draw here.
		while ((currentTime = gameClock.getElapsedTime().asSeconds()) > gameTime) {
			gameTime += delta;
			G_InputManager.UpdateKeys();
			G_InputManager.UpdateMouse(*m_window);
			Dispatcher->DispatchDelayedMessages(delta);
			//Update the world.
			m_states.back()->Update(delta, this);
		}

		//Clear the screen and render.
		m_window->clear();
		//TODO: Change to a loop that starts at the topmost opaque state
		// (starting at the top of the stack) then draws it and all states
		// above it in the stack.
		if (!m_states.empty()) {
			m_window->draw(*m_states.back());
		}
		m_window->display();
	}
	return;
}
