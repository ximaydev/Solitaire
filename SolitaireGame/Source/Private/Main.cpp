#include "SolitaireGamePCH.h"
#include "Core/Console.h"

int main()
{
    // Initialzie Console
    GConsole->Initialize();

    // Initialize Engine
    if(GSolitaireEngine->Initialize())
    {
        // Create World
        GSolitaireEngine->CreateInitialMap<SGameBoardWorld>();
    }

    // This code runs when the player exits the game via the in-game menu (not by clicking the X button).
    // We manually call ConsoleCtrlHandler to simulate the same shutdown behavior as a forced window close.
    SConsole::ConsoleCtrlHandler(CTRL_CLOSE_EVENT);

    return 0;
}