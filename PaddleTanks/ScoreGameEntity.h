#pragma once

#include "Entities/BaseGameEntity.h"

#include <SFML/Graphics/Drawable.hpp>

namespace sf {
	class Text;
	class Color;
}

/// <summary>
/// ScoreGameEntity encapsulates a number (the player's score)
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
class ScoreGameEntity :
	public BaseGameEntity, public sf::Drawable
{
public:
	///Construct a ScoreGameEntity at the specified position,
	/// which will be the top left corner of the text, with
	/// the specified color. Score starts at 0.
	ScoreGameEntity(sf::Vector2f pos, sf::Color const &textColor);
	~ScoreGameEntity();

	//Reset the score to 0 and regenerate the text.
	void Reset() {
		m_currentScore = 0;
		regen();
	}
	//Increment the score by 1 and regenerate the text.
	void IncrementScore() {
		m_currentScore++;
		regen();
	}

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	//The visual representation of the current score.
	sf::Text *m_text;
	//The numerical value of the current score.
	int m_currentScore;
	//Method that updates m_text such that it reads the current value of m_currentScore.
	// Called automatically by Reset() and IncrementScore()
	void regen();
};