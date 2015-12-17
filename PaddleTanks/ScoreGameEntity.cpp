#include "ScoreGameEntity.h"
#include "AssetManager.h"

#include <SFML/Graphics.hpp>

#include <sstream>

ScoreGameEntity::ScoreGameEntity(sf::Vector2f pos, sf::Color const &textColor)
	: BaseGameEntity(BaseGameEntity::GetNextValidID()), m_text(nullptr), m_currentScore(0) 
{
	//Get the proper font
	sf::Font *f = G_FontManager.GetAsset("assets/DroidSansMono.ttf");
	//Create the text object, assign a score of "0", and set the proper position and color.
	m_text = new sf::Text("0", *f, 40);
	m_text->setPosition(pos);
	m_text->setColor(textColor);
}

ScoreGameEntity::~ScoreGameEntity() {
	//Clean up.
	delete m_text;
}

void ScoreGameEntity::regen() {
	//Use a string stream to convert the current score integer into a string.
	std::stringstream ss;
	ss << m_currentScore;
	m_text->setString(ss.str());
}

void ScoreGameEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//Ta-draw
	target.draw(*m_text, states);
}