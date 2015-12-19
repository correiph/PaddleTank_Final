#pragma once
//#include "Globals.h"
#include "Entities/BaseGameEntity.h"
#include <SFML/Graphics/Drawable.hpp>

namespace sf {
	class Text;
	class Color;
}

/// <summary>
/// TankStatsGameEntity encapsulates a number (the player's score)
/// which can be incremented or reset to 0. Currently does not
/// use a state machine to do this, although it certainly could.
/// If you change the score through a different interface than
/// the provided Reset() and IncrementScore() methods, you 
/// should also call regen() to ensure the Text contains the
/// latest score.
/// </summary>
/// <author>
/// Kevin Forest 2015
/// </author>
class TankStatsGameEntity :
	public BaseGameEntity, public sf::Drawable
{
public:
	///Construct a TankStatsGameEntity at the specified position,
	/// which will be the top left corner of the text, with
	/// the specified color. Score starts at 0.
	TankStatsGameEntity(sf::Vector2f pos, sf::Color const &textColor);
	~TankStatsGameEntity();

	//Reset the score to 0 and regenerate the text.
	void Reset() {
		m_health = 100;
		m_power = 100;
		regenHealth();
	}
	void CurrentHealthPower() {
		regenHealth();
		regenPower();
	}

	//power management
	void RegeneratePower(float val) {
		if (!(m_power >= 100)){
			m_power += val;
		}
		regenPower();
	}
	void LosePower(float val) {
		m_power -= val;
		regenPower();
	}

	//health management
	void LoseHealth(float val) {
		m_health -= val;
		regenHealth();
	}
	void RegenerateHealth(float val) {
		if (!(m_health >= 100)){
			m_health += val;
		}
		if (val + m_health){
			m_health = 100.0f;
		}
		regenHealth();
	}

	void powerToZero(){
		m_power = 0.0f;
		regenPower();
	}

	//getters and setters
	float getHealth() {
		return m_health;
	}
	void setHealth(int health) {
		m_health = health;
	}
	float getPower() {
		return m_power;
	}
	void setPower(int power) {
		m_power = power;
	}

	//void SetMapEntityID(int eid) {
	//	m_mapEntID = eid;
	//}

	//bool HandleMessage(const Telegram& msg);

	//void SendData();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	//The visual representation of the current score.
	sf::Text *m_textHealth, *m_textPower;
	//Tne numerical values for tank stats
	float m_health;
	float m_power;
	//map ID
	int m_mapEntID;
	//Method that updates m_text such that it reads the current value of m_currentScore.
	// Called automatically by Reset() and IncrementScore()
	void regenHealth();
	void regenPower();
};