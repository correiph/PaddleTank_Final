//#include <vld.h>
#include "GameStates/GameManager.h"
#include "IntroGameState.h"

#include <iostream>

#include "tinyxml\tinyxml2.h"

#include <iostream>

int main(int argc, char *argv[]) {
	
	//Create the game manager and initialise it.	
	GameManager engine;
	engine.Init("Paddle Tanks", 800, 600, 32U, false);
	//Push the introduction state (splash screen) onto the state stack.
	engine.PushState(&IntroGameState::Instance());
	//Run the game.
	engine.Run();
	//Clean up the game engine.
	engine.Cleanup();
	//And we are done!
	return 0;
}
