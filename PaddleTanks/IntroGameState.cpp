#include "IntroGameState.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "TextureAtlas.h"
#include "TiledBackground.h"

#include "MainMenuGameState.h"

#include <SFML/Graphics.hpp>

#include <cassert>

void IntroGameState::Init(int width, int height) {
	//Check and ensure we aren't already initialised.
	assert((m_titleText == nullptr) && "IntroGameState is already initialised.");
	
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
	y += m_titleText->getGlobalBounds().height;
	y += height * 0.1f;

	//Setup the text boxes and fill them with the appropriate text.
	m_pressStartText = new sf::Text("Press Enter to Play", fmain, 30U);
	rect = m_pressStartText->getGlobalBounds();
	m_pressStartText->setOrigin(rect.width * 0.5f, 0.0f);
	m_pressStartText->setPosition(width * 0.5f, y);
	
	//Increment y and calculate position of next text box.
	y += m_pressStartText->getGlobalBounds().height;
	y += height * 0.05f;

	//Setup the text boxes and fill them with the appropriate text.
	m_pressEscapeText = new sf::Text("Press Escape to Quit", fmain, 30U);
	rect = m_pressEscapeText->getGlobalBounds();
	m_pressEscapeText->setOrigin(rect.width * 0.5f, 0.0f);
	m_pressEscapeText->setPosition(width * 0.5f, y);
	
	//Create the fancy tiled background.
	G_AtlasManager.LoadAsset("assets/sheet_tanks.xml");
	sf::Sprite const * const sprite = G_AtlasManager.GetAsset("assets/sheet_tanks.xml")->GetSprite("dirt.png");
	m_backgroundArray = new TiledBackground(*sprite->getTexture(), sf::IntRect(0, 0, width, height), sprite->getTextureRect());
	
	//Register the keys that are needed by this state.
	G_InputManager.RegisterKey(sf::Keyboard::Escape);
	G_InputManager.RegisterKey(sf::Keyboard::Return);
}

template<typename T>
inline void DeleteAndNull(T *&obj) {
	delete obj;
	obj = nullptr;
}

void IntroGameState::Cleanup() {
	//Delete all of the pointers and set them back to null.
	DeleteAndNull(m_titleText);
	DeleteAndNull(m_pressStartText);
	DeleteAndNull(m_pressEscapeText);
	DeleteAndNull(m_backgroundArray);
	
	//Deregister the keys.
	G_InputManager.DeregisterKey(sf::Keyboard::Escape);
	G_InputManager.DeregisterKey(sf::Keyboard::Return);
}

void IntroGameState::Update(float delta, GameManager *game) {
	//Transition to other states depending upon key presses.
	if (G_InputManager.wasKeyReleased(sf::Keyboard::Escape)) {
		game->PopState();
	}
	else if (G_InputManager.wasKeyReleased(sf::Keyboard::Return)) {
		game->ChangeState(&MainMenuGameState::Instance());
	}
}

void IntroGameState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//Render the background first, then the text.
	target.draw(*m_backgroundArray, states);
	target.draw(*m_titleText, states);
	target.draw(*m_pressStartText, states);
	target.draw(*m_pressEscapeText, states);
}
