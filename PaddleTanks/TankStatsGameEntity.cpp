#include "TankStatsGameEntity.h"
#include "AssetManager.h"
#include "message_types.h"
#include "Messaging\MessageDispatcher.h"

#include <SFML/Graphics.hpp>

#include <sstream>

TankStatsGameEntity::TankStatsGameEntity(sf::Vector2f pos, sf::Color const &textColor)
	: BaseGameEntity(BaseGameEntity::GetNextValidID()), m_textHealth(nullptr), m_textPower(nullptr), m_health(100), m_power(100)
{
	//Get the proper font
	sf::Font *f = G_FontManager.GetAsset("assets/DroidSansMono.ttf");
	//Create the text object, assign a value of "stats", and set the proper position and color.
	m_textHealth = new sf::Text("100", *f, 25);
	m_textHealth->setPosition(pos);
	m_textHealth->setColor(textColor);

	m_textPower = new sf::Text("100", *f, 25);
	m_textPower->setPosition(pos);
	m_textPower->setColor(textColor);
}

TankStatsGameEntity::~TankStatsGameEntity() {
	//Clean up.
	delete m_textHealth;
	delete m_textPower;
}

void TankStatsGameEntity::regenHealth() {
	//Use a string stream to convert the current score integer into a string.
	std::stringstream ssh;
	ssh << "Health: ";
	ssh << m_health;
	m_textHealth->setString(ssh.str());
	/*std::string output;
	std::stringstream ss;
	output += "Health: ";
	ss << m_health;
	output += ss.str();
	ss.str(std::string());
	ss.clear();
	output += "\nPower: ";
	ss << m_power;
	output += ss.str();
	m_text->setString(output);
	ss.str(std::string());*/
}

void TankStatsGameEntity::regenPower() {
	std::stringstream ssp;
	ssp << "\nPower: ";
	ssp << m_power;
	m_textPower->setString(ssp.str());
}

void TankStatsGameEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//Ta-draw
	target.draw(*m_textHealth, states);
	target.draw(*m_textPower, states);
}

//bool TankStatsGameEntity::HandleMessage(const Telegram& msg) {
//	switch (msg.Msg) {
//	case message_type::HIT:
//		return true;
//	default:
//		return false;
//	}
//}
//
//void TankStatsGameEntity::SendData() {
//	Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY, this->ID(), m_mapEntID, message_type::HIT, nullptr);
//}