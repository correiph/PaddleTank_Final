#include "MainMenuGameState.h"

#include "AssetManager.h"
#include "InputManager.h"
#include "PlayGameState.h"

#include <SFML/Graphics.hpp>

#include <cassert>

void MainMenuGameState::Init(int width, int height) {
	//Check and ensure we aren't already initialised.
	assert((m_titleText == nullptr) && "MainMenuGameState is already initialised.");
	
	//y is the position for the top of each text box. It gets incremented
	// by the height of each text box plus a little bit.
	float y = height * 0.3f;
	
	//Setup the text boxes and fill them with the appropriate text.
	sf::Font &fmain = *G_FontManager.GetAsset("assets/Orbitron Bold.ttf");
	m_titleText = new sf::Text("Paddle Tanks", fmain, 60U);
	sf::FloatRect rect = m_titleText->getGlobalBounds();
	m_titleText->setOrigin(rect.width * 0.5f, 0.0f);
	m_titleText->setPosition(width * 0.5f, y);

	//Increment y and calculate position of next text box.
	y += rect.height;
	y += height * 0.1f;

	//List of menu items. Create an sf::Text for each of them and place them in 
	// an array so they can be iterated. Each text is set to the correct position.
	//You can add another menu item by adding it to this list.
	std::vector<std::string> items = {
		"Start New Game",
		"Options",
		"Quit"
	};
	for (std::size_t i = 0; i < items.size(); i++) {
		//Setup the text boxes and fill them with the appropriate text.
		m_menuItems.push_back(new sf::Text(items[i], fmain, 30U));
		rect = m_menuItems.back()->getGlobalBounds();
		m_menuItems.back()->setOrigin(rect.width * 0.5f, 0.0f);
		m_menuItems.back()->setPosition(width * 0.5f, y);

		//Increment y and calculate position of next text box.
		y += rect.height;
		y += height * 0.05f;
	}

	//Register keys used by this state.
	G_InputManager.RegisterKey(sf::Keyboard::Escape);
	G_InputManager.RegisterKey(sf::Keyboard::Return);
	G_InputManager.RegisterKey(sf::Keyboard::Space);
	G_InputManager.RegisterKey(sf::Keyboard::Up);
	G_InputManager.RegisterKey(sf::Keyboard::Down);
}



void MainMenuGameState::Cleanup() {
	//Clean up objects.
	if (m_titleText != nullptr) {
		delete m_titleText;
		m_titleText = nullptr;
	}
	
	for (std::size_t i = 0; i < m_menuItems.size(); i++) {
		delete m_menuItems[i];
		m_menuItems[i] = nullptr;
	}
	m_menuItems.clear();

	//reset the select and scroll values.
	m_selectedItem = 0;
	m_selectedItemClicked = false;

	//Deregister keys used by this state.
	G_InputManager.DeregisterKey(sf::Keyboard::Escape);
	G_InputManager.DeregisterKey(sf::Keyboard::Return);
	G_InputManager.DeregisterKey(sf::Keyboard::Space);
	G_InputManager.DeregisterKey(sf::Keyboard::Up);
	G_InputManager.DeregisterKey(sf::Keyboard::Down);
}

void MainMenuGameState::Update(float delta, GameManager *game) {
	//Transition states based on key presses.
	if (G_InputManager.wasKeyReleased(sf::Keyboard::Escape)) {
		game->PopState();
	}
	//Is the user currently clicking on a menu item?
	m_selectedItemClicked = G_InputManager.isKeyDown(sf::Keyboard::Return) || G_InputManager.isKeyDown(sf::Keyboard::Space);
	if (G_InputManager.wasKeyReleased(sf::Keyboard::Return) || G_InputManager.wasKeyReleased(sf::Keyboard::Space)) {
		switch (m_selectedItem) {
		case 0: //Play the game!
			game->PushState(&PlayGameState::Instance());
			break;
		case 2: //Quit
			game->PopState();
			break;
		}
	}
	//Scroll the currently selected item up and down.
	if (G_InputManager.wasKeyReleased(sf::Keyboard::Up) && m_selectedItem > 0) {
		--m_selectedItem;
	}
	if (G_InputManager.wasKeyReleased(sf::Keyboard::Down) && m_selectedItem < m_menuItems.size() - 1) {
		++m_selectedItem;
	}
}

void MainMenuGameState::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	//Set the three colors for the menu options.
	sf::Color unselectedColor = sf::Color::White;
	sf::Color selectedColor = sf::Color::Green;
	sf::Color clickedColor = sf::Color::Red;
	
	//Draw the title text and menu items.
	target.draw(*m_titleText, states);
	for (std::size_t i = 0; i < m_menuItems.size(); i++) {
		//Render with the appropriate color based on selection
		// and clicked status.
		if (i != m_selectedItem) {
			m_menuItems[i]->setColor(unselectedColor);
		}
		else if (!m_selectedItemClicked) {
			m_menuItems[i]->setColor(selectedColor);
		}
		else {
			m_menuItems[i]->setColor(clickedColor);
		}
		target.draw(*m_menuItems[i], states);
	}
}
