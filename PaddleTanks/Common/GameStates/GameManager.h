
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

class GameState;
namespace sf {
	class RenderWindow;
}

/// <summary>
///	The GameManager encapsulates the collection of GameStates and acts
///	like a stack. States can be pushed or popped onto the stack. Currently
/// only the top-most state is rendered and updated although you can easily
/// implemented more functionality by checking the Opaque attribute of the 
/// GameStates.
/// The GameManager must be initialised before use and should be Cleaned up 
/// after use.
/// The GameManager also encapsulates the game window and the game clock.
/// Run() is a blocking call that starts the game. If at anytime the
/// GameManager has an empty state stack, the game ends and Run() will return.
/// In addition, if a window close event occurs Run() will also exit.
/// A state that is popped will have GameState::CleanUp() called.
/// A state that is pushed will have GameState::Init() called.
/// A Change() is the equivalent of a Pop() followed by a Push(), except the state below
/// is not resumed (see below).
/// When a stated has another state pushed on top of it, that state will
/// be GameState::Paused(). When the state immediately above a paused state is popped
/// the state will be GameState::Resumed().
/// The GameManager will have unpredictable/undefined behaviour if you have the same
/// instance of a state in the stack multiple times. It is the responsibility of the
/// individual GameState implementations to handle out-of-sequence calls to the 
///	Init() and Cleanup() methods (note that this usually isn't worth the effort).
/// </summary>
/// <author>
///		Modified by Kevin Forest 2015
/// </author>
///		SFML-afied
///Adapted from http://gamedevgeek.com/tutorials/managing-game-states-in-c/
class GameManager
{
public:

	void Init(std::string const &title, int width=640, int height=480, 
		unsigned int bpp = 32U, bool fullscreen = false);
	void Cleanup();

	//Swap the current top state for a new state. The current
	// state will be cleanedup and the new state will be initialised.
	void ChangeState(GameState* state);
	//Push a new state onto the stack. Pauses the current state and
	// initialised the new state.
	void PushState(GameState* state);
	//Pops the current state off the stack. Cleansup the current state
	// and resumes the state below it.
	void PopState();

	//Run the game. Enters the main gameplay loop. Don't call this with an
	// empty state stack or an uninitialised GameEngine.
	void Run();

	//Lets you query if the game is running or if it has quit.
	bool Running() { return m_running; }
	void Quit() { m_running = false; }
	
	GameManager() : m_window(NULL) { }
	~GameManager() { }

private:
	//The game window.
	sf::RenderWindow *m_window;

	// the stack of states
	std::vector<GameState *> m_states;

	bool m_running;
	bool m_fullscreen;
};

#endif
