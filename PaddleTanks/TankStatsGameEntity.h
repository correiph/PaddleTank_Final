
#pragma once
#include "Globals.h"
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

	// Increase power as a per-frame value, defined in Globals.h
	void RegeneratePower() {
		m_power += POWER_REGEN_RATE;
		regenPower();
	}

	// Decrease power per-method call, defined in Globals.h
	void LosePower() {
		m_power -= POWER_DRAIN_SHOT;
		regenPower();
	}

	//health management

	// Decrease health per-method call, defined in Globals.h
	void LoseHealth() {
		m_health -= HEALTH_DAMAGE_HIT;
		regenHealth();
	}
	// Increase health as a per-frame value, defined in Globals.h
	void RegenerateHealth() {
		m_health += HEALTH_REGEN_RATE;
		regenHealth();
	}

	//getters and setters
	int getHealth() {
		return m_health;
	}
	void setHealth(int health) {
		m_health = health;
	}
	int getPower() {
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
	int m_health;
	int m_power;
	//map ID
	int m_mapEntID;
	//Method that updates m_text such that it reads the current value of m_currentScore.
	// Called automatically by Reset() and IncrementScore()
	void regenHealth();
	void regenPower();
};