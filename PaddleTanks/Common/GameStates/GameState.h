#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameManager.h"

#include <SFML/Graphics/Drawable.hpp>

/// <summary>
///		A GameState encapsulates the logic of a single state such as
///		a menu, splash screen, or the game play. Meant to be used as
///		singletons and making use of an Init()/Cleanup() pattern. 
///		Should be managed via a GameStateManager.
///		Note that an instantiated state is not ready for use - a call
///		to Init() is necessary to load the resources. Similarly a call
///		to Cleanup() places the state back in an unusable state.
///		It is the choice of the implementer on how out of sequence calls to
///		Cleanup() and Init() are resolved.
///		Calls to Pause() and Resume() only change a boolean value. It is
///		the responsibility of the derived class to actually implement
///		pause/resume functionality.
/// </summary>
/// <author>
///		Modified by Kevin Forest 
/// </author>
///Adapted from http://gamedevgeek.com/tutorials/managing-game-states-in-c/
class GameState : public sf::Drawable
{
public:
	//Initialise a gamestate scaled to the specified screen dimensions.
	virtual void Init(int width, int height) = 0;
	//Cleanup the resources in use by the state.
	virtual void Cleanup() = 0;

	//Control the pause variable. It is the responsibility of
	// the derived class to implement paused behaviour, should it
	// choose to do so.
	void Pause() { m_paused = true;	}
	void Resume() {	m_paused = false; }
	void TogglePause() { m_paused = !m_paused; }
	bool IsPaused() { return m_paused; }

	//Update the state by a simulation tick.
	virtual void Update(float delta, GameManager* game) = 0;
	
	//An opaque state should not have the states beneath it in the stack
	// rendered.
	bool IsOpaque() { return m_opaque; }
	void SetOpaque(bool opaque) { m_opaque = opaque; }

protected:
	GameState() : m_opaque(true), m_paused(false) {}
	//Render the state to screen.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

private:
	bool m_opaque;
	bool m_paused;
};

#endif