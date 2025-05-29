#pragma once
#include "Framework/World.h"

class SVictoryWorld : public SWorld
{
public:
    /** Initializes the Victory World, preparing all necessary components. */
    virtual SBool Initialize() override;

private:
    // Callback: Return to the main menu screen
    void HandleReturnToMainMenu();

    // Callback: Exit the entire game application
    void HandleExitGame();
};