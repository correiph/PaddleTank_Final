#pragma once
#include "GameStates/GameState.h"

class TiledBackground;

namespace sf {
	class Text;
}
/// <summary>
/// A game state for the introductory screen of the game.
/// Currently draws a cool background with some text on the screen.
/// If you call Init() out of order it will crash and freak out.
/// Does not gracefully handle a single instance being multiple places
/// in the game state stack.
/// </summary>
///
///	<state-changes>
/*
	pushes on
	(none)

	changes on
	(press enter)->MainMenuGameState

	pops on
	(press escape)->

*/
/// </state-changes>
///
/// <author>
/// Kevin Forest
/// </author>
class IntroGameState :
	public GameState
{
public:
	//GameStates use a singleton pattern.
	static IntroGameState &Instance() {
		static IntroGameState state;
		return state;
	}
private:
	//Prevents other classes from instantiating.
	IntroGameState() : m_titleText(nullptr), m_pressStartText(nullptr), m_pressEscapeText(nullptr), m_backgroundArray(nullptr) {}
	//Do not implemented - ensures compiler can enforce singleton-ness.
	IntroGameState(const IntroGameState&);
	IntroGameState& operator=(const IntroGameState&);

public:

	virtual void Init(int width, int height);
	virtual void Cleanup();
	
	virtual void Update(float delta, GameManager *game);
	
protected:
	//Render to the screen.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	//Drawables to render to the screen.
	sf::Text *m_titleText;
	sf::Text *m_pressStartText;
	sf::Text *m_pressEscapeText;
	TiledBackground *m_backgroundArray;

	
};

