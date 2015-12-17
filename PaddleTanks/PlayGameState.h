#pragma once
#include "GameStates/GameState.h"

class Map;

class PlayGameState :
	public GameState
{
public:
	//GameStates use a singleton pattern.
	static PlayGameState &Instance() {
		static PlayGameState state;
		return state;
	}
	virtual ~PlayGameState();

private:
	//Prevents other classes from instantiating.
	PlayGameState() : m_map(nullptr) {}
	//Do not implemented - ensures compiler can enforce singleton-ness.
	PlayGameState(const PlayGameState&);
	PlayGameState& operator=(const PlayGameState&);

public:
	virtual void Init(int width, int height);
	virtual void Cleanup();

	virtual void Update(float delta, GameManager *game);

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	//Attributes go here.
	Map *m_map;
};

