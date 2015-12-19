#pragma once
#include "GameStates/GameState.h"

#include <vector>

namespace sf {
	class Text;
}
/// <summary>
/// A game state for the main menu screen of the game.
/// Currently draws a title and a couple of menu options.
/// Currently selected menu option is shown with a different color, when you click
/// it changes to a third color. The colors are hard-coded in draw() - feel free
/// to change them if you like.
/// Currently you click a menu item by pressing then releasing space or enter while that
/// item is selected.
/// You can use the arrow keys to change the selected menu item.
/// If you call Init() out of order it will crash and freak out.
/// Does not gracefully handle a single instance being multiple places
/// in the game state stack.
/// </summary>
///
///	<state-changes>
/*
pushes on
(currently none)
planned	
(click Play menu item)->%PlayGameState%
(click Options menu item)->%OptionsMenuGameState%

changes on
(none)

pops on
(press escape)->
(click Exit menu item)->

%GameState% indicates an unimplemented game state.
*/
/// </state-changes>
///
/// <author>
/// Kevin Forest
/// </author>
class MainMenuGameState :
	public GameState
{
public:
	//GameStates use a singleton pattern.
	static MainMenuGameState &Instance() {
		static MainMenuGameState state;
		return state;
	}
private:
	//Prevents other classes from instantiating.
	MainMenuGameState() : m_titleText(nullptr), m_selectedItemClicked(false), m_selectedItem(0) {}
	//Do not implemented - ensures compiler can enforce singleton-ness.
	MainMenuGameState(const MainMenuGameState&);
	MainMenuGameState& operator=(const MainMenuGameState&);

public:
	virtual void Init(int width, int height);
	virtual void Cleanup();
	virtual void Update(float delta, GameManager *game);

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	//Drawables to render to screen. Menu items are selectable and clickable.
	sf::Text *m_titleText;
	std::vector<sf::Text *> m_menuItems;
	//Index of currently selected menu item.
	std::size_t m_selectedItem;
	//If true, the currently selected item is in the process of being clicked
	// (i.e. the key is down but has not yet been released).
	bool m_selectedItemClicked;

};

