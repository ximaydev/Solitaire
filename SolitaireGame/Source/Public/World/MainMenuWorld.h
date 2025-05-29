#pragma once
#include "Framework/World.h"

class SMainMenuWorld : public SWorld
{
public:
	/** Constructor */
	SMainMenuWorld();

	/** Initializes the Main Menu World, preparing all necessary components. */
	virtual SBool Initialize() override;

private:
	/** Handler function triggered when the player requests to create a new match */
	void HandleCreateNewMatch();

	// Handler function triggered when the player requests exit the entire game application
	void HandleExitGame();
};