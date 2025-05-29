#pragma once
#include "Framework/World.h"

class SVictoryWorld : public SWorld
{
public:
    /** Constructor */
    SVictoryWorld(SUInt32 NewMoves) : PlayerMoves(NewMoves) {}

    /** Initializes the Victory World, preparing all necessary components. */
    virtual SBool Initialize() override;

protected:
    /** Indicates whether the player has entered their name and the console selector can be used */
    SBool NameTyped = false;

    /** Number of moves the player made during the game */
    SUInt32 PlayerMoves = 0;

private:
    // Callback: Return to the main menu screen
    void HandleReturnToMainMenu();

    // Callback: Exit the entire game application
    void HandleExitGame();

    // Callback: Called when the player confirms their name
    void HandleNameCommitted(const SWString& Line);
};